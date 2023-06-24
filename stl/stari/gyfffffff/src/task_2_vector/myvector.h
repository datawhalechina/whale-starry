#include <stdlib.h>

#include <iostream>

template <typename T>
class myvector {
 public:
  typedef T value_type;
  typedef value_type* iterator;
  typedef value_type& reference;
  typedef size_t size_type;

 protected:
  iterator start = nullptr;
  iterator finish = nullptr;
  iterator end_of_storage = nullptr;

 public:
  // constructor 1
  myvector() {
    start = new value_type;
    finish = start;
    end_of_storage = start;
  };

  // constructor 2
  myvector(size_type n, const T _x) {
    start = new value_type[n];
    finish = start + n;
    end_of_storage = finish;
    iterator tem = start;
    while (n--) {
      *(start + n) = _x;
    }
  }

  // begin(),end_of_storage(),size(),max_size()
  iterator begin() { return start; }
  iterator end() { return finish; }
  size_type size() const { return size_type(finish - start); }
  size_type max_size() const { return size_type(end_of_storage - start); }

  // []索引, 没有越界检查
  reference operator[](size_type n) { return *(start + n); }

  // push_back(T _x)
  void push_back(const T& _x) {
    if (finish == end_of_storage) {
      double_size();
      *finish = _x;
      ++finish;
    } else {
      *finish = _x;
      ++finish;
    }
  }

  // empty()
  bool empty() const { return start == finish; }

  void print() {
    iterator i = start;
    while (i != finish) {
      std::cout << *i << " ";
      ++i;
    }
    std::cout << std::endl;
  }

  ~myvector() { delete[] start; }

 private:
  void double_size() {
    std::cout << "current size:" << size();
    std::cout << "  call double_size." << std::endl;
    int cursize = finish - start;
    int newsize = (cursize == 0 ? 1 : cursize * 2);
    iterator new_start = new value_type[newsize];
    iterator newiter = new_start;
    iterator olditer = nullptr;
    for (olditer = start; olditer != finish; olditer++) {
      *newiter = *(olditer);
      ++newiter;
    }
    start = new_start;
    finish = start + cursize;
    end_of_storage = new_start + newsize;
  }
};