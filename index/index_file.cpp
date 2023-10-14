#include "index_file.h"
#include "../files.h"

void index_file::add_entry(index_file_entry entry) {
    entries.insert(entry);
    files.insert(entry.file_path);
}

void index_file::remove_entry(std::string file_path) {
    auto files_it = files.find(file_path);
    if(files_it != files.end()) {
        auto entries_it = entries.begin();
        std::advance(entries_it, std::distance(files.begin(), files_it));
        entries.erase(entries_it);
    }
    files.erase(file_path);
}

void index_file::save() {
    repo* r = repo::find_repo();
    std::string path = r->get_path("index");
    delete r;
    std::ofstream file(path);
    for(const index_file_entry& entry : entries) {
        file << entry.file_path << '\0';
        file << entry.obj_path << '\0';
        file << entry.obj_hash;
    }

    file.close();
}

index_file *index_file::load() {
    repo* r = repo::find_repo();
    std::string path = r->get_path("index");
    delete r;
    std::vector<byte> data = readFileToVector(path);
    index_file* file = new index_file;
    auto pos = data.begin();
    while(pos < data.end()) {
        index_file_entry entry;
        auto end = std::find(pos, data.end(), '\0');
        entry.file_path = std::string(pos, end);
        pos = end + 1;
        end = std::find(pos, data.end(), '\0');
        entry.obj_path = std::string(pos, end);
        entry.obj_hash = std::string(end + 1, end + 41);
        pos = end + 41;
        file->add_entry(entry);
    }
    return file;
}

std::set<std::string> &index_file::get_files() {
    return files;
}

std::set<index_file_entry>& index_file::get_entries() {
    return entries;
}