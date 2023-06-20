#include <iostream>
#include "stl_list.h"

class Test {
 public:
  Test() { std::cout << "construct.." << std::endl; }

  void method() { std::cout << "welcome Test.." << std::endl; }

  ~Test() { std::cout << "destruct.." << std::endl; }
};

void printfList(Yami::list<int>& list_INT) {
  Yami::list<int>::list_iterator itrBegin = list_INT.begin();

  while (itrBegin != list_INT.end()) {
    std::cout << *itrBegin;
    itrBegin++;
  }

  std::cout << std::endl;
}

int main(int argc, char* argv[]) {
  std::cout << "Test bdgin !" << std::endl;
  // test int
  Yami::list<int> list_INT;
  list_INT.push_back(1);
  list_INT.push_back(2);
  list_INT.push_back(3);
  list_INT.push_back(4);
  list_INT.push_back(5);
  list_INT.push_back(2);

  printfList(list_INT);

  std::cout << "delete nums: " << list_INT.remove(2) << std::endl;

  printfList(list_INT);

  Yami::list<int> list_INT1;
  list_INT1.push_front(1);
  list_INT1.push_front(2);
  list_INT1.push_front(3);
  list_INT1.push_front(4);
  list_INT1.push_front(5);

  printfList(list_INT1);

  std::cout << "front: " << list_INT1.front() << std::endl;

  std::cout << "back: " << list_INT1.back() << std::endl;

  list_INT1.pop_back();

  list_INT1.pop_front();

  std::cout << "size: " << list_INT1.size() << std::endl;

  printfList(list_INT1);

  // test class  主要看一下资源析构情况
  Test test1;
  Test test2;
  Test test3;
  Yami::list<Test> list_CLASS;
  list_CLASS.push_back(test1);
  list_CLASS.push_back(test2);
  list_CLASS.push_back(test3);

  std::cout << list_CLASS.size() << std::endl;

  list_CLASS.clear();

  std::cout << list_CLASS.size() << std::endl;

  // test string
  Yami::list<std::string> list_STRING;

  list_STRING.push_back("nihao");
  list_STRING.push_back("thanks");
  list_STRING.push_back("goodbye");
  list_STRING.push_back("seeyou");

  Yami::list<std::string>::list_iterator itBegin = list_STRING.begin();

  while (itBegin != list_STRING.end()) {
    std::cout << " " << (*itBegin).c_str();
    itBegin++;
  }

  std::cout << std::endl;

  std::cout << "Test end !" << std::endl;
  return 0;
}
