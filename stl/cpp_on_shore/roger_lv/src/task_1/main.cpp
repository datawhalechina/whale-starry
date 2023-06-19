#include <sysinfoapi.h>

#include <ctime>
#include <iostream>
#include <list>

using namespace std;

int main() {
  list<int> l1(2);  // 构造函数没传参，默认值为0
  cout << "constructor 1:  " << endl;
  // 容器取值
  for (auto iter = l1.begin(); iter != l1.end(); ++iter) {
    cout << *iter << " ";
  }
  cout << endl;
  list<int> l2;
  l2 = l1;  // 直接赋值传给另一个list
  auto it = l1.begin();
  *it = 10;
  auto it2 = l1.rend();  // rend==begin
  cout << *it2 << endl;

  list<int> l3(5, 2);
  unsigned long start = GetTickCount();
  l3.push_back(32131212312);
  cout << "test push_back_time: " << clock() - start << endl;
  unsigned long end = GetTickCount();
  l3.emplace_back(32131212312);
  cout << "test emplace_back_time: " << clock() - end << endl;
  // 测试方法感觉有问题测不出效率，今天太忙了没时间搞了，后面补上

  list<int> l4(5, 3);
  l4.swap(l3);  // 测试交换函数
  for (auto i : l3) {
    cout << i << " ";
  }
  cout << endl;
  list<int> l5(5);
  l5.assign(5, 3);  // 测试assign函数
  for (auto i : l5) {
    cout << i << " ";
  }
  cout << endl;

  list<int> l6(5);
  int i = 0;
  for (auto it = l6.begin(); it != l6.end(); ++it) {
    *it = i++;
  }
  for (auto it : l6) {
    cout << it << " ";
  }
  cout << endl;
  list<int> l7(5);
  for (auto it = l7.begin(); it != l7.end(); ++it) {
    *it = i++;
  }
  l7.merge(l6);  // 把l7合并到l6
  for (auto it : l7) {
    cout << it << " ";
  }
  return 0;
}

// 今天只是简单地测试了下接口，很水，实在是没时间学习