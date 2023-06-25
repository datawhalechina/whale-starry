#include "observer.h"

#include <iostream>

void Subject::registerObserver(Observer* o) { observers_.push_back(o); }

void Subject::removeObserver(Observer* o) {
  for (auto it = observers_.begin(); it != observers_.end(); ++it) {
    if (*it == o) {
      observers_.erase(it);
      break;
    }
  }
}

void Subject::setState(int state) {
  state_ = state;
  // 演示用
  // 自动调用notifyObservers
  notifyObservers();
}

int Subject::getState() { return state_; }

void Subject::notifyObservers() {
  for (auto it = observers_.begin(); it != observers_.end(); ++it) {
    (*it)->notify();
  }
}

Observer::Observer(Subject* subject) : subject_(*subject) {
  Observer::static_number_++;
  subject_.registerObserver(this);
  this->number_ = Observer::static_number_;

  std::cout << "Observer " << this->number_ << " Created" << std::endl;
}

void Observer::removeObserver() {
  subject_.removeObserver(this);
  std::cout << "Observer " << this->number_ << " Removed" << std::endl;
}

void Observer::notify() {
  std::cout << "Observer " << this->number_ << " notified" << std::endl;
  std::cout << "\tSubject state: " << subject_.getState() << std::endl;
}