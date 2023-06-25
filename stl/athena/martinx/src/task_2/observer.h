#ifndef OBSERVER_H
#define OBSERVER_H
#include <vector>

class Subject;

class Observer {
 public:
  Observer(Subject* subject);
  void removeObserver();
  void notify();

 protected:
  Subject& subject_;

  // 演示用
  //  参考代码：https://refactoringguru.cn/design-patterns/observer/cpp/example
  int number_;
  static int static_number_;
};

class Subject {
 public:
  void registerObserver(Observer* o);
  void removeObserver(Observer* o);
  void notifyObservers();

  //  演示用
  void setState(int state);
  int getState();

 protected:
  std::vector<Observer*> observers_;
  int state_ = 0;
};
#endif