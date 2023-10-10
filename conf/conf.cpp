#include "conf.h"

std::vector<std::string> split(std::string s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);

    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }

    return tokens;
}

std::string conf::to_string() {
    std::string res = "";
    for(const auto& pair1 : sections) {
        res += "[" + pair1.first + "]\n";
        for(const auto& pair : pair1.second->values) {
            res += pair.first + " = " + pair.second + "\n";
        }
    }
    return res;
}

void conf::add_section(std::string sec_name) {
    if(sections.find(sec_name) == sections.end()) {
        sections[sec_name] = new ini_section{sec_name};
    }
}

void conf::set(std::string sec_name, std::string name, std::string value) {
    if(sections.find(sec_name) != sections.end()) {
        ini_section* sec = sections[sec_name];
        sec->values[name] = value;
    }
}

std::string conf::get(std::string sec_name, std::string name) {
    if(sections.find(sec_name) == sections.end()) {
        return "";
    }
    ini_section* sec = sections[sec_name];
    if(sec->values.find(name) == sec->values.end()) {
        return "";
    }
    return sec->values[name];
}

void conf::write(std::string file_path) {
    std::ofstream file(file_path);
    file << to_string();
    file.close();
}

void conf::read_string(std::string str) {
    std::vector<std::string> lines = split(str, '\n');
    std::string c_sec_name = "";
    for(int i = 0; i < lines.size(); ++i) {
        std::string line = lines[i];
        if(line.length() == 0){
            continue;
        }
        if(line[0] == '[') {
            std::string sec_name = line.substr(1);
            sec_name.pop_back();
            add_section(sec_name);
            c_sec_name = sec_name;
        } else {
            std::vector<std::string> parts = split(line, '=');
            std::string key = parts[0];
            key.pop_back();
            std::string value = parts[1].substr(1);
            set(c_sec_name, key,value);
        }
    }
}

void conf::from_file(std::string path) {
    std::ifstream input_file(path);
    std::string fileContent((std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>());
    input_file.close();
    read_string(fileContent);
}

void conf::default_conf() {
    add_section("core");
    set("core", "repositoryformatversion", "0");
    set("core", "filemode", "false");
    set("core", "bare", "false");
}

conf::~conf() {
    for(const auto& pair : sections) {
        delete pair.second;
    }
}