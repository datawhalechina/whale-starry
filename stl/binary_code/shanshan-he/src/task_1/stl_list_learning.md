list容器



```
_GLIBCXX_END_NAMESPACE_VERSION
```

"_GLIBCXX_END_NAMESPACE_VERSION" 是用于结束命名空间的版本定义的宏。它通常与其他命名空间相关的宏一起使用，用于指定特定版本的命名空间，以避免命名空间冲突和版本间的兼容性问题。



## 链表节点结构体

```c++
// C++ 的模板结构体 _List_node 的定义
    struct _List_node_base
    {
      _List_node_base* _M_next;
      _List_node_base* _M_prev;

      static void
      swap(_List_node_base& __x, _List_node_base& __y) _GLIBCXX_USE_NOEXCEPT;

      void
      _M_transfer(_List_node_base* const __first,
		  _List_node_base* const __last) _GLIBCXX_USE_NOEXCEPT;

      void
      _M_reverse() _GLIBCXX_USE_NOEXCEPT;

      void
      _M_hook(_List_node_base* const __position) _GLIBCXX_USE_NOEXCEPT;

      void
      _M_unhook() _GLIBCXX_USE_NOEXCEPT;
    };

// An actual node in the %list.
// C++ 的模板结构体 _List_node 的定义

  template<typename _Tp>
    struct _List_node : public __detail::_List_node_base
    {
      ///< User's data.
      _Tp _M_data;

#if __cplusplus >= 201103L
      template<typename... _Args>
        _List_node(_Args&&... __args)
	: __detail::_List_node_base(), _M_data(std::forward<_Args>(__args)...) 
        { }
#endif
    };
    

```

该结构体继承自 `_List_node_base`，并包含一个成员变量 `_M_data`，用于存储用户的数据。

在 C++11 及以上版本中，该结构体还定义了一个带有可变参数模板的构造函数。该构造函数使用了完美转发（perfect forwarding）来接收参数，并将其传递给成员变量 `_M_data` 进行初始化。这样可以实现在创建 `_List_node` 对象时传递不同类型和数量的参数，使得构造函数更加灵活。

## 创建迭代器iterator

```c++
template<typename _Tp>
    struct _List_iterator
    {
      typedef _List_iterator<_Tp>                _Self;
      typedef _List_node<_Tp>                    _Node;

      typedef ptrdiff_t                          difference_type;
      typedef std::bidirectional_iterator_tag    iterator_category;
      typedef _Tp                                value_type;
      typedef _Tp*                               pointer;
      typedef _Tp&                               reference;

      _List_iterator() _GLIBCXX_NOEXCEPT
      : _M_node() { }   //指向链表元素的指针

      explicit
      _List_iterator(__detail::_List_node_base* __x) _GLIBCXX_NOEXCEPT
      : _M_node(__x) { }  //指向链表元素的指针。

      _Self
      _M_const_cast() const _GLIBCXX_NOEXCEPT
      { return *this; }   //将迭代器转换为常量迭代器。

      // Must downcast from _List_node_base to _List_node to get to _M_data.
      reference
      //解引用操作符，返回迭代器指向的元素的引用。
      operator*() const _GLIBCXX_NOEXCEPT 
      { return static_cast<_Node*>(_M_node)->_M_data; }

      pointer
      //成员访问操作符，返回迭代器指向的元素的指针
      operator->() const _GLIBCXX_NOEXCEPT
      { return std::__addressof(static_cast<_Node*>(_M_node)->_M_data); }
	  //前置递增操作符，将迭代器向前移动一个位置，并返回递增后的迭代器。
      _Self&
      operator++() _GLIBCXX_NOEXCEPT
      {
	_M_node = _M_node->_M_next;
	return *this;
      }
        
       //后置递增操作符，将迭代器向前移动一个位置，并返回递增前的迭代器。
      _Self
      operator++(int) _GLIBCXX_NOEXCEPT
      {
	_Self __tmp = *this;
	_M_node = _M_node->_M_next;
	return __tmp;
      }
      //前置递减操作符，将迭代器向后移动一个位置，并返回递减后的迭代器。
      _Self&
      operator--() _GLIBCXX_NOEXCEPT
      {
	_M_node = _M_node->_M_prev;
	return *this;
      }
       //后置递减操作符，将迭代器向后移动一个位置，并返回递减前的迭代器。
      _Self
      operator--(int) _GLIBCXX_NOEXCEPT
      {
	_Self __tmp = *this;
	_M_node = _M_node->_M_prev;
	return __tmp;
      }
      //相等比较操作符，比较两个迭代器是否指向同一个节点。
      bool
      operator==(const _Self& __x) const _GLIBCXX_NOEXCEPT
      { return _M_node == __x._M_node; }
      
      //不等比较操作符，比较两个迭代器是否指向不同的节点。
      bool
      operator!=(const _Self& __x) const _GLIBCXX_NOEXCEPT
      { return _M_node != __x._M_node; }

      // The only member points to the %list element.
      __detail::_List_node_base* _M_node;
    };
```



## list详细说明

