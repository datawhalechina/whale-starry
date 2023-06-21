#include <iostream>
#include "tigerwe_list.h"

class Test {
 public:
  Test() { std::cout << "list construct.." << std::endl; }

  void method() { std::cout << "Test.." << std::endl; }

  ~Test() { std::cout << "list destruct.." << std::endl; }
};

void printfList(list<int>& list_INT) {
  list<int>::iterator itrBegin = list_INT.begin();

  while (itrBegin != list_INT.end()) {
    std::cout << *itrBegin;
    itrBegin++;
  }

  std::cout << std::endl;
}

int main(int argc, char* argv[]) {
  std::cout << "Test begin !" << std::endl;
  // test int
  list<int> list_INT;
  // 尾插
  list_INT.push_back(3);
  list_INT.push_back(1);
  list_INT.push_back(2);
  list_INT.push_back(4);
  list_INT.push_back(5);
  list_INT.push_back(2);

  printfList(list_INT);

  // 删除指定节点   这里返回的是删除的个数
  std::cout << "delete nums: " << list_INT.remove(2) << std::endl;
  std::cout << "delete nums: " << list_INT.remove(2) << std::endl;

  printfList(list_INT);

  list<int> list_INT1;
  // 头插
  list_INT1.push_front(1);
  list_INT1.push_front(2);
  list_INT1.push_front(3);
  list_INT1.push_front(4);
  list_INT1.push_front(5);

  printfList(list_INT1);
  // 访问
  std::cout << "front: " << list_INT1.front() << std::endl;

  std::cout << "back: " << list_INT1.back() << std::endl;

  // 删除
  list_INT1.pop_back();

  list_INT1.pop_front();

  std::cout << "size: " << list_INT1.size() << std::endl;

  printfList(list_INT1);

  // test class  主要看一下资源析构情况
  // 这里每个会调用两次构造函数
  Test test1;
  Test test2;
  Test test3;
  list<Test> list_CLASS;
  list_CLASS.push_back(test1);
  list_CLASS.push_back(test2);
  list_CLASS.push_back(test3);

  std::cout << list_CLASS.size() << std::endl;

  list_CLASS.clear();

  std::cout << list_CLASS.size() << std::endl;

  // test string
  list<std::string> list_STRING;

  list_STRING.push_back("vivwimp");
  list_STRING.push_back("always");
  list_STRING.push_back("miss");
  list_STRING.push_back("babysong");

  list<std::string>::iterator itBegin = list_STRING.begin();

  while (itBegin != list_STRING.end()) {
    std::cout << (*itBegin).c_str() << " ";
    itBegin++;
  }

  std::cout << std::endl;

  std::cout << "Test end !" << std::endl;
  return 0;
}
