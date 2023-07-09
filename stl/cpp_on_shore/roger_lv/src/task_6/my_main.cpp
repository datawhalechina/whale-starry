#include <iostream>
#include <vector>

class MyIterator {
 private:
  std::vector<int>::iterator iter;

 public:
  MyIterator(std::vector<int>::iterator iter) : iter(iter) {}

  bool operator==(const MyIterator& other) const { return iter == other.iter; }

  bool operator!=(const MyIterator& other) const { return !(*this == other); }

  int& operator*() { return *iter; }

  MyIterator& operator++() {
    ++iter;
    return *this;
  }

  MyIterator operator++(int) {
    MyIterator temp = *this;
    ++(*this);
    return temp;
  }

  MyIterator& operator--() {
    --iter;
    return *this;
  }

  MyIterator operator--(int) {
    MyIterator temp = *this;
    --(*this);
    return temp;
  }
};

int main() {
  std::vector<int> myVector = {1, 2, 3, 4, 5};
  MyIterator myIterator(myVector.begin());

  while (myIterator != myVector.end()) {
    std::cout << *myIterator << " ";
    ++myIterator;
  }

  return 0;
}
