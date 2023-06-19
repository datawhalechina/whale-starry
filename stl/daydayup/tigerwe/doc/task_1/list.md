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

实现类 _List_impl，主要用来维护链表节点，然后list类包含该类

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

_List_base类

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

list类  只截取可以体现list代码结构的一部分，具体接口实现看源码！

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

