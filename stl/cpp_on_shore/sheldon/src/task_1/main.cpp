#include <chrono>
#include <iostream>
#include <list>

// 输出列表中的所有元素
void PrintList(const std::list<int>& mylist) {
  for (const auto& element : mylist) {
    std::cout << element << " ";
  }
  std::cout << std::endl;
}

// 测试push_back的性能
void TestPushBack(int num) {
  std::list<int> myList;
  auto start = std::chrono::high_resolution_clock::now();

  for (int i = 0; i < num; ++i) {
    myList.push_back(i);
  }

  auto end = std::chrono::high_resolution_clock::now();
  auto temp1 = end - start;
  auto temp = std::chrono::duration_cast<std::chrono::microseconds>(temp1);
  auto duration = temp.count();
  std::cout << "push_back performance: " << std::endl;
  std::cout << "inserting duration: " << duration << " us" << std::endl;
}

// 测试emplace_back的性能
void TestEmplaceBack(int numIterations) {
  std::list<int> myList;
  auto start = std::chrono::high_resolution_clock::now();

  for (int i = 0; i < numIterations; ++i) {
    myList.emplace_back(i);
  }

  auto end = std::chrono::high_resolution_clock::now();
  auto temp1 = end - start;
  auto temp = std::chrono::duration_cast<std::chrono::microseconds>(temp1);
  auto duration = temp.count();
  std::cout << "emplace_back performance: " << std::endl;
  std::cout << "inserting duration: " << duration << " us" << std::endl;
}

int main() {
  // 创建一个空的列表
  std::list<int> mylist;

  // 在列表末尾插入元素
  mylist.push_back(100);
  mylist.push_back(25);
  mylist.push_back(300);

  // 在列表开头插入元素
  mylist.push_front(500);

  // 输出列表中的所有元素
  std::cout << "List init:";
  PrintList(mylist);

  // 排序
  mylist.sort();

  // 输出列表中的所有元素
  std::cout << "List, after sort:";
  PrintList(mylist);

  // 移除列表末尾的元素
  mylist.pop_back();

  // 移除列表开头的元素
  mylist.pop_front();

  // 输出列表中的所有元素
  std::cout << "List, after pop_back and pop_front:";
  PrintList(mylist);

  // 在指定位置插入元素
  auto it = ++mylist.begin();  // 获取第二个位置的迭代器
  mylist.insert(it, 115);

  // 输出列表中的所有元素
  std::cout << "List, after insert 15 at the second place:";
  PrintList(mylist);

  // 移除指定位置的元素
  it = mylist.begin();  // 获取第一个位置的迭代器
  mylist.erase(it);

  // 输出列表中的所有元素
  std::cout << "List, after erase the first element:";
  PrintList(mylist);

  // 清空列表
  mylist.clear();

  // 输出列表中的所有元素
  std::cout << "List, after clear:";
  PrintList(mylist);

  // 测试
  int numIterations = 1000000;

  // push_back() 114235 us 109961 us 130216 us
  TestPushBack(numIterations);

  // emplace_back() 94060 us 98405 us 103185 us, emplace_back() 更快.
  TestEmplaceBack(numIterations);

  return 0;
}
