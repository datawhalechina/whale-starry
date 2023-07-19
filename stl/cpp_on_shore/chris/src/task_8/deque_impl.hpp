#include <chrono>
#include <deque>
#include <iostream>
#include <vector>

class Timer {
 public:
  std::chrono::time_point<std::chrono::steady_clock> start, end;
  std::chrono::duration<float> last_time;

  // constructor for time calculation
  Timer() { start = std::chrono::high_resolution_clock::now(); }
  // destructor for time calculation
  ~Timer() {
    end = std::chrono::high_resolution_clock::now();
    last_time = end - start;
    float result_ms = last_time.count() * 1000.0f;
    std::cout << "This action took " << result_ms << "ms" << std::endl;
  }
};

template <typename T>
void Print(T data) {
  for (auto dt : data) {
    std::cout << dt << std::endl;
  }
  std::cout << "****************************Print Finished!*************************"
            << std::endl;
}

/*
 Experiment 1: 测试vector和deque push_back, emplace_back, push_front,
 emplace_front 性能(后两个仅有deque有)

 Experiment 2: 编写一个类 测试deque的push_front和push_back的性能差异

*/

template <typename T>
void CalPushBack(T data_queue) {
  Timer timer;
  for (int i = 0; i < 10000000; i++) data_queue.push_back(i);
  // Print(data_queue);
}

template <typename T>
void CalEmplaceBack(T data_queue) {
  Timer timer;
  for (int i = 0; i < 10000000; i++) data_queue.emplace_back(i);
}
void Experiment1() {
  std::vector<int> test_vector;
  std::deque<int> test_deque;

  std::cout << "First we test vector push_back() " << std::endl;
  CalPushBack(test_vector);
  std::cout << "First we test deque push_back() " << std::endl;
  CalPushBack(test_deque);

  std::cout << "First we test vector emplace_back() " << std::endl;
  CalEmplaceBack(test_vector);
  std::cout << "First we test queue emplace_back() " << std::endl;
  CalEmplaceBack(test_deque);
}

class TestDeque : std::deque<int> {
 public:
  TestDeque() {
    Timer timer;
    std::deque<int> que;
    // TestDeque que;
    for (int i = 0; i < 10000000; i++) que.push_back(i);
    std::cout << "This constructor is to cal the time for push_back()." << std::endl;
  }
  ~TestDeque() {
    Timer timer;
    std::deque<int> que;
    // TestDeque que;
    for (int i = 0; i < 10000000; i++) que.push_front(i);
    std::cout << "This destructor is to cal the time for push_front()." << std::endl;
  }
};
