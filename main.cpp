#include <iostream>
#include <string>

int main() {

    std::string str;

    while(true) {
        std::cout << "shell> ";
        getline(std::cin, str);
        std::cout << str << std::endl;
    }
}