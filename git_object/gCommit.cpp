#include "gObject.h"
#include "../refs/ref.h"

gCommit::gCommit(std::unordered_map<std::string, std::string> kv) : gObject(){
    type = COMMIT_TYPE;
    for(const auto pair: kv) {
        commit_data[pair.first] = pair.second;
    }
}

std::vector<byte> gCommit::serialize() {
    std::string result = "";
    for(const auto& pair : commit_data) {
        if(pair.first == "") {continue;}
        result += pair.first + " ";

        size_t found = pair.second.find("\n");
        while (found != std::string::npos) {
            std::string s = pair.second;
            s.replace(found,1,"\n ");
            found = pair.second.find("\n", found + 3);
        }

        result += pair.second + '\n';

    }

    result += "\n" + commit_data[""] + "\n";

    std::vector<byte> v;
    for(const char c : result){
        v.push_back(c);
    }
    return v;
}

void gCommit::deserialize(byte *sData, int len) {
    std::string data(sData, sData + len);

    int start = 0;
    int end = data.find("\n\n");
    while(start < end) {
        int space = data.find(" ", start);
        int newLine = data.find("\n", start);

        std::string key = data.substr(start, space - start);

        while(data[newLine + 1] == ' ') {
            newLine = data.find('\n', newLine + 1);
        }
        std::string value = data.substr(space + 1,  newLine - space - 1);
        commit_data[key] = value;
        start = space + 1 + newLine - space;
    }
    commit_data[""] = data.substr(end, data.length() - end);
}

void gCommit::set(std::string key, std::string value) {
    commit_data[key] = value;
}

std::string gCommit::get(std::string key) {
    if(commit_data.find(key) == commit_data.end()) {
        return "";
    }

    return commit_data[key];
}

bool gCommit::commit(std::string message) {
    repo* rep = repo::find_repo();
    gCommit* commit = new gCommit;
    gTree* tree = gTree::create_worktree();
    std::string branch = rep->get_current_branch();
    ref* r = ref::fetch_reference("HEAD");
    if(r->get_reference() != "") {
        commit->set("parent", r->get_reference());
        gCommit* parent = dynamic_cast<gCommit*>(gObject::from_file(r->get_reference()));
        std::string last_tree_hash = parent->get("tree");

        if(tree->get_hash() == last_tree_hash) {
            delete rep;
            delete commit;
            delete r;
            delete tree;
            delete parent;

            return false;
        }



        delete parent;
    }
    tree->to_file();
    commit->set("tree", tree->get_hash());
    commit->set("branch", branch);
    commit->set("", message);


    commit->to_file();
    r->ref_path = std::filesystem::path(rep->get_path("refs")) / "heads" / rep->get_current_branch();
    r->ref_to = commit->get_hash();
    r->create_ref();

    delete tree;
    delete commit;
    delete rep;
    delete r;
    return true;
}