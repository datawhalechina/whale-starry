#include <iostream>
#include <vector>

template <typename T>
class MyIterator {
 public:
  typedef T value_type;
  typedef T& reference;
  typedef T* pointer;
  typedef typename std::ptrdiff_t difference_type;
  typedef typename std::bidirectional_iterator_tag iterator_category;

  MyIterator(pointer ptr) : m_ptr(ptr) {}

  reference operator*() const { return *m_ptr; }

  pointer operator->() const { return m_ptr; }

  MyIterator& operator++() {
    ++m_ptr;
    return *this;
  }

  MyIterator operator++(int) {
    MyIterator temp = *this;
    ++m_ptr;
    return temp;
  }

  MyIterator& operator--() {
    --m_ptr;
    return *this;
  }

  MyIterator operator--(int) {
    MyIterator temp = *this;
    --m_ptr;
    return temp;
  }

  bool operator==(const MyIterator& other) const { return m_ptr == other.m_ptr; }

  bool operator!=(const MyIterator& other) const { return !(*this == other); }

 private:
  pointer m_ptr;
};

int main() {
  std::vector<int> numbers = {1, 2, 3, 4, 5};

  MyIterator<int> beginIt(numbers.data());
  MyIterator<int> endIt(numbers.data() + numbers.size());

  // 使用迭代器进行遍历
  for (MyIterator<int> it = beginIt; it != endIt; ++it) {
    std::cout << *it << " ";
  }
  std::cout << std::endl;

  // 使用迭代器修改元素
  for (MyIterator<int> it = beginIt; it != endIt; ++it) {
    *it *= 2;
  }

  // 遍历
  for (MyIterator<int> it = beginIt; it != endIt; ++it) {
    std::cout << *it << " ";
  }
  std::cout << std::endl;

  return 0;
}
