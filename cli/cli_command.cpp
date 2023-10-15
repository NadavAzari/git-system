#include "cli_command.h"
#include "../files.h"
#include "../index/index_file.h"
#include "../refs/ref.h"
#include "../colors/colors.h"

void print_commit_history(gCommit* commit, std::string branch = "") {
    YELLOW();
    std::cout << "Commit ";
    PURPLE();
    std::cout << commit->get_hash() << "  ";
    YELLOW();
    if(branch == "" || branch != commit->get("branch")) {
        std::cout << "(";
        CYAN();
        std::cout << "HEAD -> ";
        RED();
        std::cout << commit->get("branch");
        YELLOW();
        std::cout << ")\n";
    } else {
        std::cout << "\n";
    }
    WHITE();
    std::cout << "  Message:\n\t" << commit->get("").substr(2) << "\n";
    if(commit->get("parent") == "") {
        return;
    }

    gCommit* parent = dynamic_cast<gCommit*>(gObject::from_file(commit->get("parent")));
    delete commit;
    print_commit_history(parent, parent->get("branch"));
}

cli_execution_result init_func(std::vector<std::string> args) {
    std::filesystem::path path = std::filesystem::current_path();
    cli_execution_result res;
    if(args.size() != 0) {
        path /= args[0];
    }
    if(std::filesystem::exists(path / std::filesystem::path(GIT_EXTENSION))) {
        res.message = "Error: dir is already a repo\n";
        res.succeed = false;
        return res;
    }
    repo* r =  repo::create_repo(path);
    delete r;
    res.message = "Initialized empty repository\n";
    res.succeed = true;
    return res;
}

struct worktree_file {
    std::string path;
    std::string hash;
    bool staged = false;
};

void print_changes(std::string prefix, std::vector<worktree_file>& files, bool is_staged) {
    for(const worktree_file f : files) {
        if(f.staged != is_staged) {
            continue;
        }
        std::cout << "\t" << prefix << f.path << "\n";
    }
}

void handle_changes(std::vector<worktree_file>& created, std::vector<worktree_file>& modified, std::vector<worktree_file>& deleted) {
    index_file* idx = index_file::load();
    std::vector<worktree_file> final_created;
    std::vector<worktree_file> final_modified;
    std::vector<worktree_file> final_deleted;

    std::set<std::string> idxFiles = idx->get_files();

    bool should_staged = false;
    bool should_not_staged = false;

    for(worktree_file& f : created) {
        auto it = idxFiles.find(f.path);
        if(it != idxFiles.end()) {
            int index = std::distance(idxFiles.begin(), it);
            auto it_entries = idx->get_entries().begin();
            std::advance(it_entries, index);
            bool staged = (*it_entries).obj_hash == f.hash;
            f.staged = staged;
            if(staged) {
                should_staged = true;
                final_created.push_back(f);
            } else {
                should_not_staged = true;
                final_modified.push_back(f);
            }
        }
        else {
            should_not_staged = true;
            final_created.push_back(f);
        }
    }

    for(worktree_file& f : modified) {
        auto it = idxFiles.find(f.path);
        int index = std::distance(idxFiles.begin(), it);
        auto it_entries = idx->get_entries().begin();
        std::advance(it_entries, index);
        f.staged = f.hash == (*it_entries).obj_hash;
        final_modified.push_back(f);
        should_staged = f.staged || should_staged;
        should_not_staged = !f.staged || should_not_staged;
    }

    std::set<std::string> tmpSet;
    for(auto f : idx->get_entries()) {
        tmpSet.insert(f.file_path);
    }

    for(auto& f : deleted) {
        if(tmpSet.find(f.path) == tmpSet.end()) {
            f.staged = true;
        }
        should_staged = f.staged || should_staged;
        should_not_staged = !f.staged || should_not_staged;
        final_deleted.push_back(f);
    }

    delete idx;

    if(should_staged) {
        std::cout << "Staged changes:\n";
        GREEN();
        print_changes("New file: ", created, true);
        print_changes("Modified: ", modified, true);
        print_changes("Deleted: ", deleted, true);
    }
    if(should_not_staged) {
        WHITE();
        std::cout << "Not staged changes:\n";
        RED();
        print_changes("New file: ", created, false);
        print_changes("Modified: ", modified, false);
        print_changes("Deleted: ", deleted, false);
    }


}

