#include <iostream>
#include <list>
#include <chrono>

void testPrefixDecrementPerformance(int dataSize, int iterations) {
  std::list<int> myList;

  // 添加测试数据
  for (int i = 0; i < dataSize; ++i) {
    myList.push_back(i);
  }

  // 测试前置递减性能
  auto startTime = std::chrono::high_resolution_clock::now();

  for (int iter = 0; iter < iterations; ++iter) {
    for (auto it = myList.begin(); it != myList.end(); ++it) {
      --(*it);
    }
  }

  auto endTime = std::chrono::high_resolution_clock::now();
  auto duration =
    std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();

  std::cout << "前置递减性能: " << duration << " us." << std::endl;
}

void testPostfixDecrementPerformance(int dataSize, int iterations) {
  std::list<int> myList;

  // 添加测试数据
  for (int i = 0; i < dataSize; ++i) {
    myList.push_back(i);
  }

  // 测试后置递减性能
  auto startTime = std::chrono::high_resolution_clock::now();

  for (int iter = 0; iter < iterations; ++iter) {
    for (auto it = myList.begin(); it != myList.end(); it++) {
      (*it)--;
    }
  }

  auto endTime = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();

  std::cout << "后置递减性能: " << duration << " us." << std::endl;
}

int main() {
  int dataSize = 1000;    // 测试数据规模
  int iterations = 1000;  // 重复运行次数

  testPrefixDecrementPerformance(dataSize, iterations);
  testPostfixDecrementPerformance(dataSize, iterations);

  return 0;
}
