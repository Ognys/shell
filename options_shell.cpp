#include<vector>
#include<string>
#include <stdexcept>
#include <cctype>

#include "options_shell.h"

std::unordered_map<std::string, std::string> options_shell::variables{};

std::vector<std::string> options_shell::shell_pars(std::string str) {

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
