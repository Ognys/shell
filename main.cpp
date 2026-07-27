#include <iostream>
#include <string>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <unistd.h>
#include <filesystem>

void execute_command(std::vector<std::string> args);

int main() {

    std::string str;
    std::vector<std::string> str_split;

    const char* home = std::getenv("HOME");

    chdir(home);

    while(true) {
        std::cout << "shell:" << std::filesystem::current_path() << "> ";
        getline(std::cin, str);
        boost::split(str_split, str, boost::is_any_of(" "));
        execute_command(str_split);
    }
}

void execute_command(std::vector<std::string> args) {
    if(args.empty())
        return;
    else if(args[0] == "cd")
    {
        chdir(args[1].c_str());
    }
}