/*
 * @Author: modi
 * @Date: 2023-06-17 18:00:01
 * @LastEditors: modi
 * @LastEditTime: 2023-06-17 18:10:01
 * @Description: file content
 * @FilePath: \whale-starry\stl\team_test\jackmo\src\main.cpp
 */
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