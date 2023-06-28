#include "observer.h"

#include <iostream>

//--------------------------------------------------------------------
// Subject
//--------------------------------------------------------------------
Subject::Subject() { std::cout << "Subject created" << std::endl; }

Subject::~Subject() { std::cout << "Subject destroyed" << std::endl; }

void Subject::registerObserver(std::shared_ptr<Observer> o) { observers_.push_back(o); }

void Subject::unregisterObserver(std::shared_ptr<Observer> o) {
  for (auto it = observers_.begin(); it != observers_.end(); ++it) {
    if (*it == o) {
      observers_.erase(it);
      break;
    }
  }
}

void Subject::setState(int state) {
  state_ = state;
  notifyObservers();
}

int Subject::getState() { return state_; }

void Subject::notifyObservers() {
  for (auto it = observers_.begin(); it != observers_.end(); ++it) {
    (*it)->notify();
  }
}

//--------------------------------------------------------------------
// Observer
//--------------------------------------------------------------------
Observer::Observer() {
  Observer::static_number_++;
  this->number_ = Observer::static_number_;
}

Observer::~Observer() {
  std::cout << "Observer " << this->number_ << " destroyed" << std::endl;
}

std::shared_ptr<Observer> Observer::shared_observer() {
  std::shared_ptr<Observer> obj = shared_from_this();
  return obj;
}

void Observer::notify() {
  std::cout << "Observer " << this->number_ << " notified" << std::endl;
}
