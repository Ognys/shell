#pragma once


#include<vector>
#include<string>
#include <unordered_map>



class ShellCore {

public:
    static std::vector<std::string> shell_pars(std::string str);
    static std::unordered_map<std::string, std::string> variables;

    enum class CommandStatus {
    CONTINUE,
    EXIT,
    };

private:
    enum class quotes {
        NONE,
        DOUBLE_QUOTES,
        SINGLE_QUOTES
    };
};