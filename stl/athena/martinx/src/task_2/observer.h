#ifndef OBSERVER_H
#define OBSERVER_H
#include <vector>

class Subject;

class Observer : public std::enable_shared_from_this<Observer> {
 public:
  Observer();
  ~Observer();

  void notify();
  std::shared_ptr<Observer> shared_observer();

 protected:
  // 演示用
  //  参考代码：https://refactoringguru.cn/design-patterns/observer/cpp/example

  int number_;
  static int static_number_;
};

class Subject {
 public:
  Subject();
  ~Subject();

  void registerObserver(std::shared_ptr<Observer> o);
  void unregisterObserver(std::shared_ptr<Observer> o);
  void notifyObservers();

  //  演示用
  void setState(int state);
  int getState();

 protected:
  std::vector<std::shared_ptr<Observer>> observers_;
  int state_ = 0;
};
#endif