#ifndef MY_GIT_GOBJECT_H
#define MY_GIT_GOBJECT_H

#include <iostream>
#include <vector>
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
    long dLen = 0;
    byte* data = nullptr;

    static std::string get_path_by_hash(std::string hash);
public:
    gObject(byte data[], long size);
    gObject(){};
    virtual std::vector<byte> serialize() = 0;
    void to_file(repo* r);
    virtual void deserialize(byte data[], int len) = 0;
    static gObject* from_file(repo* r, std::string hash);

    byte* get_data() {return data;};
    ~gObject(){delete[] data;}
};

class gBlob : public gObject {
public:
    gBlob(byte data[], long size);
    gBlob();
    void deserialize(byte sData[], int len);
    std::vector<byte> serialize();
};

#endif
