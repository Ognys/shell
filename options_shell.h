#pragma once


#include<vector>
#include<string>

    enum class quotes {
        NONE,
        DOUBLE_QUOTES,
        SINGLE_QUOTES
    };

class options_shell {
public:
    static std::vector<std::string> shell_pars(std::string str);
};