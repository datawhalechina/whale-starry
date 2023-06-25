# vector 的学习笔记

## 1. vector 介绍

STL 中的vector是一种动态数组，提供了像数组一样的随机访问、在末尾添加元素等操作，并且具有可自动增长和缩小容量的能力。

vector 的常用操作包括：

1. 插入和删除元素：
   - push_back：在 vector 的末尾添加元素；
   - pop_back：从 vector 的末尾删除元素；
   - insert：在指定位置插入元素；
   - erase：删除指定位置或范围内的元素。
2. 访问元素：
   - at：通过索引访问元素，并进行边界检查；
   - []：通过索引访问元素，不进行边界检查；
   - front：返回 vector 中的第一个元素；
   - back：返回 vector 中的最后一个元素。
3. 获取 vector 的长度：
   - size：返回 vector 中元素的数量；
   - empty：判断 vector 是否为空。
4. 其他操作：
   - clear：清空 vector 中的所有元素；
   - reserve：预设 vector 的容量。

## 2. vector 源码分析

先看UML图

![image-20230624091529713](C:\Users\23318\AppData\Roaming\Typora\typora-user-images\image-20230624091529713.png)

vector算是所有容器中最简单的一种，它的结构也比较简单。

### 1、**_Vector_base**

之前看list就遇到了一个不太看的懂的写法，这次解释一下**_Tp_alloc_type** 到底是什么类型    rebind -》 rebind_alloc -》 allocator

```c++
typedef typename __gnu_cxx::__alloc_traits<_Alloc>::template rebind<_Tp>::other _Tp_alloc_type;
//其实等价于
typedef allocator<_Tp> _Tp_alloc_type
```

在`__gnu_cxx::__alloc_traits`中：对应文件为：`ext/alloc_traits.h`

``` c++
template<typename _Tp>
      struct rebind
      { typedef typename _Base_type::template rebind_alloc<_Tp> other; };
// 于是
typename __gnu_cxx::__alloc_traits<_Alloc>::template rebind<_Tp>::other
// 等价于
typename _Base_type::template rebind_alloc<_Tp>
// 而
typedef std::allocator_traits<_Alloc>           _Base_type;
// 所以等价于
typename std::allocator_traits<_Alloc>::template rebind_alloc<_Tp>
```

继续到`allocator_traits`中寻找：

```c++
  template<typename _Up>
	using rebind_alloc = allocator<_Up>;
// 于是
std::allocator_traits<_Alloc>::template rebind_alloc<_Tp>
//等价于
allocator<_Tp>
```

vector**内存管理**

`_Vector_impl`类就是一个**内存管理器**，该结构体继承`allocator`

```c++
struct _Vector_impl : public _Tp_alloc_type {
    pointer _M_start;               // 使用空间起始位置
    pointer _M_finish;              // 使用空间结束位置
    pointer _M_end_of_storage;      // 可使用空间结束位置

    _Vector_impl()
        : _Tp_alloc_type(), _M_start(0), _M_finish(0), _M_end_of_storage(0) {}

    _Vector_impl(_Tp_alloc_type const& __a) _GLIBCXX_NOEXCEPT
        : _Tp_alloc_type(__a),
          _M_start(0),
          _M_finish(0),
          _M_end_of_storage(0) {}

#if __cplusplus >= 201103L
    _Vector_impl(_Tp_alloc_type&& __a) noexcept
        : _Tp_alloc_type(std::move(__a)),
          _M_start(0),
          _M_finish(0),
          _M_end_of_storage(0) {}
#endif
	// vector数据交换，只交换内存地址，不交换数据 有点像拷贝函数
    void _M_swap_data(_Vector_impl& __x) _GLIBCXX_NOEXCEPT {
      std::swap(_M_start, __x._M_start);
      std::swap(_M_finish, __x._M_finish);
      std::swap(_M_end_of_storage, __x._M_end_of_storage);
    }
  };

```

## 2、**vector  iterator**

在vector中分别使用了正向迭代器`__normal_iterator`与反向迭代器`reverse_iterator`，都定义于stl_iterator.h

