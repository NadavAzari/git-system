#ifndef MY_GIT_INDEX_FILE_H
#define MY_GIT_INDEX_FILE_H

#include <iostream>
#include <set>
#include <vector>
#include <filesystem>
#include <fstream>

#include "../repo/repo.h"
#include "../git_object/gObject.h"

typedef unsigned char byte;

struct index_file_entry {
    std::string file_path;
    std::string obj_hash;
    std::string obj_path;

    bool operator<(const index_file_entry& other) const {
        return file_path < other.file_path;
    }

    bool operator==(const index_file_entry& other) const {
        return file_path == other.file_path;
    }
};


class index_file {
private:
    std::set<index_file_entry> entries;
    std::set<std::string> files;
public:
    void add_entry(index_file_entry entry);
    void save();
    void remove_entry(std::string path);
    std::set<std::string>& get_files();
    std::set<index_file_entry>& get_entries();
    static index_file* load();
};


#endif //MY_GIT_INDEX_FILE_H
