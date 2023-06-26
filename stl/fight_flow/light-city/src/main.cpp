#include <iostream>
#include <vector>
#include <string>

int main() {
    std::vector<char> data;

    std::string str = "Hello, World!";
    for (char c : str) {
        data.push_back(c);
    }

    for (char c : data) {
        std::cout << c;
    }
    std::cout << std::endl;

    return 0;
}
