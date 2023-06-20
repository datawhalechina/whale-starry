# list 的学习笔记

## 1. list 介绍

STL（Standard Template Library，标准模板库）是C++标准库的一部分，它包含了一系列通用的数据结构和算法。其中，`std::list` 是STL中的一个容器类，用于表示双向链表。与其他线性容器（如 `std::vector` 和 `std::deque`）相比，`std::list` 提供了快速的元素插入和删除操作，而不用关心内存重新分配的问题。

`std::list` 的基本特点如下：

1. 双向链表：每个节点都有一个指向前一个节点的指针和一个指向后一个节点的指针。
2. 动态大小：链表的大小可以在运行时动态改变，插入和删除元素的时间复杂度为O(1)。
3. 非连续存储：链表中的元素可能分散在内存的不同位置，因此不能通过下标直接访问元素。
4. 不支持随机访问：要访问链表中的某个元素，需要从头遍历到目标元素，时间复杂度为O(n)。

常用的 `std::list` 成员函数包括：

- `push_back()`: 在链表尾部插入一个元素。
- `push_front()`: 在链表头部插入一个元素。
- `pop_back()`: 删除链表尾部的元素。
- `pop_front()`: 删除链表头部的元素。
- `insert()`: 在指定位置插入一个或多个元素。
- `erase()`: 删除指定位置的元素。
- `size()`: 返回链表中元素的数量。
- `empty()`: 判断链表是否为空。
- `clear()`: 删除链表中的所有元素。
- `splice()`: 将另一个链表的元素移动到当前链表的指定位置。
- `merge()`: 合并两个有序链表，结果仍然是有序的。

## 2.list 源码分析

先看UML图

