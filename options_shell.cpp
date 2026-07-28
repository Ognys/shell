#include<vector>
#include<string>

#include "options_shell.h"

std::vector<std::string> options_shell::shell_pars(std::string str) {

    quotes state = quotes::NONE;
    std::vector<std::string> res;
    std::string buf = "";
    for(int i = 0; i < str.length(); i++)
    {

        if(state == quotes::NONE)
        {
            if(str[i] == ' ' && buf.length() != 0)
            {
                res.push_back(buf);
                buf = "";
                continue;
            }
            else if(str[i] == ' ')
                continue;
        }
        buf+= str[i];
    }

    if(buf.length() != 0)
        res.push_back(buf);

    return res;
}