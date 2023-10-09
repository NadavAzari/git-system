#ifndef MY_GIT_REPO_H
#define MY_GIT_REPO_H

#include <iostream>

#define GIT_NAME "git"
#define GIT_EXTENSION ".snit"

class repo {
public:
    static repo* create_repo(std::string path);
private:
    std::string worktree_dir;
    std::string git_dir;

    repo(std::string, bool already_created);

    std::string get_path(std::string path);
    std::string create_repo_dirs(std::string path);
    std::string create_repo_file(std::string path);
};


#endif //MY_GIT_REPO_H
