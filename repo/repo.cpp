#include "repo.h"
#include <filesystem>
#include <fstream>

repo* repo::create_repo(std::string path) {
    return new repo(path, false);
}

repo::repo(std::string path, bool already_created) {
    git_dir = std::filesystem::path(path) / std::filesystem::path(GIT_EXTENSION);
    worktree_dir = path;

    if(!(already_created || std::filesystem::is_directory(git_dir))) {
        //TODO: error
    }
    ini_conf = new conf;
    std::string config_path = get_path("config");

    if(std::filesystem::exists(config_path)) {
        ini_conf->from_file(config_path);
    } else if(already_created) {
        //Todo: error
    } else {
        ini_conf->default_conf();
        ini_conf->write(config_path);
    }

    create_repo_dirs("branches");
    create_repo_dirs("objects");
    create_repo_dirs("refs/tags");
    create_repo_dirs("refs/heads");
}

std::string repo::get_path(std::string path) {
    return std::filesystem::path(git_dir) / std::filesystem::path(path);
}

std::string repo::create_repo_file(std::string path) {
    std::string p = get_path(path);
    std::filesystem::path parent = std::filesystem::path(p).parent_path();
    create_repo_dirs(parent.string());
    std::ofstream file(p);
    if(file.is_open()){
        file.close();
        return p;
    }
    return "";
}

std::string repo::create_repo_dirs(std::string path) {
    std::filesystem::path p = get_path(path);
    if(std::filesystem::exists(p) && std::filesystem::is_directory(p)) {
        return p.string();
    }

    std::filesystem::create_directories(p);
    return p.string();
}