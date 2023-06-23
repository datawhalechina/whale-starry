#ifndef _IVARND_VECTOR_H_
#define _IVARND_VECTOR_H_

namespace ivarnd {
// 定义vector结构
template <class T>
class vector {
 public:
  typedef T* iterator;
  typedef const T* const_iterator;

  vector() : _start(nullptr), _finish(nullptr), _end_of_storagre(nullptr) {}

  void push_back(const T& x) {
    if (_finish == _end_of_storagre) {
      // 扩容
      reverse(capacity() == 0 ? 4 : capacity() * 2);
    }
    *_finish = x;
    ++_finish;
  }
  void pop_back() {
    assert(!empty());
    --_finish;
  }
  void reverse(size_t n) {
    if (n > capacity()) {
      size_t sz = size();
      T* tmp = new T[n];
      if (_start) {
        memcpy(tmp, _start, size() * sizeof(T));
        delete[] _start;
      }
      _start = tmp;
      _finish = _start + sz;
      _end_of_storagre = _start + n;
    }
  }

  void resize(size_t n, T val = T()) {
    if (n < size()) {
      _finish = _start + n;
    } else {
      if (n > capacity()) {
        reverse(n);
      }
      // 初始化
      while (_finish != _start + n) {
        *_finish = val;
        ++_finish;
      }
    }
  }

  iterator begin() { return _start; }
  
  iterator end() { return _finish; }

  bool empty() { return _start == _finish; }

  size_t capacity() const { return _end_of_storagre - _start; }
  size_t size() const { return _finish - _start; }

  T& operator[](size_t pos) {
    assert(pos < size());
    return _start[pos];
  }

  const_iterator begin() const { return _start; }
  const_iterator end() const { return _finish; }
  const T& operator[](size_t pos) const {
    assert(pos < size());
    return _start[pos];
  }

 private:
  iterator _start;
  iterator _finish;
  iterator _end_of_storagre;
};

}  // namespace ivarnd

#endif  // _IVARND_VECTOR_H_