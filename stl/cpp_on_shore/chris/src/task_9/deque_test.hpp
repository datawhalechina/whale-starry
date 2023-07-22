#include <string>

#include "deque_class.hpp"

void IntSelfDequeTest() {
  MyDeque::deque<int> int_deque;
  for (int i = 0; i < 320; i++) {
    int_deque.push_back(i);
  }
  int_deque.push_front(10);
  std::cout << "End is :" << int_deque.back() << std::endl;
  std::cout << "Front is: " << int_deque.front() << std::endl;

  int_deque.pop_back();
  int_deque.pop_front();

  std::cout << "Now End is :" << int_deque.back() << std::endl;
  std::cout << "Now Front is: " << int_deque.front() << std::endl;

  for (int i = 0; i < int_deque.size(); i++) {
    if (i % 20 == 0) {
      std::cout << int_deque[i] << std::endl;
    }
  }

  int_deque.erase(int_deque.begin() + 10);
  std::cout << int_deque[10] << " deque's size is " << int_deque.size() << std::endl;

  int_deque.insert(int_deque.begin() + 10, 114514);
  std::cout << int_deque[10] << " deque's size is " << int_deque.size() << std::endl;
  std::cout << int_deque[11] << std::endl;
  /*
   Here we can see that my self-made deque can maintain the functions
   as the stl Deque do.
  */
}

void StrSelfDequeTest() {
  MyDeque::deque<std::string> str_deque;

  // memory access violation in MSVC, but reasonable in gcc.
  for (int i = 0; i < 10; i++) {
    str_deque.push_back_aux(" helloworld! ");
  }
  // memory access violation in MSVC, but reasonable in gcc.
  for (int i = 0; i < 10; i++) {
    str_deque.push_front_aux(" helloworld! ");
  }
  // can lead to a lot of mess codes.
  for (auto s : str_deque) {
    std::cout << s << std::endl;
  }
}