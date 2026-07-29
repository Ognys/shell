#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <algorithm>
#include <filesystem>
#include <cstdlib>
#include <cstring>

#include "ShellCore.h"


int main() {

    std::string str;
    std::vector<std::string> str_split;

    ShellCore::CommandStatus status = ShellCore::CommandStatus::CONTINUE;

    const char* home = std::getenv("HOME");

    chdir(home);

    while(true) {
        std::string path = std::filesystem::current_path();
        auto new_end = std::remove(path.begin(), path.end(), '"');
        path.erase(new_end, path.end());
        path.replace(0, std::strlen(home), "~");

        std::cout << "shell:"  << path << "> " << "\033[35m";
        getline(std::cin, str);
        std::cout << "\033[0m" << std::flush;
        str_split = ShellCore::shell_pars(str);
        status = ShellCore::execute_command(str_split);

        if(status == ShellCore::CommandStatus::EXIT)
            break;
    }
}