这是一个双向链表。沿着链表向上和向下遍历需要线性时间，但是无论更改发生在哪个位置，添加和删除元素（或节点）都以常数时间完成。与 std::vector 和 std::deque 不同，不提供随机访问迭代器，因此不允许使用下标访问（@c []）。对于只需要顺序访问的算法，这种缺乏没有影响。

list参数：

```c++
class list : protected _List_base<_Tp, _Alloc>
    {
      // concept requirements
      typedef typename _Alloc::value_type                _Alloc_value_type;
      __glibcxx_class_requires(_Tp, _SGIAssignableConcept)
      __glibcxx_class_requires2(_Tp, _Alloc_value_type, _SameTypeConcept)

      typedef _List_base<_Tp, _Alloc>                    _Base;
      typedef typename _Base::_Tp_alloc_type		 _Tp_alloc_type;
      typedef typename _Base::_Node_alloc_type		 _Node_alloc_type;

    public:
      typedef _Tp                                        value_type;
      typedef typename _Tp_alloc_type::pointer           pointer;
      typedef typename _Tp_alloc_type::const_pointer     const_pointer;
      typedef typename _Tp_alloc_type::reference         reference;
      typedef typename _Tp_alloc_type::const_reference   const_reference;
      typedef _List_iterator<_Tp>                        iterator;
      typedef _List_const_iterator<_Tp>                  const_iterator;
      typedef std::reverse_iterator<const_iterator>      const_reverse_iterator;
      typedef std::reverse_iterator<iterator>            reverse_iterator;
      typedef size_t                                     size_type;
      typedef ptrdiff_t                                  difference_type;
      typedef _Alloc                                     allocator_type;

    protected:
      // Note that pointers-to-_Node's can be ctor-converted to
      // iterator types.
      typedef _List_node<_Tp>				 _Node;

      using _Base::_M_impl;
      using _Base::_M_put_node;
      using _Base::_M_get_node;
      using _Base::_M_get_Tp_allocator;
      using _Base::_M_get_Node_allocator;

```





## 主要函数梳理

1、创建node

```c++
#if __cplusplus < 201103L
      _Node*
      _M_create_node(const value_type& __x)
      {
	_Node* __p = this->_M_get_node();
	__try
	  {
	    _M_get_Tp_allocator().construct
	      (std::__addressof(__p->_M_data), __x);
	  }
	__catch(...)
	  {
	    _M_put_node(__p);
	    __throw_exception_again;
	  }
	return __p;
      }
#else
      template<typename... _Args>
        _Node*
        _M_create_node(_Args&&... __args)
	{
	  _Node* __p = this->_M_get_node();
	  __try
	    {
	      _M_get_Node_allocator().construct(__p,
						std::forward<_Args>(__args)...);
	    }
	  __catch(...)
	    {
	      _M_put_node(__p);
	      __throw_exception_again;
	    }
	  return __p;
	}
```

定义了 `_M_create_node` 函数，用于创建 `_Node` 节点对象，并进行值的构造。这个函数在 `list` 容器的内部被使用，用于在插入元素时创建新的节点。

对于 C++11 之前的版本，`_M_create_node` 接受一个值参数 `__x`，并使用 `_M_get_Tp_allocator()` 获取的分配器对象构造节点的值成员 `_M_data`。

而对于 C++11 及更新的版本，`_M_create_node` 是一个模板函数，接受可变参数模板 `Args`，可以传递多个参数用于构造节点的值成员 `_M_data`。这样可以更灵活地使用不同类型和数量的参数进行构造。

2、构造函数的实现部分

```C++
  /**
       *  @brief  Creates a %list with no elements.
       */
      list()
#if __cplusplus >= 201103L
      noexcept(is_nothrow_default_constructible<_Node_alloc_type>::value)
#endif
      : _Base() { }

      /**
       *  @brief  Creates a %list with no elements.
       *  @param  __a  An allocator object.
       */
      explicit
      list(const allocator_type& __a) _GLIBCXX_NOEXCEPT
      : _Base(_Node_alloc_type(__a)) { }
```

第一个构造函数是默认构造函数，它创建一个空的链表。根据编译器对 C++ 标准的支持情况进行了条件编译。如果编译器版本支持 C++11 及更新的标准（`__cplusplus >= 201103L`），则使用 `noexcept` 关键字标记为默认构造函数不会抛出异常。否则，没有条件编译的部分不包含 `noexcept`。

第二个构造函数是带有分配器参数的构造函数，它创建一个空的链表，并使用给定的分配器对象进行内存分配。这个构造函数使用了 `explicit` 关键字，表示它只能显式地调用来创建链表对象。

```c++
#if __cplusplus >= 201103L
      template<typename _InputIterator,
	       typename = std::_RequireInputIter<_InputIterator>>
        list(_InputIterator __first, _InputIterator __last,
	     const allocator_type& __a = allocator_type())
	: _Base(_Node_alloc_type(__a))
        { _M_initialize_dispatch(__first, __last, __false_type()); }
#else
      template<typename _InputIterator>
        list(_InputIterator __first, _InputIterator __last,
	     const allocator_type& __a = allocator_type())
	: _Base(_Node_alloc_type(__a))
        { 
	  // Check whether it's an integral type.  If so, it's not an iterator.
	  typedef typename std::__is_integer<_InputIterator>::__type _Integral;
	  _M_initialize_dispatch(__first, __last, _Integral());
	}
```

