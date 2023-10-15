#include "gObject.h"
#include "../index/index_file.h"

gTree::gTree(std::vector<gTreeLeaf> ls) : gObject(){
    type = TREE_TYPE;
    for(const gTreeLeaf& leaf: ls) {
        leafs.push_back(leaf);
    }
}

std::vector<byte> gTree::serialize() {
    std::vector<byte> result;
    for(const gTreeLeaf l : leafs) {
        result.insert(result.end(), l.path.begin(), l.path.end());
        result.push_back('\0');
        auto bytes = crypto::hashStringToBytes(l.hash);
        result.insert(result.end(), bytes.begin(), bytes.end());
    }
    return result;
}

std::pair<std::vector<byte>::iterator, gTreeLeaf> parseLeaf(std::vector<byte>& data, std::vector<byte>::iterator start) {
    const auto pit = std::find(start, data.end(), 0);
    std::string path(start, pit);
    std::vector<byte> hash(pit + 1, pit + 21);
    std::string hashStr = crypto::hashBytesToString(hash.data());

    auto pos = pit + 21;
    gTreeLeaf leaf;

    leaf.path = path;
    leaf.hash = hashStr;

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

gTree *gTree::create_worktree() {
    index_file* idx = index_file::load();
    gTree* tree = new gTree;
    std::set<index_file_entry> entries = idx->get_entries();

    for(const index_file_entry entry : entries) {
        gTreeLeaf leaf;
        leaf.path = entry.file_path;
        leaf.hash = entry.obj_hash;
        tree->add_leaf(leaf);
    }

    delete idx;
    return tree;
}

void sortTree(gTree* t) {
    std::sort(t->get_leafs().begin(), t->get_leafs().end(), [](gTreeLeaf l1, gTreeLeaf l2) {
        return l1.hash < l2.hash;
    });
}

bool gTree::equals(gTree *t1, gTree *t2) {
    sortTree(t1);
    sortTree(t2);

    return t1->get_hash() == t2->get_hash();
}

