#include "gObject.h"

gBlob::gBlob(byte *d, long size) : gObject(){
    type = BLOB_TYPE;
    dLen = size;
    data = new byte[size];
    memcpy(data,d, size);
}

std::vector<byte> gBlob::serialize() {
    return std::vector<byte>(data, data + dLen);
}

void gBlob::deserialize(byte *sData, int len) {
    if(data != nullptr) {
        delete[] data;
    }
    data = new byte[len];
    std::memcpy(data,sData, len);
}