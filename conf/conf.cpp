#include "conf.h"

std::string conf::to_string() {
    std::string res = "";
    for(int i = 0; i < sections.size(); ++i) {
        res += "[" + sections[i].section_name + "]\n";
        for(const auto& pair : sections[i].values) {
            res += pair.first + " = " + pair.second + "\n";
        }
    }
    return res;
}

void conf::add_section(std::string sec_name) {
    if(section_names.insert(sec_name).second) {
        sections.push_back(ini_section{sec_name});
    }
}

void conf::set(std::string name, std::string value) {
    sections[sections.size() - 1].values[name] = value;
}

void conf::write(std::string file_path) {
    std::ofstream file(file_path);
    file << to_string();
    file.close();
}