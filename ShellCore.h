#pragma once

#include<vector>
#include<string>
#include <unordered_map>
#include <optional>



class ShellCore {

public:
    static std::vector<std::string> shell_pars(std::string str);
    static std::unordered_map<std::string, std::string> variables;


    enum class CommandStatus {
    CONTINUE,
    EXIT,
    };

    static CommandStatus execute_command(std::vector<std::string> args);

private:
    static std::optional<std::filesystem::path> find_command(const std::string& command);

    enum class quotes {
        NONE,
        DOUBLE_QUOTES,
        SINGLE_QUOTES
    };
};