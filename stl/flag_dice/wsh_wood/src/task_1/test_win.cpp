#include <chrono>
#include <iostream>
#include <list>

int main() {
  for (int i = 0; i < 10; ++i) {
    std::list<int> myList;

    // Add 1000000 elements to the list
    for (int i = 0; i < 1000000; ++i) {
      myList.push_back(i);
    }

    // Test the performance of pre decrement
    auto startTime = std::chrono::high_resolution_clock::now();

    for (auto it = myList.begin(); it != myList.end(); ++it) {
      --(*it);
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);
    std::cout << "prefix time:" << duration.count() << " ns" << std::endl;

    // Test the performance of post decreasing
    startTime = std::chrono::high_resolution_clock::now();

    for (auto it = myList.begin(); it != myList.end(); it++) {
      (*it)--;
    }

    endTime = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);
    std::cout << "postfix time:" << duration.count() << " ns" << std::endl;
  }

  return 0;
}
