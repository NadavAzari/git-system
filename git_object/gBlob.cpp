#include "gObject.h"

gBlob::gBlob(byte *data, long size) : gObject(data, size){
    type = BLOB_TYPE;
}

gBlob::gBlob() : gObject() {
    type = BLOB_TYPE;
}

std::vector<byte> gBlob::serialize() {
    return std::vector<byte>(data, data + dLen);
}

void gBlob::deserialize(byte *sData, int len) {
    if(data == nullptr) {
        delete[] data;
    }
    data = new byte[len];
    std::memcpy(data,sData, len);
}