#include <iostream>
#include <dirent.h>
#include <vector>
#include <filesystem>

int main(int argc, char* argv[]) {
    DIR* dir = opendir(std::filesystem::current_path().string().c_str());
    if(!dir)
        return 0;

    bool flag_a = argv[1] != nullptr ? std::string(argv[1]) == "-a" : false;

    std::vector<std::string> elements_dir;
    dirent* entry;

    while(entry = readdir(dir)) {
        elements_dir.push_back(entry->d_name);
    }



    for(auto v : elements_dir)
    {
        if(flag_a)
            std::cout<< v << std::endl;
        else if(v[0] != '.')
            std::cout<< v << std::endl;

    }
}