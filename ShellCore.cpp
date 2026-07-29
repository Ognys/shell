#include <iostream>
#include <string>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <unistd.h>
#include <filesystem>
#include <cstdlib>
#include <optional>
#include <sys/wait.h>
#include <unordered_map>
#include <fcntl.h>   

#include "ShellCore.h"

std::unordered_map<std::string, std::string> ShellCore::variables{};

std::vector<std::string> ShellCore::shell_pars(std::string str) {

    quotes state = quotes::NONE;
    std::vector<std::string> res;
    std::string buf = "";
    bool argument_started = false;

    for(int i = 0; i < str.length(); i++)
    {

        if(state == quotes::NONE)
        {
            if(str[i] == '"')
            {
                state = quotes::DOUBLE_QUOTES;
                argument_started = true;
                continue;
            }
            else if(str[i] == '\'')
            {
                state = quotes::SINGLE_QUOTES;
                argument_started = true;
                continue;
            }

            if(str[i] == ' ' && argument_started)
            {

                res.push_back(buf);
                buf = "";
                argument_started = false;
                continue;
            }
            else if(str[i] == ' ')
            {
                argument_started = false;
                continue;
            }
            if(str[i] == '\\')
            {
                if((i + 1) < str.length())
                {
                i++;
                buf+= str[i];
                argument_started = true;
                continue;
                }

            }
        
            buf+= str[i];
            argument_started = true;
        }
        else if(state == quotes::DOUBLE_QUOTES)
        {
            if(str[i] == '"')
            {
                state = quotes::NONE;
                continue;
            }
            
            if(str[i] == '$')
            {
                int j = i + 1;
                std::string key = "";
                if(!((j < str.length()) && (std::isalpha(str[j]) || str[j] == '_')))
                {
                    buf += str[i];
                    continue;
                }
                key += str[j];
                j++;
                while((j < str.length()) && (std::isalnum(str[j]) || (str[j] == '_')))
                {
                    key += str[j];
                    j++;

                }
                auto fk = variables.find(key);

                if(fk != variables.end())
                    buf += fk->second;
                i = j - 1;
                continue;
            }

            if(str[i] == '\\')
            {
                if( ((i + 1) < str.length()) && (std::string("\\\"$").find(str[i + 1]) != std::string::npos))
                {
                    i++;
                    buf+= str[i];
                    continue;
                }
            }

            buf+= str[i];
        }
        else if(state == quotes::SINGLE_QUOTES)
        {
            if(str[i] == '\'')
            {
                state = quotes::NONE;
                continue;
            }        
            buf+= str[i];
        }
        
    }

        if(state != quotes::NONE)
         throw std::runtime_error("quotes not closed");

    if(argument_started)
        res.push_back(buf);



    return res;
}

std::optional<std::filesystem::path> ShellCore::find_command(const std::string& command) {

    const char* c_path = getenv("PATH");
    std::string str_path = c_path;
    std::vector<std::string> split_path;
    boost::split(split_path, str_path, boost::is_any_of(":"));
    for(int i =0;i < split_path.size();i++)
    {
        if(std::filesystem::exists(split_path[i] + "/" + command))
            return split_path[i] + "/" + command;  
    }

    //std::vector<char> buffer(4096);
    //auto length = readlink("/proc/self/exe", buffer.data(), buffer.size());
    //std::filesystem::path executable_path(buffer.data(), buffer.data() + length);
    //std::filesystem::path command_path = executable_path.parent_path() / "exec" / command;

    return std::nullopt;
}

ShellCore::CommandStatus ShellCore::execute_command(std::vector<std::string> args) {
    if(args.empty())
        return ShellCore::CommandStatus::CONTINUE;

    
    if(args.size() == 1 && args[0].find("=") != std::string::npos)
    {
        std::size_t pos = args[0].find("=");
        std::string key = args[0].substr(0, pos);
        std::string val = args[0].substr(pos + 1);
        ShellCore::variables[key] = val;
    }
    else if(args[0] == "cd")
    {
        if(args[1] != "~")
            chdir(args[1].c_str());
        else
            chdir(std::getenv("HOME"));
        return ShellCore::CommandStatus::CONTINUE;
    }
    else if(args[0] == "exit")
    {
        return ShellCore::CommandStatus::EXIT;
    }
    else if(args[0] == "export")
    {
        if(args[1].find("=") != std::string::npos)
        {
            std::size_t pos = args[0].find("=");
            std::string key = args[0].substr(0, pos);
            std::string val = args[0].substr(pos + 1);
            ShellCore::variables[key] = val;
            setenv(key.c_str(), val.c_str(), 1);
        }
        else{
            setenv(args[1].c_str(), ShellCore::variables[args[1]].c_str(), 1);
        }
    }
    else if(args[0] == "unset")
    {
        ShellCore::variables.erase(args[1]);
        unsetenv(args[1].c_str());
    }
    else
    {
        std::string file_path;
        std::string redirect_operator;
        for(int i = 0; i < args.size(); i++)
        {
            if(args[i] == ">")
            {
                file_path = args[i + 1];
                redirect_operator = args[i];
                args.erase(args.begin() + i, args.end());
                break;
            }
        }

        auto fc = find_command(args[0]);
        pid_t pid = fork();
        std::vector<char*> argv;
        for (std::string& arg : args) {
            argv.push_back(arg.data());
        }

        argv.push_back(nullptr);
        int status;
        if(pid == 0){
            
            int fd = open(file_path.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
            dup2(fd, STDOUT_FILENO);
            close(fd);
            execv(fc->string().c_str(), argv.data());
        }
        else if(pid > 0)
        {
            waitpid(pid, &status, 0);
        }

    
        return ShellCore::CommandStatus::CONTINUE;
    }

    return ShellCore::CommandStatus::CONTINUE;
}
