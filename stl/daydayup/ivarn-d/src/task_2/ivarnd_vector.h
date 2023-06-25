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
  vector(size_t n, const T& val = T())
      : _start(nullptr), _finish(nullptr), _end_of_storagre(nullptr) {
    reverse(n);
    for (size_t i = 0; i < n; ++i) {
      push_back(val);
    }
  }
  vector(int n, const T& val = T())
      : _start(nullptr), _finish(nullptr), _end_of_storagre(nullptr) {
    reverse(n);
    for (int i = 0; i < n; ++i) {
      push_back(val);
    }
  }
  template <class InputIterator>
  vector(InputIterator first, InputIterator last)
      : _start(nullptr), _finish(nullptr), _end_of_storagre(nullptr) {
    while (first != last) {
      push_back(*first);
      ++first;
    }
  }

  /*
  // 不够传统
  vector(const vector<T>& v) {
    reverse(v.capacity());
    for (auto e : v) {
      push_back(e);
    }
  }
  */

  vector(const vector<T>& v) {
    _start = new T[v.capacity()];
    // memcpy(_start, v._start, sizeof(T) * v.size());
    for (size_t i = 0; i < v.size(); ++i) {
      _start[i] = v._start[i];
    }
    _finish = _start + v.size();
    _end_of_storagre = _start + v.capacity();
  }

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
        // memcpy(tmp, _start, size() * sizeof(T));
        for (size_t i = 0; i < sz; ++i) {
          tmp[i] = _start[i];
        }
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

  void swap(vector<T>& v) {
    std::swap(_start, v._start);
    std::swap(_finish, v._finish);
    std::swap(_end_of_storagre, v._end_of_storagre);
  }

  iterator insert(iterator pos, const T& val) {
    assert(pos >= _start && pos <= _finish);
    if (_finish == _end_of_storagre) {
      size_t len = pos - _start;
      reverse(capacity() == 0 ? 4 : capacity() * 2);
      // 扩容后更新pos，解决pos失效问题
      pos = _start + len;
    }
    iterator end = _finish - 1;
    while (end >= pos) {
      *(end + 1) = *end;
      --end;
    }
    *pos = val;
    ++_finish;
    return pos;
  }

  iterator erase(iterator pos) {
    assert(pos >= _start && pos < _finish);
    iterator start = pos + 1;
    while (start != _finish) {
      *(start - 1) = *start;
      ++start;
    }
    --_finish;
    return pos;
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

  vector<T>& operator=(vector<T> v) {
    swap(v);
    return *this;
  }

  ~vector() {
    delete[] _start;
    _start = _finish = _end_of_storagre = nullptr;
  }

 private:
  iterator _start = nullptr;
  iterator _finish = nullptr;
  iterator _end_of_storagre = nullptr;
};

}  // namespace ivarnd

#endif  // _IVARND_VECTOR_H_