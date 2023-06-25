#include "observer.h"

int Observer::static_number_ = 0;

int main() {
  Subject* subject = new Subject();
  Observer* observer1 = new Observer(subject);
  Observer* observer2 = new Observer(subject);

  subject->setState(1);
  subject->setState(2);

  //   subject->removeObserver(observer);
  observer1->removeObserver();

  subject->setState(3);

  return 0;
}