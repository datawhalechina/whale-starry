#include "my_test.h"

#include <gtest/gtest.h>

int main() {
  std::vector<Person> v;
  // 可以看到push的速度比emplace速度慢一些，但实际上在c++11以上没什么太大区别
  TestPushAndEmplaceTime(v);
  // 可以看到由于at要检查是否越界且底层调用[]，很明显速度不如[]直接索引来得快
  TestAboutAt(v);

  // 下面是自己手写的vector模板类测试
  MyVector<int> my_vec(2);
  my_vec.push_back(10);
  my_vec.push_back(20);
  my_vec.push_back(30);
  my_vec.pop_back();
  // 应当输出4
  std::cout << my_vec.Size() << std::endl;
  // 应当输出8
  std::cout << my_vec.Capacity() << std::endl;
  // 使用[]索引取值，输出应为10
  std::cout << my_vec[3] << std::endl;
  // 使用clear清空，输出应为0
  my_vec.clear();
  std::cout << my_vec.Size() << std::endl;
  std::cout << my_vec[0] << std::endl;
  return 0;
}
