#include "cli.h"

void cli::add_arg(std::string arg, callback_func callback) {
    callbacks[arg] = callback;
}

cli_execution_result error_msg(std::string msg) {
    cli_execution_result res;
    res.succeed = false;
    res.message = msg;
    return res;
}

cli_execution_result cli::execute(std::vector<std::string> args) {
    if(args.size() == 0) {
        return error_msg("You must specify the command git <command> <args>");
    }
    if(callbacks.find(args[0]) == callbacks.end()) {
        return error_msg("'" + args[0] + "' is not a valid command.");
    }

    return callbacks[args[0]](std::vector<std::string>(args.begin() + 1, args.end()));
}
