#ifndef MY_GIT_CONF_H
#define MY_GIT_CONF_H

#include <iostream>
#include <map>
#include <vector>
#include <set>
#include <fstream>

struct ini_section {
    std::string section_name;
    std::map<std::string, std::string> values;
};

class conf {
private:
    std::set<std::string> section_names;
    std::vector<ini_section> sections;
public:
    void add_section(std::string);
    void set(std::string name, std::string value);
    void write(std::string file_path);
    std::string to_string();
};


#endif //MY_GIT_CONF_H
