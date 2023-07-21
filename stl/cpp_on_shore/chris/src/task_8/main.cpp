#include "deque_impl.hpp"

int main() {
  Experiment1();

  // 对象que的作用域存在于整个main函数
  TestDeque que;  // Experiment 2
  /*
   for deque, push_back() cost more time
   than push_front()
  */

  std::cout << "Hello deque!" << std::endl;
  // here the destructor applies
}