#include <iostream>
#include <string>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <unistd.h>
#include <filesystem>
#include <cstdlib>



enum class status {
    CONTINUE,
    EXIT,
};

status execute_command(std::vector<std::string> args);

int main() {

    std::string str;
    std::vector<std::string> str_split;

    status status = status::CONTINUE;

    const char* home = std::getenv("HOME");

    chdir(home);

    while(true) {
        std::string path = std::filesystem::current_path();
        auto new_end = std::remove(path.begin(), path.end(), '"');
        path.erase(new_end, path.end());
        path.replace(0, std::strlen(home), "~");

        std::cout << "shell:" << path << "> ";
        getline(std::cin, str);
        boost::split(str_split, str, boost::is_any_of(" "));
        status = execute_command(str_split);

        if(status == status::EXIT)
            break;
    }
}

status execute_command(std::vector<std::string> args) {
    if(args.empty())
        return status::CONTINUE;
    else if(args[0] == "cd")
    {
        if(args[1] != "~")
            chdir(args[1].c_str());
        else
            chdir(std::getenv("HOME"));
        return status::CONTINUE;
    }
    else if(args[0] == "exit")
    {
        return status::EXIT;
    }
    else if(args[0] == "ls")
        std::system("/home/zaharovya/project/shell/exec/ls");
    return status::CONTINUE;
}

