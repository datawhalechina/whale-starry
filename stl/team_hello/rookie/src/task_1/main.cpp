#include <iostream>
#include <list>
#include <algorithm>

int main() {
    std::list<int> List;

    List.push_back(4);
    List.push_back(5);
    List.push_back(6);
    List.push_front(3);
    List.push_front(2);
    List.push_front(1);

    std::cout << "Init:" << std::endl;
    std::for_each(List.begin(), List.end(), [](int data){
        std::cout << data << " ";
    });
    std::cout << std::endl;

    List.insert(++List.begin(), 10);
    std::cout << "Insert:" << std::endl;
    std::for_each(List.begin(), List.end(), [](int data){
        std::cout  << data << " ";
    });
    std::cout << std::endl;

    std::cout << "Delete:" << std::endl;
    List.erase(std::find(List.begin(), List.end(), 4));
    std::for_each(List.begin(), List.end(), [](int data){
        std::cout << data << " ";
    });
    std::cout << std::endl;

    return 0;
}
