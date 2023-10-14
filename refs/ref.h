#ifndef MY_GIT_REF_H
#define MY_GIT_REF_H

#include <iostream>
#include <fstream>
#include <filesystem>
#include "../repo/repo.h"

class ref {
private:
    ref(std::string ref_name);
public:
    std::string ref_path;
    std::string ref_to;
    std::string get_reference();
    static ref* fetch_reference(std::string ref_name);
    void create_ref();
};


#endif //MY_GIT_REF_H
