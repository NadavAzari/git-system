#ifndef MY_GIT_CLI_COMMAND_H
#define MY_GIT_CLI_COMMAND_H

#include <iostream>
#include <vector>
#include <filesystem>
#include <set>

#include "../repo/repo.h"

struct cli_execution_result {
    bool succeed;
    std::string message;
};

typedef cli_execution_result(*callback_func)(std::vector<std::string>);

namespace commands {
    extern callback_func init;
    extern callback_func status;
    extern callback_func add;
    extern callback_func commit;
    extern callback_func log;
}

#endif
