#ifndef TINYSTL_ITERATOR_H_
#define TINYSTL_ITERATOR_H_

namespace tiny_stl {

// iterator type: GCC 4.9 实现
// input iterators.
struct InputIteratorTag {};

// output iterators.
struct OutputIteratorTag {};

// Forward iterators support a superset of input iterator operations.
struct ForwardIteratorTag : public InputIteratorTag {};

// Bidirectional iterators support a superset of forward iterator
// operations.
struct BidirectionalIteratorTag : public ForwardIteratorTag {};

// Random-access iterators support a superset of bidirectional
// iterator operations.
struct RandAccessIteratorTag : public BidirectionalIteratorTag {};

// iterator traits
// 1. iterator is class
template <typename I>
struct IteratorTraits {
  // ⭐ need typename before I::ValueType to identify this is a type
  using ValueType = typename I::ValueType;
};

// // 2. iterator is Pointer
template <typename T>
struct IteratorTraits<T*> {
  using ValueType = T;
};

// // 2. iterator is const Pointer
// // value_type主要用于声明变量，而声明一个常量(无法赋值)没什么用处
// // 因此返回的是T，而非const T
template <typename T>
struct IteratorTraits<const T*> {
  using ValueType = T;
};

}  // namespace tiny_stl

#endif