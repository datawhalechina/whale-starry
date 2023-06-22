#include <chrono>
#include <ctime>
#include <iostream>
#include <list>
#include <mutex>
#include <thread>
#include <vector>

#include "my_lru.h"

// define a timer by myself to check the efficiency between list.push_back
//&emplace_back
struct Timer {
  std::chrono::time_point<std::chrono::steady_clock> start, end;
  std::chrono::duration<float> last_time;
  Timer() { start = std::chrono::high_resolution_clock::now(); }
  ~Timer() {
    end = std::chrono::high_resolution_clock::now();
    last_time = end - start;
    float result_ms = last_time.count() * 1000.0f;
    std::cout << "Took " << result_ms << "ms" << std::endl;
  }
};

void cal_emplace_back(std::list<int> int_list) {
  Timer timer;
  for (int i = 0; i < 10000000; i++) {
    int_list.emplace_back(i);
  }
}

void cal_push_back(std::list<int> int_list) {
  Timer timer;
  for (int i = 0; i < 10000000; i++) {
    int_list.push_back(i);
  }
}

void lru_test() {
  lru<int,          // key type
      std::string,  // value type
      std::list<Node<int, std::string>>::iterator>
      a(5);  // iterator type
  a.setVal(1, "aaa");
  a.setVal(2, "bbbb");
  a.setVal(3, "ccccc");
  a.setVal(4, "dddddd");
  a.setVal(5, "eeeeeee");
  std::cout << a.getVal(1) << std::endl;
  a.setVal(6, "ffffffff");
  std::cout << a.getVal(2) << std::endl;

  std::cout << a.getVal(6) << std::endl;
}

int main() {
  std::clock_t t_begin1, t_end1, t_begin2, t_end2;
  std::list<char> say_hello;
  say_hello.push_back('a');
  say_hello.push_front('b');
  for (auto it = say_hello.cbegin(); it != say_hello.cend(); it++) {
    std::cout << *it << std::endl;
  }
  const std::list<char> try_str{'a', 'b', 'c'};
  /*To see the difference between iterator & const_iterator*/
  auto it1 = say_hello.begin();  //
  auto it1_c = say_hello.cbegin();
  it1_c++;
  std::cout << *it1_c << std::endl;
  auto it2_c = try_str.begin();
  auto it2 = try_str.cbegin();  // const_iterator for const std::list
  std::cout << say_hello.size() << std::endl;
  std::list<char> hello_copy(say_hello);
  std::list<int> integer_1;
  std::list<int> integer_2(integer_1);

  /*list push_back & emplace_back cost the same time!*/
  /*We can easily see that*/
  t_begin1 = clock();
  for (int i = 0; i < 10000000; i++) {
    integer_1.push_back(i);  // 8509ms 1658.95ms on My Mac
  }
  t_end1 = clock();
  std::cout << double(t_end1 - t_begin1) / CLOCKS_PER_SEC * 1000 << "ms" << std::endl;

  t_begin2 = clock();
  for (int i = 0; i < 10000000; i++) {
    integer_1.emplace_back(i);  // 8509ms  1658.95ms on My Mac
  }
  t_end2 = clock();
  std::cout << double(t_end1 - t_begin1) / CLOCKS_PER_SEC * 1000 << "ms" << std::endl;

  /*use the timer define by ourselves to compare*/
  std::list<int> test_timer;
  // use copy constructor to keep the same
  std::list<int> test_copy(test_timer);

  /*It seems that the effciency for emplace_back is a little faster*/
  /*Why this may happen?*/
  /*It may because I used chrono.h */
  cal_push_back(test_timer);  // 1572.24ms
  // use copy constructor to keep the same
  cal_emplace_back(test_copy);  // 1447.66ms
  std::cout << "This two ways for adding elements is kind of different.";
  std::cout << " And list.emplace_back method is kind of faster." << std::endl;
  // std::cout << "Happy List Ending!" << std::endl;
  lru_test();
}
