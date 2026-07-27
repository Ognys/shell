#include <iostream>
#include <string>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <unistd.h>
#include <filesystem>
#include <cstdlib>
#include <optional>
#include <sys/wait.h>



enum class status {
    CONTINUE,
    EXIT,
};

status execute_command(std::vector<std::string> args);
std::optional<std::filesystem::path> find_command(const std::string& command);

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
        boost::split(str_split, str, boost::is_any_of(" "), boost::token_compress_on);
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
    else
    {
        auto fc = find_command(args[0]);
        pid_t pid = fork();
        std::vector<char*> argv;
        for (std::string& arg : args) {
    argv.push_back(arg.data());
    }

    argv.push_back(nullptr);
    int status;
    if(pid == 0)
        execv(fc->string().c_str(), argv.data());
    else if(pid > 0)
    {
        waitpid(pid, &status, 0);
    }

    
    return status::CONTINUE;
    }

    return status::CONTINUE;
}

std::optional<std::filesystem::path> find_command(const std::string& command) {
    std::vector<char> buffer(4096);
    auto length = readlink("/proc/self/exe", buffer.data(), buffer.size());
    std::filesystem::path executable_path(buffer.data(), buffer.data() + length);
    std::filesystem::path command_path = executable_path.parent_path() / "exec" / command;
    if(!std::filesystem::exists(executable_path))
        return std::nullopt;

    return command_path;
}

