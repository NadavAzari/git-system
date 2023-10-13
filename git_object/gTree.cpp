#include "gObject.h"

gTree::gTree(std::vector<gTreeLeaf> ls) : gObject(){
    type = TREE_TYPE;
    for(const gTreeLeaf& leaf: ls) {
        leafs.push_back(leaf);
    }
}

std::vector<byte> gTree::serialize() {
    std::vector<byte> result;
    for(const gTreeLeaf l : leafs) {
        result.insert(result.end(), l.mode.begin(), l.mode.end());
        result.push_back(' ');
        result.insert(result.end(), l.path.begin(), l.path.end());
        result.push_back('\0');
        auto bytes = crypto::hashStringToBytes(l.hash);
        result.insert(result.end(), bytes.begin(), bytes.end());
    }
    return result;
}

std::pair<std::vector<byte>::iterator, gTreeLeaf> parseLeaf(std::vector<byte> data, std::vector<byte>::iterator start) {
    const auto spc = std::find(start,data.end(), 0x20);
    if (spc == data.end()) {
        //TODO: ERROR
    }

    std::string mode(start,spc);
    const auto pit = std::find(spc + 1, data.end(), '\0');
    std::string path(spc + 1, pit);
    std::string hash(pit + 1, pit + 21);

    auto pos = pit + 22;
    gTreeLeaf leaf = {
        .path=path,
        .hash=hash,
        .mode=mode
    };

    return {
        pos,
        leaf
    };
}

void gTree::deserialize(byte *sData, int len) {
    std::vector<byte> data(sData, sData + len);
    std::vector<byte>::iterator pos = data.begin();
    do {
        auto res = parseLeaf(data, pos);
        pos = res.first;
        leafs.push_back(res.second);
    }while(pos < data.end());
}