cli_execution_result status_func(std::vector<std::string> args) {
    cli_execution_result res;
    res.succeed = true;
    res.message = "";

    ref* r = ref::fetch_reference("HEAD");

    std::vector<worktree_file> created;
    std::vector<worktree_file> modified;
    std::vector<worktree_file> deleted;

    gCommit* commit = r->ref_to != "" ? dynamic_cast<gCommit*>(gObject::from_file(r->ref_to)) : nullptr;
    gTree* tree = r->ref_to != "" ? dynamic_cast<gTree*>(gObject::from_file(commit->get("tree"))) : nullptr;

    std::vector<std::string> current_worktree;
    walk(current_worktree, std::filesystem::current_path());

    std::set<std::string> current_files_set(current_worktree.begin(), current_worktree.end());
    std::vector<gTreeLeaf> leafs;
    if(tree != nullptr) {
        leafs = tree->get_leafs();
    }
    std::set<std::string> commit_tree_set;

    for(const gTreeLeaf leaf: leafs) {
        commit_tree_set.insert(leaf.path);
    }


    for(const gTreeLeaf leaf : leafs) {
        if(current_files_set.find(leaf.path) == current_files_set.end()) {
            deleted.push_back({
                .path = leaf.path,
                .hash = leaf.hash,
            });
        }
    }

    for(const std::string file : current_files_set) {
        auto it = commit_tree_set.find(file);
        int index = 0;
        if(tree != nullptr) {
            std::vector<gTreeLeaf> leafs = tree->get_leafs();
            index = std::distance(commit_tree_set.begin(), it);
        }

        if(it == commit_tree_set.end()) {
            created.push_back({
                .path=file,
                .hash=crypto::sha1OnVector(readFileToVector(file)),
            });
        } else if(tree != nullptr && crypto::sha1OnVector(readFileToVector(file)) != tree->get_leafs()[index].hash) {
            modified.push_back({
                   .path=file,
                   .hash=crypto::sha1OnVector(readFileToVector(file)),
           });
        }
    }

    delete tree;
    delete commit;

    handle_changes(created, modified, deleted);

    delete r;
    return res;
}

cli_execution_result add_func(std::vector<std::string> args) {
    repo* r = repo::find_repo();
    index_file* idx = index_file::load();
    for(const std::string arg : args) {
        idx->remove_entry(arg);
        std::vector<gObject*> objs;
        std::vector<std::string> pathes;
        if(std::filesystem::is_directory(arg)) {
            walk(pathes, arg);
        } else {
            pathes.push_back(arg);
        }
        gObject::create_objs_from_path(std::filesystem::current_path() / arg, objs);
        for(int i = 0; i < objs.size(); ++i) {
            gObject* obj = objs[i];
            std::string hash = obj->get_hash();
            std::string obj_full_path = std::filesystem::path(r->get_path("objects")) / hash.substr(0,2) / hash.substr(2);
            index_file_entry entry;
            entry.file_path = pathes[i];
            entry.obj_hash = hash;
            entry.obj_path = std::filesystem::relative(obj_full_path, std::filesystem::current_path());
            idx->add_entry(entry);
            obj->to_file();
            delete obj;
        }
    }
    idx->save();
    delete idx;
    delete r;
    cli_execution_result cer;
    cer.succeed = true;
    cer.message = "";
    return cer;
}

cli_execution_result commit_func(std::vector<std::string> args) {
    if(args.size() == 0) {
        cli_execution_result res;
        res.succeed = false;
        res.message = "Specify commit message, snit commit <message>\n";
        return res;
    }
    std::string message = args[0];
    cli_execution_result result;
    bool commited = gCommit::commit(message);
    result.succeed = true;
    result.message = commited ? "Changes commited!\n" : "Worktree is clean, nothing to commit\n";
    return result;
}

