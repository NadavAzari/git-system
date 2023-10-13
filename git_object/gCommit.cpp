#include "gObject.h"

gCommit::gCommit(std::unordered_map<std::string, std::string> kv) : gObject(){
    type = COMMIT_TYPE;
    for(const auto pair: kv) {
        commit_data[pair.first] = pair.second;
    }
}

std::vector<byte> gCommit::serialize() {
    std::string result = "";
    for(const auto& pair : commit_data) {
        if(pair.first == "") continue;
        result += pair.first + " ";

        size_t found = pair.second.find("\n");
        while (found != std::string::npos) {
            std::string s = pair.second;
            s.replace(found,1,"\n ");
            found = pair.second.find("\n", found + 3);
        }

        result += pair.second + '\n';

    }

    result += "\n" + commit_data[""] + "\n";

    std::vector<byte> v;
    for(const char c : result){
        v.push_back(c);
    }
    return v;
}

void gCommit::deserialize(byte *sData, int len) {
    std::string data(sData, sData + len);

    int start = 0;
    int end = data.find("\n\n");
    while(start < end) {
        int space = data.find(" ", start);
        int newLine = data.find("\n", start);

        std::string key = data.substr(start, space - start);

        while(data[newLine + 1] == ' ') {
            newLine = data.find('\n', newLine + 1);
        }
        std::string value = data.substr(space + 1,  newLine - space - 1);
        commit_data[key] = value;
    }
    commit_data[""] = data.substr(end, data.length() - end);
}