```c++
template <typename _Iterator, typename _Container>
class __normal_iterator {
 protected:
  _Iterator _M_current;

  typedef iterator_traits<_Iterator> __traits_type;

 public:
  typedef _Iterator iterator_type;
  typedef typename __traits_type::iterator_category iterator_category;
  typedef typename __traits_type::value_type value_type;
  typedef typename __traits_type::difference_type difference_type;
  typedef typename __traits_type::reference reference;
  typedef typename __traits_type::pointer pointer;

  _GLIBCXX_CONSTEXPR __normal_iterator() _GLIBCXX_NOEXCEPT
      : _M_current(_Iterator()) {}

  explicit __normal_iterator(const _Iterator& __i) _GLIBCXX_NOEXCEPT
      : _M_current(__i) {}

  // Allow iterator to const_iterator conversion
  template <typename _Iter>
  __normal_iterator(
      const __normal_iterator<
          _Iter,
          typename __enable_if<
              (std::__are_same<_Iter, typename _Container::pointer>::__value),
              _Container>::__type>& __i) _GLIBCXX_NOEXCEPT
      : _M_current(__i.base()) {}

  // Forward iterator requirements
  reference operator*() const _GLIBCXX_NOEXCEPT { return *_M_current; }

  pointer operator->() const _GLIBCXX_NOEXCEPT { return _M_current; }

  __normal_iterator& operator++() _GLIBCXX_NOEXCEPT {
    ++_M_current;
    return *this;
  }

  __normal_iterator operator++(int) _GLIBCXX_NOEXCEPT {
    return __normal_iterator(_M_current++);
  }

  // Bidirectional iterator requirements
  __normal_iterator& operator--() _GLIBCXX_NOEXCEPT {
    --_M_current;
    return *this;
  }

  __normal_iterator operator--(int) _GLIBCXX_NOEXCEPT {
    return __normal_iterator(_M_current--);
  }

  // Random access iterator requirements
  reference operator[](difference_type __n) const _GLIBCXX_NOEXCEPT {
    return _M_current[__n];
  }

  __normal_iterator& operator+=(difference_type __n) _GLIBCXX_NOEXCEPT {
    _M_current += __n;
    return *this;
  }

  __normal_iterator operator+(difference_type __n) const _GLIBCXX_NOEXCEPT {
    return __normal_iterator(_M_current + __n);
  }

  __normal_iterator& operator-=(difference_type __n) _GLIBCXX_NOEXCEPT {
    _M_current -= __n;
    return *this;
  }

  __normal_iterator operator-(difference_type __n) const _GLIBCXX_NOEXCEPT {
    return __normal_iterator(_M_current - __n);
  }

  const _Iterator& base() const _GLIBCXX_NOEXCEPT { return _M_current; }
};
```

## 3、vector 容器

一些定义：

```c++
template <typename _Tp, typename _Alloc = std::allocator<_Tp>>
class vector : protected _Vector_base<_Tp, _Alloc> {
  // Concept requirements.
  typedef typename _Alloc::value_type _Alloc_value_type;
  __glibcxx_class_requires(_Tp, _SGIAssignableConcept)
      __glibcxx_class_requires2(_Tp, _Alloc_value_type, _SameTypeConcept)

          typedef _Vector_base<_Tp, _Alloc> _Base;
  typedef typename _Base::_Tp_alloc_type _Tp_alloc_type;
  typedef __gnu_cxx::__alloc_traits<_Tp_alloc_type> _Alloc_traits;

 public:
  // 定义自身的嵌套类型
  typedef _Tp value_type;
  typedef typename _Base::pointer pointer;
  typedef typename _Alloc_traits::const_pointer const_pointer;
  typedef typename _Alloc_traits::reference reference;
  typedef typename _Alloc_traits::const_reference const_reference;
  typedef __gnu_cxx::__normal_iterator<pointer, vector> iterator;
  typedef __gnu_cxx::__normal_iterator<const_pointer, vector> const_iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
  typedef std::reverse_iterator<iterator> reverse_iterator;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef _Alloc allocator_type;

 protected:
  using _Base::_M_allocate;
  using _Base::_M_deallocate;
  using _Base::_M_get_Tp_allocator;
  using _Base::_M_impl;
  ............
}
```

【元素访问】

比如返回 vector 的开始和结尾，返回最后一个元素，返回当前元素个数，元素容量，是否为空等。

这里需要注意的是因为 end() 返回的是 finish，而 finish 是指向**最后一个元素的后一个位置的指针**，所以使用 end() 的时候要注意。

