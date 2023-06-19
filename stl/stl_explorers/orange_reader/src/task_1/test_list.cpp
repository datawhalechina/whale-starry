#include <algorithm>
#include <iostream>
#include <list>

int main() {
  // 创建含整数的 list
  std::list<int> l = {7, 5, 16, 8};

  // 添加整数到 list 开头
  l.push_front(25);
  // 添加整数到 list 结尾
  l.push_back(13);

  // 以搜索插入 16 前的值
  auto it = std::find(l.begin(), l.end(), 16);
  if (it != l.end())
    l.insert(it, 42);

  // 迭代并打印 list 的值
  std::cout << "l = { ";
  for (int n : l)
    std::cout << n << ", ";
  std::cout << "};" << std::endl;

  return 0;
}