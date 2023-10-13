#ifndef MY_GIT_REF_H
#define MY_GIT_REF_H

#include <iostream>
#include <fstream>
#include <filesystem>]
#include "../repo/repo.h"

class ref {
private:
    std::string ref_path;
    std::string ref_to;
    ref(std::string ref_name);
public:
    std::string get_reference();
public ref* fetch_reference(std::string ref_name);
};


#endif //MY_GIT_REF_H
