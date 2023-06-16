#include <iostream>
using namespace std;

int main() {
    const char* s = "Hello, world!";
    for (int i = 0; s[i]; ++i) {
        putchar(s[i]);
    }
    std::cout << std::endl;
    return 0;
}