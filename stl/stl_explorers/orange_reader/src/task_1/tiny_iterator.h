namespace __tiny_stl {

// iterator type: GCC 4.9 实现
// input iterators.
struct input_iterator_tag {};

// output iterators.
struct output_iterator_tag {};

// Forward iterators support a superset of input iterator operations.
struct forward_iterator_tag : public input_iterator_tag {};

// Bidirectional iterators support a superset of forward iterator
// operations.
struct bidirectional_iterator_tag : public forward_iterator_tag {};

// Random-access iterators support a superset of bidirectional
// iterator operations.
struct random_access_iterator_tag : public bidirectional_iterator_tag {};

// iterator traits
// 1. iterator is class
template <typename I>
struct iterator_traits {
  // ⭐ need typename before I::value_type to identify this is a type
  using value_type = typename I::value_type;
};

// // 2. iterator is pointer
template <typename T>
struct iterator_traits<T*> {
  using value_type = T;
};

// // 2. iterator is const pointer
// // value_type主要用于声明变量，而声明一个常量(无法赋值)没什么用处
// // 因此返回的是T，而非const T
template <typename T>
struct iterator_traits<const T*> {
  using value_type = T;
};

}  // namespace __tiny_stl