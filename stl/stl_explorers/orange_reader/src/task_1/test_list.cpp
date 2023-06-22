#include "tiny_list.hpp"

#include <iostream>

template <typename T>
void print_list(tiny_stl::List<T>& l) {
  if (l.empty()) {
    std::cout << "list is empty!" << std::endl;
    return;
  }

  auto iter = l.begin();
  while (iter != l.end()) {
    std::cout << *iter << " ";
    ++iter;
  }
  std::cout << std::endl;
}

int main() {
  // 创建含整数的 list
  tiny_stl::List<int> l;

  // 添加整数到 list 开头
  for (int i = 4; i >= 0; --i) {
    l.push_front(i);
  }

  // 添加整数到 list 结尾
  for (int i = 5; i < 10; ++i) {
    l.push_back(i);
  }

  print_list(l);

  for (int i = 0; i < 5; i++) {
    l.pop_front();
    l.pop_back();
    print_list(l);
  }

  return 0;
}