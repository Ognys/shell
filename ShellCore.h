#pragma once

#include<vector>
#include<string>
#include <unordered_map>
#include <optional>
#include <unordered_set>



class ShellCore {

public:
    static std::vector<std::string> shell_pars(std::string str);
    static std::unordered_map<std::string, std::string> variables;


    enum class CommandStatus {
    CONTINUE,
    EXIT,
    ERROR,
    };

    static CommandStatus execute_command(std::vector<std::string> args);

private:
    static std::optional<std::filesystem::path> find_command(const std::string& command);
    inline static const std::unordered_set<std::string> redirect_ops = {">", ">>", "<", "2>", "2>>"};

    enum class quotes {
        NONE,
        DOUBLE_QUOTES,
        SINGLE_QUOTES
    };

    struct Redirection {
        std::string op;
        std::string path;
    };
};