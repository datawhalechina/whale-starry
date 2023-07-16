#include <deque>
#include <iostream>
#include <type_traits>
#include <vector>

using std::iterator;
using std::iterator_traits;

template <typename _Iterator, typename _Container>
class SequenceIterator {
 protected:
  _Iterator iter;
  typedef iterator_traits<_Iterator> __traits_type;

 public:
  typedef _Iterator iterator_type;
  typedef typename __traits_type::iterator_category iterator_category;
  typedef typename __traits_type::value_type value_type;
  typedef typename __traits_type::difference_type difference_type;
  typedef typename __traits_type::reference reference;
  typedef typename __traits_type::pointer pointer;

  SequenceIterator(const _Iterator& it) : iter(it) {}
  reference operator*() const { return *iter; }
  pointer operator->() const { return iter; }
  SequenceIterator& operator++() {
    ++iter;
    return *this;
  }
  SequenceIterator operator++(int) { return SequenceIterator(iter++); }
  SequenceIterator& operator--() {
    --iter;
    return *this;
  }
  SequenceIterator operator--(int) { return SequenceIterator(iter--); }
  reference operator[](difference_type __n) const { return iter[__n]; }
  SequenceIterator& operator+=(difference_type __n) {
    iter += __n;
    return *this;
  }
  SequenceIterator operator+(difference_type __n) { return SequenceIterator(iter + __n); }
  SequenceIterator& operator-=(difference_type __n) {
    iter -= __n;
    return *this;
  }
  SequenceIterator operator-(difference_type __n) { return SequenceIterator(iter - __n); }
  const _Iterator& base() const { return iter; }
};

template <typename _IteratorL, typename _IteratorR, typename _Container>
inline bool operator==(const SequenceIterator<_IteratorL, _Container>& __lhs,
                       const SequenceIterator<_IteratorR, _Container>& __rhs) {
  return __lhs.base() == __rhs.base();
}

template <typename _IteratorL, typename _IteratorR, typename _Container>
inline bool operator!=(const SequenceIterator<_IteratorL, _Container>& __lhs,
                       const SequenceIterator<_IteratorR, _Container>& __rhs) {
  return __lhs.base() != __rhs.base();
}