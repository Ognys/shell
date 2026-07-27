#include <iostream>
#include <dirent.h>
#include <vector>
#include <filesystem>

int main() {
    DIR* dir = opendir(std::filesystem::current_path().string().c_str());
    if(!dir)
        return 0;

    std::vector<std::string> elements_dir;
    dirent* entry;

    while(entry = readdir(dir)) {
        elements_dir.push_back(entry->d_name);
    }



    for(auto v : elements_dir)
    {
        std::cout<< v << std::endl;
    }
}