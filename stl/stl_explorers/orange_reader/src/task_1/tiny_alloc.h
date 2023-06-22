#ifndef TINYSTL_ALLOC_H_
#define TINYSTL_ALLOC_H_

#include <cstdlib>

namespace tiny_stl {
// TODO:default memory allocator
template <typename T>
struct Allocator {
  using ValueType = T;
  using Pointer = T*;

  // 修改allocator的模板参数类型T -> U
  // [std::allocator](https://en.cppreference.com/w/cpp/memory/allocator)
  template <typename U>
  struct Rebind {
    typedef Allocator<U> other;
  };

  Pointer allocate() { return (Pointer)malloc(sizeof(T)); }

  void deallocate(Pointer m) { free(m); }
};

// TODO: other allocator, like pool_allocator...
}  // namespace tiny_stl

#endif