#include <algorithm>
#include <iostream>

#include "ivarnd_list.h"


int main() {
  ivarnd::list<int> m_list1;

  for (int i = 1; i <= 5; i++) {
    m_list1.push_back(i);
  }
  for (auto e : m_list1) {
    std::cout << e << " ";
  }
  std::cout << std::endl;

  return 0;
}
