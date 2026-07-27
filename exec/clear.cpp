#include <iostream>

int main() {
    std::cout << "\x1B[H" << "\x1B[2J" << "\x1B[3J";
}