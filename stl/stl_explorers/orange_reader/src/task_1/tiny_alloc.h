#include <cstdlib>

namespace __tiny_stl {
// TODO:default memory allocator
template <typename T>
struct allocator {
  using value_type = T;
  using pointer = T*;

  // 修改allocator的模板参数类型T -> U
  // [std::allocator](https://en.cppreference.com/w/cpp/memory/allocator)
  template <typename U>
  struct rebind {
    typedef allocator<U> other;
  };

  pointer allocate() { return (pointer)malloc(sizeof(T)); }

  void deallocate(pointer m) { free(m); }
};

// TODO: other allocator, like pool_allocator...
}  // namespace __tiny_stl