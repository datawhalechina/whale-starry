#include <deque>
#include <iostream>

class MyClass {
 public:
  // 构造函数
  MyClass(int val) : value(val) {}

 public:
  int value;
};

int main() {
  std::deque<MyClass> myDeque;

  // 在队尾插入元素
  myDeque.push_back(MyClass(10));
  myDeque.push_back(MyClass(20));
  myDeque.push_back(MyClass(30));

  // 在队头插入元素
  myDeque.push_front(MyClass(5));
  myDeque.push_front(MyClass(2));

  // 输出deque的元素
  std::cout << "Deque elements: ";
  for (const auto& element : myDeque) {
    std::cout << element.value << " ";
  }
  std::cout << std::endl;

  // 访问deque的第三个元素
  std::cout << "Third element value: " << myDeque[2].value << std::endl;

  // 修改第一个元素的值
  myDeque[0].value = 1;

  // 使用迭代器遍历deque并输出元素
  std::cout << "Deque elements (iterator): ";
  for (auto it = myDeque.begin(); it != myDeque.end(); ++it) {
    std::cout << it->value << " ";
  }
  std::cout << std::endl;

  // 删除队头和队尾元素
  myDeque.pop_front();
  myDeque.pop_back();

  // 在指定位置插入元素
  auto insertIt = myDeque.insert(myDeque.begin() + 2, MyClass(15));

  // 删除指定位置的元素
  auto eraseIt = myDeque.erase(myDeque.begin() + 1);

  // 输出修改后的deque
  std::cout << "Modified deque elements: ";
  for (const auto& element : myDeque) {
    std::cout << element.value << " ";
  }
  std::cout << std::endl;

  // 输出被插入和删除元素后的迭代器位置
  std::cout << "Insert iterator position: " << insertIt->value << std::endl;
  std::cout << "Erase iterator position: " << eraseIt->value << std::endl;

  // 清空deque
  myDeque.clear();

  // 检查deque是否为空
  if (myDeque.empty()) {
    std::cout << "Deque is empty." << std::endl;
  } else {
    std::cout << "Deque is not empty." << std::endl;
  }

  return 0;
}