![img](https://img2018.cnblogs.com/blog/1285081/201809/1285081-20180917230805741-810429693.png)

我们只需把握住整体结构即可，实现总共由三部分组成，链表节点(struct _List_node : public __detail::_List_node_base) ，迭代器（struct _List_iterator），链表数据结构（class list : protected _List_base<_Tp, _Alloc>）

### 1、**链表节点，父类维护两个指针，子类才加入具体的value**

```c++
/// Common part of a node in the %list. 
struct _List_node_base {
  _List_node_base* _M_next;
  _List_node_base* _M_prev;

  static void swap(_List_node_base& __x,
                   _List_node_base& __y) _GLIBCXX_USE_NOEXCEPT;

  void _M_transfer(_List_node_base* const __first,
                   _List_node_base* const __last) _GLIBCXX_USE_NOEXCEPT;

  void _M_reverse() _GLIBCXX_USE_NOEXCEPT;

  void _M_hook(_List_node_base* const __position) _GLIBCXX_USE_NOEXCEPT;

  void _M_unhook() _GLIBCXX_USE_NOEXCEPT;
};

template <typename _Tp>
struct _List_node : public __detail::_List_node_base {
  ///< User's data.
  _Tp _M_data;

#if __cplusplus >= 201103L
  template <typename... _Args>
  _List_node(_Args&&... __args)
      : __detail::_List_node_base(), _M_data(std::forward<_Args>(__args)...) {}
#endif
};
```

### 2、**迭代器，主要是实现++和--等操作符重载，实现链表节点的前后移动。**

迭代器需要遵循的原则就是，满足算法的要求，其中使用了iterator traits，通过添加一个中间层，将class iterator和 non-class iterator统一起来。

iterator traits被称为**特性萃取机**，能够方便的让外界获取以下5种型别：

- value_type：迭代器所指对象的型别
- difference_type：两个迭代器之间的距离
- pointer：迭代器所指向的型别
- reference：迭代器所引用的型别
- iterator_category：是一种分类（移动的性质， 比如一次移动一个，或者一次移动多个等）

```c++
template <typename _Tp>
struct _List_iterator {
  typedef _List_iterator<_Tp> _Self;
  typedef _List_node<_Tp> _Node;

  typedef ptrdiff_t difference_type;
  typedef std::bidirectional_iterator_tag iterator_category;
  typedef _Tp value_type;
  typedef _Tp* pointer;
  typedef _Tp& reference;

  _List_iterator() _GLIBCXX_NOEXCEPT : _M_node() {}

  explicit _List_iterator(__detail::_List_node_base* __x) _GLIBCXX_NOEXCEPT
      : _M_node(__x) {}

  _Self _M_const_cast() const _GLIBCXX_NOEXCEPT { return *this; }

  // Must downcast from _List_node_base to _List_node to get to _M_data.
  reference operator*() const _GLIBCXX_NOEXCEPT {
    return static_cast<_Node*>(_M_node)->_M_data;
  }

  pointer operator->() const _GLIBCXX_NOEXCEPT {
    return std::__addressof(static_cast<_Node*>(_M_node)->_M_data);
  }

  _Self& operator++() _GLIBCXX_NOEXCEPT {
    _M_node = _M_node->_M_next;
    return *this;
  }

  _Self operator++(int) _GLIBCXX_NOEXCEPT {
    _Self __tmp = *this;
    _M_node = _M_node->_M_next;
    return __tmp;
  }

  _Self& operator--() _GLIBCXX_NOEXCEPT {
    _M_node = _M_node->_M_prev;
    return *this;
  }

  _Self operator--(int) _GLIBCXX_NOEXCEPT {
    _Self __tmp = *this;
    _M_node = _M_node->_M_prev;
    return __tmp;
  }

  bool operator==(const _Self& __x) const _GLIBCXX_NOEXCEPT {
    return _M_node == __x._M_node;
  }

  bool operator!=(const _Self& __x) const _GLIBCXX_NOEXCEPT {
    return _M_node != __x._M_node;
  }

  // The only member points to the %list element.
  __detail::_List_node_base* _M_node;
};
```

### 3、**链表数据结构**

#### 实现类 _List_impl，主要用来维护链表节点，然后list类包含该类

```c++
struct _List_impl : public _Node_alloc_type {
    __detail::_List_node_base _M_node;

    _List_impl() : _Node_alloc_type(), _M_node() {}

    _List_impl(const _Node_alloc_type& __a) _GLIBCXX_NOEXCEPT
        : _Node_alloc_type(__a),
          _M_node() {}

#if __cplusplus >= 201103L
    _List_impl(_Node_alloc_type&& __a) _GLIBCXX_NOEXCEPT
        : _Node_alloc_type(std::move(__a)),
          _M_node() {}
#endif
  };
```

#### _List_base类

```c++
/// See bits/stl_deque.h's _Deque_base for an explanation.
template <typename _Tp, typename _Alloc>
class _List_base {
 protected:

  typedef
      typename _Alloc::template rebind<_List_node<_Tp>>::other _Node_alloc_type;

  typedef typename _Alloc::template rebind<_Tp>::other _Tp_alloc_type;

  struct _List_impl : public _Node_alloc_type {
    __detail::_List_node_base _M_node;

    _List_impl() : _Node_alloc_type(), _M_node() {}

    _List_impl(const _Node_alloc_type& __a) _GLIBCXX_NOEXCEPT
        : _Node_alloc_type(__a),
          _M_node() {}

#if __cplusplus >= 201103L
    _List_impl(_Node_alloc_type&& __a) _GLIBCXX_NOEXCEPT
        : _Node_alloc_type(std::move(__a)),
          _M_node() {}
#endif
  };

  _List_impl _M_impl;

  _List_node<_Tp>* _M_get_node() {
    return _M_impl._Node_alloc_type::allocate(1);
  }

  void _M_put_node(_List_node<_Tp>* __p) _GLIBCXX_NOEXCEPT {
    _M_impl._Node_alloc_type::deallocate(__p, 1);
  }

 public:
  typedef _Alloc allocator_type;

  _Node_alloc_type& _M_get_Node_allocator() _GLIBCXX_NOEXCEPT {
    return *static_cast<_Node_alloc_type*>(&_M_impl);
  }

  const _Node_alloc_type& _M_get_Node_allocator() const _GLIBCXX_NOEXCEPT {
    return *static_cast<const _Node_alloc_type*>(&_M_impl);
  }

  _Tp_alloc_type _M_get_Tp_allocator() const _GLIBCXX_NOEXCEPT {
    return _Tp_alloc_type(_M_get_Node_allocator());
  }

  allocator_type get_allocator() const _GLIBCXX_NOEXCEPT {
    return allocator_type(_M_get_Node_allocator());
  }

  _List_base() : _M_impl() { _M_init(); }

  _List_base(const _Node_alloc_type& __a) _GLIBCXX_NOEXCEPT : _M_impl(__a) {
    _M_init();
  }

#if __cplusplus >= 201103L
  _List_base(_List_base&& __x) noexcept
      : _M_impl(std::move(__x._M_get_Node_allocator())) {
    _M_init();
    __detail::_List_node_base::swap(_M_impl._M_node, __x._M_impl._M_node);
  }
#endif

  // This is what actually destroys the list.
  ~_List_base() _GLIBCXX_NOEXCEPT { _M_clear(); }

  void _M_clear() _GLIBCXX_NOEXCEPT;

  void _M_init() _GLIBCXX_NOEXCEPT {
    this->_M_impl._M_node._M_next = &this->_M_impl._M_node;
    this->_M_impl._M_node._M_prev = &this->_M_impl._M_node;
  }
};
```

#### list类  只截取可以体现list代码结构的一部分，具体接口实现看源码！

**list继承_List_base**

```c++
template <typename _Tp, typename _Alloc = std::allocator<_Tp>>
class list : protected _List_base<_Tp, _Alloc> {
    // concept requirements
  typedef typename _Alloc::value_type _Alloc_value_type;
  __glibcxx_class_requires(_Tp, _SGIAssignableConcept)
      __glibcxx_class_requires2(_Tp, _Alloc_value_type, _SameTypeConcept)

          typedef _List_base<_Tp, _Alloc> _Base;
  typedef typename _Base::_Tp_alloc_type _Tp_alloc_type;
  typedef typename _Base::_Node_alloc_type _Node_alloc_type;

 public:
  typedef _Tp value_type;
  typedef typename _Tp_alloc_type::pointer pointer;
  typedef typename _Tp_alloc_type::const_pointer const_pointer;
  typedef typename _Tp_alloc_type::reference reference;
  typedef typename _Tp_alloc_type::const_reference const_reference;
  typedef _List_iterator<_Tp> iterator;
  typedef _List_const_iterator<_Tp> const_iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
  typedef std::reverse_iterator<iterator> reverse_iterator;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef _Alloc allocator_type;

 protected:
  // Note that pointers-to-_Node's can be ctor-converted to
  // iterator types.
  typedef _List_node<_Tp> _Node;

  using _Base::_M_get_node;
  using _Base::_M_get_Node_allocator;
  using _Base::_M_get_Tp_allocator;
  using _Base::_M_impl;
  using _Base::_M_put_node;
  .............
};
```

**双向环形链表实现**

【构造函数】

```c++
/**
*  @brief  Creates a %list with no elements.
*  @param  __a  An allocator object.
*/
explicit list(const allocator_type& __a) _GLIBCXX_NOEXCEPT
  : _Base(_Node_alloc_type(__a)) {}


/**
*  @brief  Creates a %list with default constructed elements.
*  @param  __n  The number of elements to initially create.
*
*  This constructor fills the %list with @a __n default
*  constructed elements.
*/
explicit list(size_type __n) : _Base() { _M_default_initialize(__n); }

/**
*  @brief  Creates a %list with copies of an exemplar element.
*  @param  __n  The number of elements to initially create.
*  @param  __value  An element to copy.
*  @param  __a  An allocator object.
*
*  This constructor fills the %list with @a __n copies of @a __value.
*/
list(size_type __n, const value_type& __value,
   const allocator_type& __a = allocator_type())
  : _Base(_Node_alloc_type(__a)) {
	_M_fill_initialize(__n, __value);}

/**
*  @brief  %List copy constructor.
*  @param  __x  A %list of identical element and allocator types.
*
*  The newly-created %list uses a copy of the allocation object used
*  by @a __x.
*/
list(const list& __x) : _Base(__x._M_get_Node_allocator()) {
	_M_initialize_dispatch(__x.begin(), __x.end(), __false_type());}

/**
*  @brief  %List move constructor.
*  @param  __x  A %list of identical element and allocator types.
*
*  The newly-created %list contains the exact contents of @a __x.
*  The contents of @a __x are a valid, but unspecified %list.
*/
list(list&& __x) noexcept : _Base(std::move(__x)) {}

................

```

【创建节点】

```c++
template <typename... _Args>
_Node* _M_create_node(_Args&&... __args) {
    _Node* __p = this->_M_get_node();
    __try 
    {
      // 这个是构造对象 _M_get_Node_allocator()是一个分配器， __p是一个指针，__args是构造参数 
      _M_get_Node_allocator().construct(__p, std::forward<_Args>(__args)...);
    }
    __catch(...) 
    {
      _M_put_node(__p);
      __throw_exception_again;
    }
    return __p;
}
```

这里面有两个重要的函数`_M_get_node`和`_M_put_node`，作用就是分配和释放内存，它们来自基类_List_base：

```c++
_List_node<_Tp>* _M_get_node() {
    // _M_impl._Node_alloc_type  An allocator
    // 1 The number of objects to allocate space for
	return _M_impl._Node_alloc_type::allocate(1);
}
void _M_put_node(_List_node<_Tp>* __p) _GLIBCXX_NOEXCEPT {
    _M_impl._Node_alloc_type::deallocate(__p, 1);
}
```

【插入节点】

函数`_M_fill_initialize` -> `push_back` -> `_M_insert` -> `_M_hook`(太难找了！)`_M_hook`实现在`gcc-4.9.1/libstdc++-v3/src/c++98/list.cc`中。

```c++
void _M_fill_initialize(size_type __n, const value_type& __x) {
    for (; __n; --__n) push_back(__x);
}

void push_back(const value_type& __x) { this->_M_insert(end(), __x); }

template <typename... _Args>
void _M_insert(iterator __position, _Args&&... __args) {
    _Node* __tmp = _M_create_node(std::forward<_Args>(__args)...);
    __tmp->_M_hook(__position._M_node);
}

void _List_node_base::_M_hook(_List_node_base* const __position)
    _GLIBCXX_USE_NOEXCEPT {
    this->_M_next = __position;
    this->_M_prev = __position->_M_prev;
    __position->_M_prev->_M_next = this;
    __position->_M_prev = this;
}
```

【删除节点】

通过迭代器删除，对应函数为_M_erase， 其中`_M_unhook`实现在`gcc-4.9.1/libstdc++-v3/src/c++98/list.cc`

```c++
void pop_back() _GLIBCXX_NOEXCEPT {
    this->_M_erase(iterator(this->_M_impl._M_node._M_prev));
}

// Erases element at position given.
  void _M_erase(iterator __position) _GLIBCXX_NOEXCEPT {
    __position._M_node->_M_unhook();
    _Node* __n = static_cast<_Node*>(__position._M_node);
#if __cplusplus >= 201103L
    _M_get_Node_allocator().destroy(__n);
#else
    _M_get_Tp_allocator().destroy(std::__addressof(__n->_M_data));
#endif
    _M_put_node(__n);
  }

void _List_node_base::_M_unhook() _GLIBCXX_USE_NOEXCEPT {
  _List_node_base* const __next_node = this->_M_next;       // 第一步：保存后继节点
  _List_node_base* const __prev_node = this->_M_prev;       // 第二步：保存前驱节点
  __prev_node->_M_next = __next_node;                       
  __next_node->_M_prev = __prev_node;
}

```

删除所有元素  clear（）

```c++
/**
*  Erases all the elements.  Note that this function only erases
*  the elements, and that if the elements themselves are
*  pointers, the pointed-to memory is not touched in any way.
*  Managing the pointer is the user's responsibility.
*/
void clear() _GLIBCXX_NOEXCEPT {
    _Base::_M_clear();
    _Base::_M_init();
}

template <typename _Tp, typename _Alloc>
void _List_base<_Tp, _Alloc>::_M_clear() _GLIBCXX_NOEXCEPT {
  typedef _List_node<_Tp> _Node;
  _Node* __cur = static_cast<_Node*>(_M_impl._M_node._M_next);
  while (__cur != &_M_impl._M_node) {						// while循环
    _Node* __tmp = __cur;									// 保存节点
    __cur = static_cast<_Node*>(__cur->_M_next);            // 往后遍历
#if __cplusplus >= 201103L
    _M_get_Node_allocator().destroy(__tmp);
#else
    _M_get_Tp_allocator().destroy(std::__addressof(__tmp->_M_data));
#endif
    _M_put_node(__tmp);                                     // 释放内存
  }
}
// 全部指向自己
void _M_init() _GLIBCXX_NOEXCEPT {
    this->_M_impl._M_node._M_next = &this->_M_impl._M_node;
    this->_M_impl._M_node._M_prev = &this->_M_impl._M_node;
}
```

【元素访问】

每个都实现了两个版本：引用与常引用。

- front 返回第一个元素

```c++
reference front() _GLIBCXX_NOEXCEPT { return *begin(); }
const_reference front() const _GLIBCXX_NOEXCEPT { return *begin(); }
```

- 返回最后一个元素

```c++
reference back() _GLIBCXX_NOEXCEPT {
    iterator __tmp = end();
    --__tmp;
    return *__tmp;
}
const_reference back() const _GLIBCXX_NOEXCEPT {
    const_iterator __tmp = end();
    --__tmp;
    return *__tmp;
}
```

还有一些算法类的，后续有时间再看看.....
