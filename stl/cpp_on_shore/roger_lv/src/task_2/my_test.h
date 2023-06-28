#include <chrono>
#include <iostream>
#include <vector>

class Person {
 private:
  int age = 24;
  std::string name = "Roger";

 public:
  Person(){};
  Person(int x, std::string n) {
    age = x + 14;
    name = n;
  }
  int GetAge() { return age; }
  std::string GetName() { return name; }
};

void TestPushAndEmplaceTime(std::vector<Person>& vc) {
  auto start = std::chrono::steady_clock::now();
  for (int i = 0; i < 100000; ++i) {
    vc.push_back(Person(10, "roger"));
  }
  auto end = std::chrono::steady_clock::now();
  std::cout << "Test the push time: "
            << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()
            << " ns" << std::endl;
  start = std::chrono::steady_clock::now();
  for (int i = 0; i < 100000; ++i) {
    vc.emplace_back(Person(10, "roger"));
  }
  end = std::chrono::steady_clock::now();
  std::cout << "Test the emplace time: "
            << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()
            << " ns" << std::endl;
}

void TestAboutAt(std::vector<Person>& vc) {
  auto start = std::chrono::steady_clock::now();
  for (int i = 0; i < 100000; ++i) {
    int temp = vc[10].GetAge();
  }
  auto end = std::chrono::steady_clock::now();
  std::cout << "Test the [] time: "
            << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()
            << " ns" << std::endl;
  start = std::chrono::steady_clock::now();
  for (int i = 0; i < 100000; ++i) {
    int temp = vc.at(10).GetAge();
  }
  end = std::chrono::steady_clock::now();
  std::cout << "Test the at time: "
            << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()
            << " ns" << std::endl;
}

// 手写一个简易版本的vector
template <typename T>
class MyVector {
 private:
  T* data;
  int size;
  int capacity;

 public:
  MyVector() {
    data = new T[10];
    size = 0;
    capacity = 10;
  }
  MyVector(int n) {
    data = new T[n];
    for (int i = 0; i < n; ++i) {
      data[i] = 0;
    }
    size = n;
    capacity = n;
  }
  MyVector(const MyVector& v) { *this = v; }
  ~MyVector() { delete[] data; }
  void push_back(const T& x) {
    if (size == capacity) {
      capacity *= 2;
      T* temp = new T[capacity];
      for (int i = 0; i < size; ++i) {
        temp[i] = data[i];
      }
      temp[++size] = x;
      data = temp;
      delete[] temp;
    } else {
      data[++size] = x;
    }
  }
  void pop_back() {
    if (size == 0) {
      std::cout << "The vector is empty!" << std::endl;
      return;
    }
    --size;
  }
  T& operator[](int index) noexcept {
    if (index < 0 || index >= size) {
      return data[0];
    }
    return data[index];
  }
  T Size() { return size; }
  T Capacity() { return capacity; }
  void clear() { size = 0; }
};