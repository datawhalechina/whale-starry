#include <iostream>
#include <list>

int main() {
  std::list<int> mylist;
  mylist.push_back(1);
  mylist.push_back(2);
  mylist.push_back(3);
  mylist.push_front(0);
  for (auto it = mylist.begin(); it != mylist.end(); ++it) {
    std::cout << *it << " ";
  }
  std::cout << std::endl;
  mylist.remove(2);
  for (auto it = mylist.begin(); it != mylist.end(); ++it) {
    std::cout << *it << " ";
  }
  std::cout << std::endl;

  return 0;
}