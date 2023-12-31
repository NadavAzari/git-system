#include <iostream>
#include "cli/cli.h"
#include "cli/cli_command.h"

std::vector<std::string> convert(int argc, char* argv[]) {
    std::vector<std::string> args;
    for (int i = 1; i < argc; ++i) {
        args.push_back(argv[i]);
    }
    return args;
}

int main(int argc, char* argv[]) {
    cli c;
    c.add_arg("init", commands::init);
    c.add_arg("status", commands::status);
    c.add_arg("add", commands::add);
    c.add_arg("commit", commands::commit);
    c.add_arg("log", commands::log);
    c.add_arg("branch", commands::branch);
    c.add_arg("checkout", commands::checkout);

    std::vector<std::string> args = convert(argc,argv);
    std::cout << c.execute(args).message;
}
