#include <iostream>
#include "mlist.h"

template <typename _Tp>
void listpr(List<_Tp>& list) {
  for (List_iterator<int> it = list.begin(); it != list.end(); ++it) {
    std::cout << (*it) << ' ';
  }
  std::cout << std::endl;
}
int main() {
  std::cout << "--------init---------" << std::endl;
  List<int> list1;
  list1.push_back(1);
  list1.push_back(2);
  list1.push_back(3);
  list1.push_front(4);
  list1.push_front(5);
  list1.push_front(6);

  listpr(list1);

  std::cout << "-------test copylist and pop----------" << std::endl;
  // 测试拷贝构造函数和pop_
  List<int> list2(list1.begin(), list1.end());
  list2.pop_front();
  list2.pop_front();
  list2.pop_back();

  listpr(list2);

  std::cout << "-------test addlist----------" << std::endl;

  list1.insert(list1.begin(), list2.begin(), list2.end());
  list1.insert(list1.end(), list2.begin(), list2.end());

  listpr(list1);

  std::cout << "------- other ----------" << std::endl;

  return 0;
}