```c++
// 获取数据的开始以及结束位置的指针. 记住这里返回的是迭代器, 也就是 vector 迭代器就是该类型的指针.
iterator begin() _GLIBCXX_NOEXCEPT { return iterator(this->_M_impl._M_start); }  
iterator end() _GLIBCXX_NOEXCEPT { return iterator(this->_M_impl._M_finish); }
reverse_iterator rbegin() _GLIBCXX_NOEXCEPT { return reverse_iterator(end()); }
reverse_iterator rend() _GLIBCXX_NOEXCEPT { return reverse_iterator(begin()); }
const_iterator cbegin() const noexcept { return const_iterator(this->_M_impl._M_start); }
const_iterator cend() const noexcept { return const_iterator(this->_M_impl._M_finish); }

reference front() _GLIBCXX_NOEXCEPT { return *begin(); }
reference back() _GLIBCXX_NOEXCEPT { return *(end() - 1); }

// [23.2.4.2] capacity
/**  Returns the number of elements in the %vector.  */
size_type size() const _GLIBCXX_NOEXCEPT {
	return size_type(this->_M_impl._M_finish - this->_M_impl._M_start);
}

/**  Returns the size() of the largest possible %vector.  */
size_type max_size() const _GLIBCXX_NOEXCEPT {
	return _Alloc_traits::max_size(_M_get_Tp_allocator());
}
/**
*  Returns the total number of elements that the %vector can
*  hold before needing to allocate more memory.
*/
size_type capacity() const _GLIBCXX_NOEXCEPT {
	return size_type(this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
}

/**
*  Returns true if the %vector is empty.  (Thus begin() would
*  equal end().)
*/
bool empty() const _GLIBCXX_NOEXCEPT { return begin() == end(); }
```

【push 和 pop 操作】

vector 的 push 和 pop 操作都只是对尾进行操作， 这里说的尾部是指数据的尾部

当调用 push_back 插入新元素的时候，首先会检查是否有备用空间，如果有就直接在备用空间上构造元素，并调整迭代器 finish。

```c++
void push_back(const value_type& __x) {
    // 尚有可用空间  调用全局构造函数， 调整迭代器
    if (this->_M_impl._M_finish != this->_M_impl._M_end_of_storage) {
      _Alloc_traits::construct(this->_M_impl, this->_M_impl._M_finish, __x);
      ++this->_M_impl._M_finish;
    } else					// 已无可用空间
#if __cplusplus >= 201103L
      _M_emplace_back_aux(__x);
#else
      _M_insert_aux(end(), __x);
#endif
}

template <typename _Tp, typename _Alloc>
template <typename... _Args>
void vector<_Tp, _Alloc>::_M_emplace_back_aux(_Args&&... __args) {
  const size_type __len =
      _M_check_len(size_type(1), "vector::_M_emplace_back_aux");
  pointer __new_start(this->_M_allocate(__len));
  pointer __new_finish(__new_start);
  __try {
    _Alloc_traits::construct(this->_M_impl, __new_start + size(),
                             std::forward<_Args>(__args)...);
    __new_finish = 0;

    __new_finish = std::__uninitialized_move_if_noexcept_a(
        this->_M_impl._M_start, this->_M_impl._M_finish, __new_start,
        _M_get_Tp_allocator());

    ++__new_finish;
  }
  __catch(...) {
    if (!__new_finish)
      _Alloc_traits::destroy(this->_M_impl, __new_start + size());
    else
      std::_Destroy(__new_start, __new_finish, _M_get_Tp_allocator());
    _M_deallocate(__new_start, __len);
    __throw_exception_again;
  }
  //  释放原来的 vector
  std::_Destroy(this->_M_impl._M_start, this->_M_impl._M_finish,
                _M_get_Tp_allocator());
  _M_deallocate(this->_M_impl._M_start,
                this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
  //  调整迭代器指向新 vector
  this->_M_impl._M_start = __new_start;
  this->_M_impl._M_finish = __new_finish;
  this->_M_impl._M_end_of_storage = __new_start + __len;
}
```

【删除元素】

```c++
// Remove element at given position
erase(const_iterator __position) {
    return _M_erase(begin() + (__position - cbegin()));
}

template <typename _Tp, typename _Alloc>
typename vector<_Tp, _Alloc>::iterator vector<_Tp, _Alloc>::_M_erase(
    iterator __position) {
  if (__position + 1 != end())
    _GLIBCXX_MOVE3(__position + 1, end(), __position);
  --this->_M_impl._M_finish;
  _Alloc_traits::destroy(this->_M_impl, this->_M_impl._M_finish);
  return __position;
}

// Remove a range of elements.
erase(const_iterator __first, const_iterator __last) {
    const auto __beg = begin();
    const auto __cbeg = cbegin();
    return _M_erase(__beg + (__first - __cbeg), __beg + (__last - __cbeg));
}

template <typename _Tp, typename _Alloc>
typename vector<_Tp, _Alloc>::iterator vector<_Tp, _Alloc>::_M_erase(
    iterator __first, iterator __last) {
  if (__first != __last) {
    if (__last != end()) _GLIBCXX_MOVE3(__last, end(), __first);
    _M_erase_at_end(__first.base() + (end() - __last));
  }
  return __first;
}
```

