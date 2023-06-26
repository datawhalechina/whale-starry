#include <chrono>
#include <ctime>
#include <iostream>
#include <list>
#include <mutex>
#include <thread>
#include <vector>

#include "my_vector.h"

class Timer {
 public:
  std::chrono::time_point<std::chrono::steady_clock> start, end;
  std::chrono::duration<float> last_time;

  // constructor
  Timer() { start = std::chrono::high_resolution_clock::now(); }
  // destructor
  ~Timer() {
    end = std::chrono::high_resolution_clock::now();
    last_time = end - start;
    float result_ms = last_time.count() * 1000.0f;
    std::cout << "Took " << result_ms << "ms" << std::endl;
  }
};

void test_emplace(std::vector<int> int_vector) {
  Timer timer;
  for (int i = 0; i < 10000000; i++) {
    int_vector.emplace_back(i);
  }
}

void test_push(std::vector<int> int_vector) {
  Timer timer;
  for (int i = 0; i < 10000000; i++) {
    int_vector.push_back(i);
  }
}

template <class T>
void test_push_method(T int_data) {
  Timer timer;
  for (int i = 0; i < 10000000; i++) {
    int_data.push_back(i);
  }
}

template <class T>
void test_emplace_method(T int_data) {
  Timer timer;
  for (int i = 0; i < 10000000; i++) {
    int_data.emplace_back(i);
  }
}

int main() {
  std::vector<int> push_vector;
  // use copy constructor to keep the same
  std::vector<int> emplace_vector(push_vector);
  test_push(push_vector);        // 470.52ms
  test_emplace(emplace_vector);  // 1447.66ms

  std::list<int> list_push_test;
  std::list<int> list_emplace_test(list_push_test);

  test_push_method(std::list<int>(list_push_test));        // use template func
  test_emplace_method(std::list<int>(list_emplace_test));  // use template func

  std::vector<int> vector_push_test;
  std::vector<int> vector_emplace_test(vector_push_test);

  // use template func
  test_push_method(std::vector<int>(vector_push_test));
  test_emplace_method(std::vector<int>(vector_emplace_test));
  std::cout << " Let's look at emplace & push, which is faster" << std::endl;
  std::cout << " And vector.emplace_back is faster." << std::endl;

  std::cout << "Let's see my own vector!" << std::endl;
  test();
  std::cout << "This is the end of vector!" << std::endl;
}