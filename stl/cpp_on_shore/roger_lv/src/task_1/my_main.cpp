
#include "my_func.h"

int main() {
  std::list<int> my_list1(2);  // 构造函数没传参，默认值为0
  my_list1.push_back(10);
  my_list1.push_back(20);
  MyList my_list2(my_list1);

  std::cout << "constructor 1:  " << std::endl;
  // 容器取值
  for (auto iter = my_list1.begin(); iter != my_list1.end(); ++iter) {
    std::cout << *iter << " ";
  }
  std::cout << std::endl;
  TestPushAndEmplaceTime(my_list1);  // 明显能看出emplace_back运行速度更快

  // 测试自定义运算符重载[]，希望实现和vector一样的索引取值方式
  std::cout << "my [] output: " << my_list2[2] << std::endl;

  // 测试自定义输出运算符<<，希望能直接通过cout打印list
  std::cout << "my << output: " << my_list2 << std::endl;
  return 0;
}
