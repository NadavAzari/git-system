#ifndef MY_GIT_CLI_H
#define MY_GIT_CLI_H

#include <iostream>
#include <unordered_map>
#include <vector>

struct cli_execution_result {
    bool succeed;
    std::string message;
};

typedef cli_execution_result(*callback_func)(std::vector<std::string>);

class cli {
private:
    std::unordered_map<std::string, callback_func> callbacks;
public:
    void add_arg(std::string arg, callback_func callback);
    cli_execution_result execute(std::vector<std::string>);
};


#endif
