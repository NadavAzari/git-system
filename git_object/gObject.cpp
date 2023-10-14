#include "gObject.h"
#include "../files.h"

std::string gObject::get_path_by_hash(std::string hash) {
    std::string dir = hash.substr(0,2);
    std::string file_name = hash.substr(2, hash.length() - 2);
    std::string obj_path = std::filesystem::path("objects") / std::filesystem::path(dir) /  std::filesystem::path(file_name);
    return obj_path;
}

gObject* handle_obj(std::string type, byte* data, long len) {
    if(type == BLOB_TYPE) {
        return new gBlob(data, len);
    }
    else if(type == COMMIT_TYPE) {
        gCommit* commit = new gCommit();
        commit->deserialize(data, len);
        return commit;
    }
    else if(type == TREE_TYPE) {
        gTree* tree = new gTree();
        tree->deserialize(data, len);
        return tree;
    }
    return nullptr;
}

gObject* gObject::from_file(std::string hash) {
    repo* r = repo::find_repo();
    if(r == nullptr) {
        //TODO: ERROR
    }
    std::string path = r->get_path(get_path_by_hash(hash));
    std::vector<byte> data = crypto::decompressData(readFileToVector(path));
    auto it = std::find(data.begin(), data.end(),0x20);
    if(it == data.end()){
        //TODO: error
    }

    int idx = std::distance(data.begin(), it);
    std::string type(data.begin(), data.begin() + idx);
    it = std::find(data.begin() + idx + 1, data.end(), '\x00');
    int nIdx = std::distance(data.begin(), it);
    std::string size(data.begin() + idx + 1, data.begin() + nIdx);
    long block_size = std::stol(size);
    byte dataRes[block_size];
    std::copy(data.begin() + nIdx + 1, data.end(), dataRes);
    delete r;
    return handle_obj(type, dataRes, block_size);
 }

void gObject::to_file() {
    repo* r = repo::find_repo();
    if(r == nullptr) {
        //TODO: ERROR
    }
    std::vector<byte> data = serialize();
    std::string sha1 = crypto::sha1OnVector(data);
    std::string path = get_path_by_hash(sha1);
    std::filesystem::create_directory(r->get_path(std::filesystem::path("objects") / sha1.substr(0,2)));
    std::vector<byte> res;
    for(const byte& b : type) {
        res.push_back(b);
    }
    res.push_back(' ');
    std::string sz = std::to_string(data.size());
    for(const char& c: sz) {
        res.push_back(static_cast<byte>(c));
    }
    res.push_back('\x00');
    for(const byte b : data) {
        res.push_back(b);
    }

    std::vector<byte> compressedRes = crypto::compressData(res);
    std::ofstream output(r->get_path(path), std::ios::binary);
    for(byte b: compressedRes) {
        output.write(reinterpret_cast<char*>(&b), sizeof(b));
    }
    output.close();
    delete r;
}

std::string gObject::get_hash() {
    std::vector<byte> data = serialize();
    std::string sha1 = crypto::sha1OnVector(data);
    return sha1;
}

void gObject::create_objs_from_path(std::string path, std::vector<gObject*>& vec) {
    if(std::filesystem::is_directory(path)) {
        std::vector<std::string> files;
        walk(files, path);
        for(const std::string file : files) {
            create_objs_from_path(file, vec);
        }
    } else {
        std::vector<byte> bytes = readFileToVector(path);
        gBlob* blob = new gBlob(bytes.data(), bytes.size());
        vec.push_back(blob);
    }
}
