#include <iostream>
#include <list>

template <class T>
void print_list(std::list<T>& li)
{
    for(auto iter = li.begin();iter != li.end();iter++) {
        std::cout << *iter << std::endl;
    }
}

int main() {

    std::list<int> l_i;
    // add item from head
    l_i.push_front(123);
    // add item from tail
    l_i.push_back(321);
    //print list
    print_list(l_i);
    
    return 0;
}