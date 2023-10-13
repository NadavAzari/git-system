#ifndef MY_GIT_CRYPTO_H
#define MY_GIT_CRYPTO_H

#include <iostream>
#include <vector>
#include <zlib.h>
#include <openssl/sha.h>

typedef unsigned char byte;

namespace crypto {
    extern std::string sha1OnVector(const std::vector<unsigned char>& data);
    extern std::vector<byte> compressData(const std::vector<byte>& input);
    extern std::vector<byte> decompressData(const std::vector<byte>& compressedData);
    extern std::string hashBytesToString(byte* hash);
    std::vector<byte> hashStringToBytes(const std::string& sha1);
}

#endif
