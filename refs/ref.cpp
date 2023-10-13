#include "ref.h"

std::string get_ref_from_file(repo* r, std::string path) {
    std::ifstream file(path);
    std::string fileContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    if(fileContent.substr(0,5) == "ref: ") {
        return get_ref_from_file(r, r->get_path(fileContent.substr(5, fileContent.length() - 6)));
    }

    return fileContent.substr(0, fileContent.length() - 1);
}

ref::ref(std::string ref_name) {
    repo* r = repo::find_repo();
    ref_path = std::filesystem::path(r->get_path("refs")) / ref_name;
    delete r;
}

ref *ref::fetch_reference(std::string ref_name) {
    ref* re = new ref(ref_name);
    repo* r = repo::find_repo();
    re->ref_to =get_ref_from_file(r, ref_name);
    delete r;
    return re;
}

std::string ref::get_reference() {return ref_to;}