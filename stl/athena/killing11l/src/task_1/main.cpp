#include <functional>
#include <iostream>
#include <list>
#include <optional>

// 事件类型
using Event = std::function<void()>;

// 事件调度程序
class EventDispatcher {
 public:
  // 添加事件
  template <typename F>
  void AddEvent(F&& f) {
    events_.emplace_back(std::forward<F>(f));
  }

  // 处理事件
  void Dispatch() {
    while (!events_.empty()) {
      // 获取下一个事件
      auto event = std::move(events_.front());
      events_.pop_front();

      // 处理事件
      if (event) {
        std::invoke(*event);
      }
    }
  }

 private:
  std::list<std::optional<Event>> events_;
};

int main() {
  // 创建事件调度程序
  EventDispatcher dispatcher;

  // 添加多个事件
  int a = 1, b = 2;
  dispatcher.AddEvent([&a, &b]() { std::cout << "a + b = " << a + b << std::endl; });
  dispatcher.AddEvent([&a, &b]() { std::cout << "a * b = " << a * b << std::endl; });
  dispatcher.AddEvent([&a, &b]() { std::cout << "a - b = " << a - b << std::endl; });

  // 处理事件
  dispatcher.Dispatch();

  return 0;
}