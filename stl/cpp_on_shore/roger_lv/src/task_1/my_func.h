#include <chrono>
#include <iostream>
#include <list>

void TestPushAndEmplaceTime(std::list<int>& ll) {
  auto start = std::chrono::steady_clock::now();
  for (int i = 0; i < 100000; ++i) {
    ll.push_back(rand());
  }
  auto end = std::chrono::steady_clock::now();
  std::cout << "Test the push time: "
            << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()
            << " ns" << std::endl;
  start = std::chrono::steady_clock::now();
  for (int i = 0; i < 100000; ++i) {
    ll.emplace_back(rand());
  }
  end = std::chrono::steady_clock::now();
  std::cout << "Test the emplace time: "
            << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()
            << " ns" << std::endl;
}

// 简单的mylist，重载两个运算符
class MyList {
 private:
  std::list<int> my_list;

 public:
  MyList(std::list<int> x) { my_list = x; }

  //希望能实现直接通过[]索引取值
  auto operator[](int pos) {
    int temp = 0;
    for (auto l : this->my_list) {
      if (temp == pos) return l;
      ++temp;
    }
  }

  //希望能重载<<输出流，直接能通过cout打印list的值
  friend std::ostream& operator<<(std::ostream& os, const MyList& m_list) {
    for (auto it : m_list.my_list) {
      os << it << " ";
    }
    return os;
  }
};
