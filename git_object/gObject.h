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
};

class gBlob : public gObject {
private:
    long dLen;
    byte* data;
public:
    gBlob(byte data[], long size);
    void deserialize(byte sData[], int len) override;
    std::vector<byte> serialize() override;
};

class gCommit : public gObject {
private:
    std::unordered_map<std::string, std::string> commit_data;
public:
    gCommit(std::unordered_map<std::string, std::string> kv);
    void deserialize(byte sData[], int len) override;
    std::vector<byte> serialize() override;
};


struct gTreeLeaf {
    std::string mode;
    std::string path;
    std::string hash;
};

class gTree : public gObject {
private:
    std::vector<gTreeLeaf> leafs;
public:
    gTree(std::vector<gTreeLeaf> leafs);
    void deserialize(byte sData[], int len) override;
    std::vector<byte> serialize() override;
};

#endif
