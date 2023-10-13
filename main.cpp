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

    std::vector<std::string> args = convert(argc,argv);
    std::cout << c.execute(args).message << "\n";
}