【插入元素】

```c++
// Inserts given rvalue into %vector before specified iterator.
iterator insert(const_iterator __position, value_type&& __x) {
    return emplace(__position, std::move(__x));
}

// Inserts an initializer_list into the %vector.
iterator insert(const_iterator __position, initializer_list<value_type> __l) {
    return this->insert(__position, __l.begin(), __l.end());
}

// Inserts a number of copies of given data into the %vector.
iterator insert(const_iterator __position, size_type __n,
                  const value_type& __x) {
    difference_type __offset = __position - cbegin();
    _M_fill_insert(begin() + __offset, __n, __x);
    return begin() + __offset;
}

template <typename _Tp, typename _Alloc>
void vector<_Tp, _Alloc>::_M_fill_insert(iterator __position, size_type __n,
                                         const value_type& __x) {
  if (__n != 0) {
    if (size_type(this->_M_impl._M_end_of_storage - this->_M_impl._M_finish) >=
        __n) {
      value_type __x_copy = __x;
      const size_type __elems_after = end() - __position;
      pointer __old_finish(this->_M_impl._M_finish);
      if (__elems_after > __n) {
        std::__uninitialized_move_a(
            this->_M_impl._M_finish - __n, this->_M_impl._M_finish,
            this->_M_impl._M_finish, _M_get_Tp_allocator());
        this->_M_impl._M_finish += __n;
        _GLIBCXX_MOVE_BACKWARD3(__position.base(), __old_finish - __n,
                                __old_finish);
        std::fill(__position.base(), __position.base() + __n, __x_copy);
      } else {
        std::__uninitialized_fill_n_a(this->_M_impl._M_finish,
                                      __n - __elems_after, __x_copy,
                                      _M_get_Tp_allocator());
        this->_M_impl._M_finish += __n - __elems_after;
        std::__uninitialized_move_a(__position.base(), __old_finish,
                                    this->_M_impl._M_finish,
                                    _M_get_Tp_allocator());
        this->_M_impl._M_finish += __elems_after;
        std::fill(__position.base(), __old_finish, __x_copy);
      }
    } else {
      const size_type __len = _M_check_len(__n, "vector::_M_fill_insert");
      const size_type __elems_before = __position - begin();
      pointer __new_start(this->_M_allocate(__len));
      pointer __new_finish(__new_start);
      __try {
        // See _M_insert_aux above.
        std::__uninitialized_fill_n_a(__new_start + __elems_before, __n, __x,
                                      _M_get_Tp_allocator());
        __new_finish = 0;

        __new_finish = std::__uninitialized_move_if_noexcept_a(
            this->_M_impl._M_start, __position.base(), __new_start,
            _M_get_Tp_allocator());

        __new_finish += __n;

        __new_finish = std::__uninitialized_move_if_noexcept_a(
            __position.base(), this->_M_impl._M_finish, __new_finish,
            _M_get_Tp_allocator());
      }
      __catch(...) {
        if (!__new_finish)
          std::_Destroy(__new_start + __elems_before,
                        __new_start + __elems_before + __n,
                        _M_get_Tp_allocator());
        else
          std::_Destroy(__new_start, __new_finish, _M_get_Tp_allocator());
        _M_deallocate(__new_start, __len);
        __throw_exception_again;
      }
      std::_Destroy(this->_M_impl._M_start, this->_M_impl._M_finish,
                    _M_get_Tp_allocator());
      _M_deallocate(this->_M_impl._M_start,
                    this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
      this->_M_impl._M_start = __new_start;
      this->_M_impl._M_finish = __new_finish;
      this->_M_impl._M_end_of_storage = __new_start + __len;
    }
  }
}
```

还有很多没有分析，先看到这里 ..........

## 总结一下

需要提醒的是，vector成员函数都不做边界检查(at方法会抛异常)，自己要确保迭代器和索引值的合法性。

vector优点：

- 在内存中分配一块连续的内存空间进行存，可以像数组一样操作，动态扩容。
- 随机访问方便，支持下标访问和vector.at()操作。
- 节省空间。

vector缺点：

- 由于其顺序存储的特性，vector 插入删除操作的时间复杂度是 O(n)。
- 只能在末端进行pop和push。
- 当动态长度超过默认分配大小后，要整体重新分配、拷贝和释放空间。
