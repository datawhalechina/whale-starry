#include <iostream>
#include "mlist.h"

template <typename T>
void print(const T& value) {
  std::cout << value << " ";
}

int main() {
  List<int> list1;
  list1.push_back(1);
  list1.push_back(2);
  list1.push_back(3);
  list1.push_front(4);
  list1.push_front(5);
  list1.push_front(6);
  for (List_iterator<int> it = list1.begin(); it != list1.end(); ++it) {
    print(*it);
  }
  std::cout << std::endl;

  List<double> list2;
  list2.push_back(1.2);
  list2.push_back(2.5);
  list2.push_back(3.6);

  list2.erase(list2.begin());
  for (List_iterator<double> it = list2.begin(); it != list2.end(); ++it) {
    print(*it);
  }
  std::cout << std::endl;

  List<char> list3;
  list3.push_back('x');
  list3.push_back('h');
  list3.push_back('s');

  for (List_iterator<char> it = list3.begin(); it != list3.end(); ++it) {
    if (*it == 'x') {
      it = list3.insert(++it, 'u');
      it = list3.insert(++it, 'e');
    }
    if (*it == 'h') {
      it = list3.insert(++it, 'u');
      it = list3.insert(++it, 'a');
    }
  }
  list3.insert(list3.end(), 'u');
  for (List_iterator<char> it = list3.begin(); it != list3.end(); ++it) {
    print(*it);
  }
  std::cout << std::endl;
  std::cout << std::endl;

  return 0;
}