如果编译器版本支持 C++11 及更新的标准（`__cplusplus >= 201103L`），则定义了一个模板构造函数。这个构造函数接受两个迭代器参数 `__first` 和 `__last`，以及一个可选的分配器参数 `__a`。它使用 `_M_initialize_dispatch` 函数来初始化链表容器，并根据迭代器类型的特性选择适当的初始化方式。

如果编译器版本不支持 C++11 及更新的标准，即旧版本的 C++（`__cplusplus < 201103L`），则定义了另一个构造函数。这个构造函数同样接受两个迭代器参数 `__first` 和 `__last`，以及一个可选的分配器参数 `__a`。在这种情况下，它通过检查迭代器是否是整数类型来判断是否为有效的迭代器，并使用 `_M_initialize_dispatch` 函数来初始化链表容器。

3、分配值给列表

```c++
      void
      assign(size_type __n, const value_type& __val)
      { _M_fill_assign(__n, __val); }
```

-  将给定的值赋给列表。
- param __n 要赋值的元素数量。
- param __val 要赋的值。
- 此函数使用 @a __val 的 @a __n 个副本填充列表。请注意，赋值操作完全改变了列表，并且结果列表的大小与分配的元素数量相同。旧数据可能会丢失。

4、返回链表的内存分配器对象

```c++
      allocator_type
      get_allocator() const _GLIBCXX_NOEXCEPT
      { return _Base::get_allocator(); }
```

该函数返回链表使用的分配器对象的副本。

5、指向链表中的第一个元素

```c++
     iterator
      begin() _GLIBCXX_NOEXCEPT
      { return iterator(this->_M_impl._M_node._M_next); }
```

```c++
     const_iterator
      begin() const _GLIBCXX_NOEXCEPT
      { return const_iterator(this->_M_impl._M_node._M_next); }
```

`begin() const`: 返回一个只读（常量）迭代器，指向链表中的第一个元素。迭代按照普通的元素顺序进行。

6、指向链表中最后一个元素之后的位

```C++
      iterator
      end() _GLIBCXX_NOEXCEPT
      { return iterator(&this->_M_impl._M_node); }
```

```c++
      const_iterator
      end() const _GLIBCXX_NOEXCEPT
      { return const_iterator(&this->_M_impl._M_node); }
```

`end() const`: 返回一个只读（常量）迭代器，指向链表中最后一个元素之后的位置。迭代按照普通的元素顺序进行。

7、返回对链表中第一个元素的读写引用

```c++
      reference
      front() _GLIBCXX_NOEXCEPT
      { return *begin(); }

      /**
       *  Returns a read-only (constant) reference to the data at the first
       *  element of the %list.
       */
      const_reference
      front() const _GLIBCXX_NOEXCEPT
      { return *begin(); }

```

通过调用 `begin()` 函数获取链表的第一个元素的迭代器，然后通过解引用操作符 `*` 返回该元素的引用。

`front() const`: 返回对链表中第一个元素的只读引用。与上述 `front()` 函数类似，但返回的是只读引用。

8、返回对链表中最后一个元素的读写引用

```c++
     reference
      back() _GLIBCXX_NOEXCEPT
      { 
	iterator __tmp = end();
	--__tmp;
	return *__tmp;
      }

      /**
       *  Returns a read-only (constant) reference to the data at the last
       *  element of the %list.
       */
      const_reference
      back() const _GLIBCXX_NOEXCEPT
      { 
	const_iterator __tmp = end();
	--__tmp;
	return *__tmp;
```

通过调用 `end()` 函数获取链表的最后一个元素之后的位置的迭代器，然后通过减小迭代器的操作 `--` 移动到最后一个元素，并返回该元素的引用。

 `back() const `与之前描述的 `back()` 函数类似，不同之处在于返回的是只读引用，即不能修改最后一个元素的值。

9、插入函数，插入到链表的末尾

```c++
      void
      push_back(const value_type& __x)
      { this->_M_insert(end(), __x); }
```

`push_back()` 函数用于将给定的值 `__x` 插入到链表的末尾。

函数通过调用 `_M_insert()` 函数，在链表的末尾位置（通过 `end()` 返回的迭代器位置）插入新的元素。该函数将会在末尾位置之前插入一个新节点，并将值 `__x` 赋给新节点的数据。

10、删除链表的最后一个元素

```c++
     void
      pop_back() _GLIBCXX_NOEXCEPT
      { this->_M_erase(iterator(this->_M_impl._M_node._M_prev)); }
```

函数通过调用 `_M_erase()` 函数，删除位于链表末尾（通过 `end()` 返回的迭代器位置的前一个位置）的节点。该函数会将该节点从链表中移除，并释放相关的内存。