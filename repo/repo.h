#ifndef MY_GIT_REPO_H
#define MY_GIT_REPO_H

#include <iostream>
#include <filesystem>
#include <fstream>
#include "../conf/conf.h"

#define GIT_NAME "snit"
#define GIT_EXTENSION ".snit"

class repo {
private:
    std::string worktree_dir;
    std::string git_dir;
    conf* ini_conf;

    repo(std::string, bool already_created);
    std::string create_repo_dirs(std::string path);
    std::string create_repo_file(std::string path);
public:
    static repo* find_repo();
    static repo* create_repo(std::string path);
    std::string get_path(std::string path);
};

#endif //MY_GIT_REPO_H
