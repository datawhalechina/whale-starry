#include "observer.h"

int Observer::static_number_ = 0;

int main() {
  std::shared_ptr<Subject> subject = std::make_shared<Subject>();
  std::shared_ptr<Observer> observer1 = std::make_shared<Observer>();
  std::shared_ptr<Observer> observer2 = std::make_shared<Observer>();

  subject.get()->registerObserver(observer1);
  subject.get()->registerObserver(observer2);

  subject->setState(1);
  subject->setState(2);

  subject.get()->unregisterObserver(observer1);
  subject->setState(3);

  return 0;
}