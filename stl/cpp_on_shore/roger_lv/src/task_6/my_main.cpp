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
  std::vector<int> my_vector = {1, 2, 3, 4, 5};
  MyIterator my_iterator(my_vector.begin());

  while (my_iterator != my_vector.end()) {
    std::cout << *my_iterator << " ";
    ++my_iterator;
  }

  return 0;
}
