#include "crypto.h"
namespace crypto {
    std::string sha1OnVector(const std::vector<unsigned char>& data) {
        byte buffer[SHA_DIGEST_LENGTH];
        SHA1(data.data(), data.size(), buffer);

        char result[SHA_DIGEST_LENGTH * 2 + 1];
        for(int i = 0; i < SHA_DIGEST_LENGTH; ++i) {
            sprintf(result + (i * 2), "%02x", buffer[i]);
        }
        result[SHA_DIGEST_LENGTH * 2] = '\0';
        return result;
    }

    std::vector<byte> compressData(const std::vector<byte>& input) {
        std::vector<byte> compressedData;
        uLong inputSize = input.size();
        uLong compressedSize = compressBound(inputSize);
        compressedData.resize(compressedSize);

        int result = compress(compressedData.data(), &compressedSize, input.data(), inputSize);

        if (result == Z_OK) {
            compressedData.resize(compressedSize);
        } else {
            //Todo: error
        }

        return compressedData;
    }

    std::vector<byte> decompressData(const std::vector<byte>& compressedData) {
        std::vector<byte> decompressedData;

        uLong inputSize = compressedData.size();
        uLong decompressedSize = inputSize * 2;
        decompressedData.resize(decompressedSize);

        int result = uncompress(decompressedData.data(), &decompressedSize, compressedData.data(), inputSize);

        if (result == Z_OK) {
            decompressedData.resize(decompressedSize);
        } else {
            //TODO: Error
        }

        return decompressedData;
    }
}