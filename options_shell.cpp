#include<vector>
#include<string>
#include <stdexcept>

#include "options_shell.h"

std::vector<std::string> options_shell::shell_pars(std::string str) {

    quotes state = quotes::NONE;
    std::vector<std::string> res;
    std::string buf = "";
    for(int i = 0; i < str.length(); i++)
    {

        if(state == quotes::NONE)
        {
            if(str[i] == '"')
            {
                state = quotes::DOUBLE_QUOTES;
                continue;
            }
            else if(str[i] == '\'')
            {
                state = quotes::SINGLE_QUOTES;
                continue;
            }

            if(str[i] == ' ' && buf.length() != 0)
            {
                res.push_back(buf);
                buf = "";
                continue;
            }
            else if(str[i] == ' ')
                continue;
        
            buf+= str[i];
        }
        else if(state == quotes::DOUBLE_QUOTES)
        {
            if(str[i] == '"')
            {
                state = quotes::NONE;
                continue;
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

    if(buf.length() != 0)
        res.push_back(buf);



    return res;
}