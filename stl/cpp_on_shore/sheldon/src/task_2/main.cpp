#include <chrono>
#include <iostream>
#include <vector>

// 输出列表中的所有元素
void PrintVector(const std::vector<int>& myvector) {
  for (const auto& element : myvector) {
    std::cout << element << " ";
  }
  std::cout << std::endl;
}

int main() {
  // 创建一个vector
  std::vector<int> vec_0;
  std::vector<int> vec_1(5, 1);
  std::vector<int> vec_2(vec_1);

  std::cout << "vec_0:";
  PrintVector(vec_0);
  std::cout << "vec_1:";
  PrintVector(vec_1);
  std::cout << "vec_2:";
  PrintVector(vec_2);

  // 测试 assign
  vec_0.assign(10, 1);
  std::cout << "vec_0 after assign:";
  PrintVector(vec_0);
  vec_1.assign(15, 2);
  std::cout << "vec_1 after assign:";
  PrintVector(vec_1);  // 会直接改变原来vector
  vec_2.assign(vec_1.begin(), vec_1.begin() + 3);
  std::cout << "vec_2 after assign:";
  PrintVector(vec_2);  // 会直接改变原来vector

  // 测试size相关
  std::cout << "size of vec_1: " << vec_1.size() << std::endl;
  std::cout << "max size of vec_1: " << vec_1.max_size() << std::endl;
  std::cout << "capacity of vec_1: " << vec_1.capacity() << std::endl;
  vec_1.reserve(25);
  std::cout << "capacity of vec_1: " << vec_1.capacity() << std::endl;
  vec_1.resize(20);
  std::cout << "vec_1 after resize:";
  PrintVector(vec_1);

  // 测试emplace, insert, erase
  int val = 100;
  vec_1.emplace(vec_1.begin() + 2, val);
  PrintVector(vec_1);
  vec_1.insert(vec_1.begin() + 2, 5, val);
  PrintVector(vec_1);
  vec_1.erase(vec_1.begin() + 2);
  PrintVector(vec_1);
  vec_1.erase(vec_1.begin(), vec_1.begin() + 2);  // 注意左闭右开
  PrintVector(vec_1);

  // 测试扩容机制
  std::vector<int> vec_4;
  std::cout << "capacity of vec_4: " << vec_4.capacity() << std::endl;
  PrintVector(vec_4);
  vec_4.push_back(1);
  vec_4.push_back(1);
  std::cout << "capacity of vec_4: " << vec_4.capacity() << std::endl;
  PrintVector(vec_4);
  vec_4.insert(vec_4.end(), 5, 2);
  std::cout << "capacity of vec_4: " << vec_4.capacity() << std::endl;
  PrintVector(vec_4);
  vec_4.insert(vec_4.end(), 1, 2);
  std::cout << "capacity of vec_4: " << vec_4.capacity() << std::endl;
  PrintVector(vec_4);  // 会以两倍的容量进行扩容
  for (int i = 0; i < 10; i++) {
    vec_4.insert(vec_4.end(), 1, i);
    std::cout << "capacity of vec_4: " << vec_4.capacity() << std::endl;
  }

  return 0;
}
