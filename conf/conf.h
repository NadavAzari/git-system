#ifndef MY_GIT_CONF_H
#define MY_GIT_CONF_H

#include <iostream>
#include <map>
#include <vector>
#include <set>
#include <fstream>
#include <sstream>

struct ini_section {
    std::string section_name;
    std::map<std::string, std::string> values;
};

class conf {
private:
    std::map<std::string, ini_section*> sections;
    void read_string(std::string);
public:
    void add_section(std::string);
    void set(std::string sec_name, std::string name, std::string value);
    std::string get(std::string sec_name, std::string name);
    void write(std::string file_path);
    std::string to_string();
    void from_file(std::string path);
    void default_conf();

    ~conf();
};


#endif //MY_GIT_CONF_H
