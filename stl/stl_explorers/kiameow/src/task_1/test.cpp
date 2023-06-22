#include "my_list.h"

#include <iostream>

int main() {
  MyListNode<int>* header = new MyListNode<int>;
  header->pos = 0;
  MyListNode<int>* p = header;
  for (int i = 0; i < 10; ++i) {
    MyListNode<int>* tmp = new MyListNode<int>;
    int value;
    std::cin >> value;
    tmp->value = value;
    tmp->pos = p->pos + 1;
    p->next = tmp;
    p = tmp;
  }

  MyListIterator<int> it(header->next);
  MyListIterator<int> it_end(p);
  for (; it < it_end; it += 2) {
    std::cout << *it << " ";
  }
  std::cout << std::endl;

  return 0;
}