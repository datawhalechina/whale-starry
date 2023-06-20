#include "myList.h"

#include <iostream>

int main() {
  myList_node<int>* header = new myList_node<int>;
  header->pos = 0;
  myList_node<int>* p = header;
  for (int i = 0; i < 10; ++i) {
    myList_node<int>* tmp = new myList_node<int>;
    int value;
    std::cin >> value;
    tmp->value = value;
    tmp->pos = p->pos + 1;
    p->next = tmp;
    p = tmp;
  }

  myList_iterator<int> it(header->next);
  myList_iterator<int> it_end(p);
  for (; it < it_end; it += 2) {
    std::cout << *it << " ";
  }
  std::cout << std::endl;

  return 0;
}