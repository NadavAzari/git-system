#ifndef MY_GIT_GOBJECT_H
#define MY_GIT_GOBJECT_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <algorithm>

#include "../crypto/crypto.h"
#include "../repo/repo.h"

#define COMMIT_TYPE "commit"
#define TREE_TYPE "tree"
#define TAG_TYPE "tag"
#define BLOB_TYPE "blob"

typedef unsigned char byte;

class gObject {
protected:
    std::string type;

    static std::string get_path_by_hash(std::string hash);
public:
    virtual std::vector<byte> serialize() = 0;
    void to_file();
    virtual void deserialize(byte data[], int len) = 0;
    static gObject* from_file(std::string hash);
    static void create_objs_from_path(std::string path, std::vector<gObject*>& vec);

    std::string get_hash();
};

class gBlob : public gObject {
private:
    long dLen;
    byte* data;
public:
    gBlob(byte data[], long size);
    void deserialize(byte sData[], int len) override;
    std::vector<byte> serialize() override;
    ~gBlob(){
        if(data != nullptr) {
            delete[]data;
        }
    }
};

class gCommit : public gObject {
private:
    std::unordered_map<std::string, std::string> commit_data;
public:
    gCommit() : gObject() {type = COMMIT_TYPE;};
    gCommit(std::unordered_map<std::string, std::string> kv);
    void deserialize(byte sData[], int len) override;
    std::vector<byte> serialize() override;
    void set(std::string key, std::string value);
    std::string get(std::string key);
    static bool commit(std::string message);
};


struct gTreeLeaf {
    std::string path;
    std::string hash;

    bool operator==(const gTreeLeaf& other) const {
        return path == other.path && hash == other.hash;
    }

    bool operator<(const gTreeLeaf& other) const {
        return hash < other.hash;
    }
};

class gTree : public gObject {
private:
    std::vector<gTreeLeaf> leafs;
public:
    gTree() : gObject() {type = TREE_TYPE;};
    gTree(std::vector<gTreeLeaf> leafs);
    void add_leaf(gTreeLeaf leaf) {leafs.push_back(leaf);};
    void deserialize(byte sData[], int len) override;
    std::vector<byte> serialize() override;
    std::vector<gTreeLeaf>& get_leafs() {return leafs;};

    static gTree* create_worktree();
    static bool equals(gTree* t1, gTree* t2);
};

#endif
