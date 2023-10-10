#ifndef MY_GIT_CLI_COMMAND_H
#define MY_GIT_CLI_COMMAND_H

#include <iostream>
#include <vector>
#include <filesystem>

#include "../repo/repo.h"

struct cli_execution_result {
    bool succeed;
    std::string message;
};

typedef cli_execution_result(*callback_func)(std::vector<std::string>);

namespace commands {
    extern callback_func init;
}

#endif