cli_execution_result log_func(std::vector<std::string> args) {
    cli_execution_result res;
    ref* r = ref::fetch_reference("HEAD");
    std::string reference = r->get_reference();
    if(reference == "") {
        res.message = "";
        res.succeed = true;
        return res;
    }
    gCommit* commit = dynamic_cast<gCommit*>(gObject::from_file(reference));
    print_commit_history(commit);
    delete r;

    res.message = "";
    res.succeed = true;
    return res;
}

cli_execution_result branch_func(std::vector<std::string>args) {
    std::vector<std::string> files;
    walk(files, std::filesystem::current_path() / GIT_EXTENSION / "refs" / "heads");
    repo* r = repo::find_repo();
    std::string active_branch = r->get_current_branch();
    delete r;
    for(int i = 0; i < files.size(); i ++) {
        std::string branch = std::filesystem::path(files[i]).filename().string();
        if(branch == active_branch) {
            std:: cout << "* ";
            GREEN();
        }
        std::cout << branch << "\n";
        WHITE();
    }

    cli_execution_result res;
    res.message = "";
    res.succeed = true;
    return res;
}

cli_execution_result checkout_func(std::vector<std::string> args) {
    cli_execution_result res;
    repo* r = repo::find_repo();
    if(args.size() == 2) {
        if(args[0] == "-b" || args[1] == "-b") {
            std::string branch_name;
            if(args[0] == "-b") {
                branch_name = args[1];
            } else {
                branch_name = args[0];
            }

            std::string path = std::filesystem::path(r->get_path("refs")) / "heads" / branch_name;
            if(std::filesystem::exists(path)) {
                res.message = "Branch '" + branch_name + "' already exists\n";
                res.succeed = false;
                delete r;
                return res;
            }
            r->create_repo_file(path);
            res.message = "Branch '" + branch_name + "' created\n";
            res.succeed = true;

            ref* tmp_ref = ref::fetch_reference("HEAD");
            std::string hash = tmp_ref->ref_to;
            delete tmp_ref;

            std::ofstream head_file(r->get_path("HEAD"));
            std::string path_of_ref = std::filesystem::relative(path, std::filesystem::current_path() / GIT_EXTENSION);
            head_file << "ref: " <<path_of_ref << "\n";
            head_file.close();

            std::ofstream branch_file(path);
            branch_file << hash << "\n";
            branch_file.close();
            delete r;
            return res;

        } else {
            res.message = "Use checkout like this: checkout <branch> OR checkout -b <branch>\n";
            res.succeed = false;
            delete r;
            return res;
        }
    } else {
        std::string branch_name = args[0];
        std::string path = std::filesystem::path(r->get_path("refs")) / "heads" / branch_name;
        if(!std::filesystem::exists(path)) {
            res.message = "Branch '" + branch_name + "' does not exists\n";
            res.succeed = false;
            delete r;
            return res;
        }

        std::vector<std::string> files;
        walk(files);

        for(std::string& file : files){
            std::filesystem::remove(file);
        }

        std::ofstream head_file(r->get_path("HEAD"));
        head_file << "ref: " << path << "\n";
        head_file.close();

        index_file* idx = new index_file;
        ref* re = ref::fetch_reference("HEAD");
        gTree* tree = dynamic_cast<gTree*>(gObject::from_file(re->ref_to));
        delete re;
        auto leafs = tree->get_leafs();
        for(const auto& leaf : leafs) {
            std::ofstream file(leaf.path);
            gBlob* blob = dynamic_cast<gBlob*>(gObject::from_file(leaf.hash));
            index_file_entry entry;
            entry.file_path = std::filesystem::relative(leaf.path, std::filesystem::current_path());
            entry.obj_hash = blob->get_hash();
            entry.obj_path = gObject::get_path_by_hash(entry.obj_hash);
            idx->add_entry(entry);
            auto dataVec = blob->serialize();
            file << std::string(dataVec.begin(), dataVec.end());
            file.close();
            delete blob;
        }
        idx->save();
        delete idx;
        delete tree;

        res.message = "Switched to branch '" + branch_name + "'\n";
        res.succeed = true;
    }
    delete r;
    return res;
}

namespace commands {
    callback_func init = init_func;
    callback_func status = status_func;
    callback_func add = add_func;
    callback_func commit = commit_func;
    callback_func log = log_func;
    callback_func branch = branch_func;
    callback_func checkout = checkout_func;
}