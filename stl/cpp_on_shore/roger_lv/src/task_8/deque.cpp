#include <iostream>
#include <stdexcept>

// 这里是简易版自定义deque，当然是无法跟原来stl设计相匹配，只是尝试实现下功能
template <typename T>
class Deque {
 private:
  struct Node {
    T data;
    Node* prev;
    Node* next;
  };

  Node* front;   // 前端元素指针
  Node* back;    // 后端元素指针
  size_t count;  // 元素个数

 public:
  // 构造函数
  Deque() : front(nullptr), back(nullptr), count(0) {}

  // 析构函数
  ~Deque() {
    while (!empty()) {
      pop_front();
    }
  }

  // 返回d元素的个数
  size_t size() const { return count; }

  // 检查是否为空
  bool empty() const { return count == 0; }

  // 头插
  void push_front(const T& value) {
    Node* newNode = new Node{value, nullptr, front};
    if (empty()) {
      back = newNode;
    } else {
      front->prev = newNode;
    }
    front = newNode;
    count++;
  }

  // 尾插
  void push_back(const T& value) {
    Node* newNode = new Node{value, back, nullptr};
    if (empty()) {
      front = newNode;
    } else {
      back->next = newNode;
    }
    back = newNode;
    count++;
  }

  // 头删
  void pop_front() {
    if (empty()) {
      throw std::out_of_range("Deque is empty");
    }
    Node* oldFront = front;
    front = front->next;
    if (front == nullptr) {
      back = nullptr;
    } else {
      front->prev = nullptr;
    }
    delete oldFront;
    count--;
  }

  // 尾删
  void pop_back() {
    if (empty()) {
      throw std::out_of_range("Deque is empty");
    }
    Node* oldBack = back;
    back = back->prev;
    if (back == nullptr) {
      front = nullptr;
    } else {
      back->next = nullptr;
    }
    delete oldBack;
    count--;
  }

  // 返回deque的前端元素的引用
  T& get_front() {
    if (empty()) {
      throw std::out_of_range("Deque is empty");
    }
    return front->data;
  }

  // 返回deque的后端元素的引用
  T& get_back() {
    if (empty()) {
      throw std::out_of_range("Deque is empty");
    }
    return back->data;
  }
};

int main() {
  Deque<int> deque;
  deque.push_back(1);
  deque.push_front(2);
  deque.push_back(3);

  std::cout << "Front: " << deque.get_front() << std::endl;
  std::cout << "Back: " << deque.get_back() << std::endl;

  deque.pop_front();
  deque.pop_back();

  std::cout << "Size: " << deque.size() << std::endl;
  std::cout << "Empty: " << (deque.empty() ? "true" : "false") << std::endl;

  return 0;
}
