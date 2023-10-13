#include "cli_command.h"

cli_execution_result init_func(std::vector<std::string> args) {
    std::filesystem::path path = std::filesystem::current_path();
    cli_execution_result res;
    if(args.size() != 0) {
        path /= args[0];
    }
    if(std::filesystem::exists(path / std::filesystem::path(GIT_EXTENSION))) {
        res.message = "Error: dir is already a repo";
        res.succeed = false;
        return res;
    }
    repo* r =  repo::create_repo(path);
    delete r;
    res.message = "Initialized empty repository";
    res.succeed = true;

    return res;
}

namespace commands {
    callback_func init = init_func;
}