

# list.h

list是一个双向链表。

## 节点设计

`_List_node`放数据部分

`_List_node_base`放前后指针

```c
struct _List_node_base
{
    _List_node_base *_M_next;
    _List_node_base *_M_prev;

    static void
        swap(_List_node_base &__x, _List_node_base &__y) _GLIBCXX_USE_NOEXCEPT;

    void
        _M_transfer(_List_node_base *const __first,
                    _List_node_base *const __last) _GLIBCXX_USE_NOEXCEPT;

    void
        _M_reverse() _GLIBCXX_USE_NOEXCEPT;

    void
        _M_hook(_List_node_base *const __position) _GLIBCXX_USE_NOEXCEPT;

    void
        _M_unhook() _GLIBCXX_USE_NOEXCEPT;
};

```

```c
  template <typename _Tp>
  struct _List_node : public __detail::_List_node_base
  {
#if __cplusplus >= 201103L
    __gnu_cxx::__aligned_membuf<_Tp> _M_storage;
    _Tp *_M_valptr() { return _M_storage._M_ptr(); }
    _Tp const *_M_valptr() const { return _M_storage._M_ptr(); }
#else
    _Tp _M_data;
    _Tp *_M_valptr() { return std::__addressof(_M_data); }
    _Tp const *_M_valptr() const { return std::__addressof(_M_data); }
#endif
  };
```

通过 `_M_storage._M_ptr()` 可以获取 `_Tp` 类型对象的指针。

`_M_storage` 和`_M_data`都是数据，在 C++11 之前的版本中，`_List_node` 的实现没有使用 `__aligned_membuf<_Tp>`，而是直接在结构体中定义了一个 `_Tp` 类型的对象。这种实现方式可能会导致 `_List_node` 对象的大小不同，因为不同大小的类型可能需要不同的内存对齐方式。因此，C++11 引入了 `__aligned_membuf`，它能够保证 `_List_node` 对象的大小恒定，并在所有平台上提供正确的内存对齐方式。



## 头节点

```c
    struct _List_node_header : public _List_node_base
    {
      std::size_t _M_size;//大小

      _List_node_header() _GLIBCXX_NOEXCEPT
      {
        _M_init();
      }

      _List_node_header(_List_node_header &&__x) noexcept
          : _List_node_base{__x._M_next, __x._M_prev}  ,_M_size(__x._M_size)
      {
        if (__x._M_base()->_M_next == __x._M_base())
          this->_M_next = this->_M_prev = this;
        else
        {
          this->_M_next->_M_prev = this->_M_prev->_M_next = this->_M_base();
          __x._M_init();
        }
      }

      void
      _M_move_nodes(_List_node_header &&__x) // 用于将 x 对象的所有节点移动到当前 list 对象中。
      {
        _List_node_base *const __xnode = __x._M_base();
        if (__xnode->_M_next == __xnode)
          _M_init();
        else
        {
          _List_node_base *const __node = this->_M_base();
          __node->_M_next = __xnode->_M_next;
          __node->_M_prev = __xnode->_M_prev;
          __node->_M_next->_M_prev = __node->_M_prev->_M_next = __node;
          _M_size = __x._M_size;
            
          __x._M_init();
            // __x只是一个表头，实际上不包含任何数据，可以放心“丢弃” 
        }
      }

      void
      _M_init() _GLIBCXX_NOEXCEPT
      {
        this->_M_next = this->_M_prev = this;
        this->_M_size = 0;
      }

    private:
      _List_node_base *_M_base() { return this; }
    };
```



## 迭代器

```c
  template <typename _Tp>
  struct _List_iterator
  {
    typedef _List_iterator<_Tp> _Self;
    typedef _List_node<_Tp> _Node;

    typedef ptrdiff_t difference_type;
    typedef std::bidirectional_iterator_tag iterator_category;
    typedef _Tp value_type;
    typedef _Tp *pointer;
    typedef _Tp &reference;

    _List_iterator() _GLIBCXX_NOEXCEPT
        : _M_node() {}

    explicit _List_iterator(__detail::_List_node_base *__x) _GLIBCXX_NOEXCEPT
        : _M_node(__x) {}

    _Self
    _M_const_cast() const _GLIBCXX_NOEXCEPT
    {
      return *this;
    }
	/*还有若干重载未展示*/
    __detail::_List_node_base *_M_node;//前后指针
  };
```

疑惑的是数据部分不知道放在哪里了，我们看下他的取值操作：

```c
reference
    operator*() const _GLIBCXX_NOEXCEPT
{
    return *static_cast<_Node *>(_M_node)->_M_valptr();
}

pointer
    operator->() const _GLIBCXX_NOEXCEPT
{
    return static_cast<_Node *>(_M_node)->_M_valptr();
}

```

发现是使用基类对象调用子类方法获取到数据，说实话我还很难理解是怎么到这一步的，但确实是这样设计了

再看几个运算符重载加深下理解，还有没列举到的就不深究了：

```c
// ++i
_Self &
operator++() _GLIBCXX_NOEXCEPT
{
    _M_node = _M_node->_M_next;
    return *this;
}
// i++
_Self
operator++(int) _GLIBCXX_NOEXCEPT
{
    _Self __tmp = *this;
    _M_node = _M_node->_M_next;
    return __tmp;
}

friend bool
operator==(const _Self &__x, const _Self &__y) _GLIBCXX_NOEXCEPT
{
	return __x._M_node == __y._M_node;
}
```

还有一个 `struct _List_const_iterator` , 实现基本一致

```c
template <typename _Tp>
    struct _List_const_iterator
    {
        typedef _List_const_iterator<_Tp> _Self;
        typedef const _List_node<_Tp> _Node;
        typedef _List_iterator<_Tp> iterator;

        typedef ptrdiff_t difference_type;
        typedef std::bidirectional_iterator_tag iterator_category;
        typedef _Tp value_type;
        typedef const _Tp *pointer;
        typedef const _Tp &reference;

        const __detail::_List_node_base *_M_node;
    }
```



## list_base

```c
class _List_base
{
protected:
    typedef typename __gnu_cxx::__alloc_traits<_Alloc>::template rebind<_Tp>::other 
        _Tp_alloc_type;
    typedef typename _Tp_alloc_traits::template rebind<_List_node<_Tp>>::other 
        _Node_alloc_type;
    
    struct _List_impl
        : public _Node_alloc_type
    {
      	__detail::_List_node_header _M_node;
	};

    _List_impl _M_impl;
    
public:
    typedef _Alloc allocator_type;
    
    _List_base(const _Node_alloc_type &__a) _GLIBCXX_NOEXCEPT
        : _M_impl(__a)
    {
    }
    
    // Used when allocator is_always_equal.
    _List_base(_Node_alloc_type &&__a, _List_base &&__x)
        : _M_impl(std::move(__a), std::move(__x._M_impl))
    {
    }

    // Used when allocator !is_always_equal.
    _List_base(_Node_alloc_type &&__a)
        : _M_impl(std::move(__a))
    {
    }
};

```

我看不懂，总之就是`base` 提供了各种版本的构造函数和方法，`impl`封装了 `std::list` 中的链表实现细节，管理链表中元素的插入、删除、移动等操作，以及提供迭代器的实现。



## list

这个里面也是一堆看着头痛的`typedef`, 和不知道在干什么的构造函数，还有很多`list`容器的方法，比如增删改查

构造函数

```c
 template<typename _Tp, typename _Alloc = std::allocator<_Tp> >
 class list : protected _List_base<_Tp, _Alloc> {}
```

`std::allocator`是C++标准库提供的一个内存分配器，用于在堆上动态分配和释放内存。它是一个泛型类，可以作为容器类（如`std::vector`、`std::list`等）的默认分配器。使用`std::allocator`可以避免手动管理内存分配和释放，从而降低代码的复杂性和出错的可能性。

`_Tp`表示链表节点存储的数据类型，`_Alloc`表示分配器类型，它默认为`std::allocator<_Tp>`，即使用默认的分配器进行内存分配和释放。

`std::list`继承自`_List_base<_Tp, _Alloc>`，`_List_base`是一个模板类，定义了链表节点的基本结构和相关操作，包括节点指针、链表大小等。由于`_List_base`是一个实现细节，因此被保护起来，不能被外部访问。



### 增_push_back()

```c
void push_back(const value_type &__x) { this->_M_insert(end(), __x); }
```

`M_insert` 

是将x插入到指定位置之上，而不是指定位置之后，可以理解为前插

```c
void _M_insert(iterator __position, const value_type &__x) {
    _Node *__tmp = _M_create_node(__x); //总之，就是创建了一个节点
    __tmp->_M_hook(__position._M_node);
}
```

`_M_create_node` 

```c
_Node *
    _M_create_node(const value_type &__x)
{
    _Node *__p = this->_M_get_node();
    __try
    {
        _Tp_alloc_type __alloc(_M_get_Node_allocator());//这里不知道在干什么，标记一下
        __alloc.construct(__p->_M_valptr(), __x);
    }
    __catch(...)
    {
        _M_put_node(__p);
        __throw_exception_again;
    }
    return __p;
}
```

`M_hook`

`this`插入到`pos`前面

```c
void_List_node_base::_M_hook(_List_node_base* const __position) _GLIBCXX_USE_NOEXCEPT
{
  this->_M_next = __position;		
  this->_M_prev = __position->_M_prev;
  __position->_M_prev->_M_next = this;
  __position->_M_prev = this;
}
```

### 增_push_front()

```c
void push_front(const value_type &__x) {
    this->_M_insert(begin(), __x);
}
```

### 删_pop_back()

```c
void pop_back() _GLIBCXX_NOEXCEPT {
    this->_M_erase(iterator(this->_M_impl._M_node._M_prev));
}
```

`_M_erase`

```c
void
    _M_erase(iterator __position) _GLIBCXX_NOEXCEPT
{
    this->_M_dec_size(1);
    __position._M_node->_M_unhook();	//从list删除节点，然后释放资源
    _Node *__n = static_cast<_Node *>(__position._M_node);
    _Tp_alloc_type(_M_get_Node_allocator()).destroy(__n->_M_valptr());
    _M_put_node(__n);
}
```

`_M_unhook`

删除this

```c
void _List_node_base::_M_unhook() _GLIBCXX_USE_NOEXCEPT
{
  _List_node_base* const __next_node = this->_M_next;
  _List_node_base* const __prev_node = this->_M_prev;
  __prev_node->_M_next = __next_node
  __next_node->_M_prev = __prev_node;
}
```

### 删_pop_front()

```c
void pop_front() _GLIBCXX_NOEXCEPT {
    this->_M_erase(begin());
}
```

### 删_clear()

```c++
void clear() _GLIBCXX_NOEXCEPT {
    _Base::_M_clear();	//清空
    _Base::_M_init();	//初始化
}
```

`_M_clear`

```c++
_List_base<_Tp, _Alloc>::
_M_clear() _GLIBCXX_NOEXCEPT
{
  typedef _List_node<_Tp>  _Node;
  _Node* __cur = static_cast<_Node*>(_M_impl._M_node._M_next);
  while (__cur != &_M_impl._M_node)
    {
      _Node* __tmp = __cur;			// 保存节点
      __cur = static_cast<_Node*>(__cur->_M_next);	// 往后遍历
#if __cplusplus >= 201103L
      _M_get_Node_allocator().destroy(__tmp);
#else
      _M_get_Tp_allocator().destroy(std::__addressof(__tmp->_M_data));
#endif
      _M_put_node(__tmp);	// 释放内存
    }
}
```

`_M_init`

```c++
void _M_init()
_GLIBCXX_NOEXCEPT
{
    this->_M_impl._M_node._M_next = &this->_M_impl._M_node;
    this->_M_impl._M_node._M_prev = &this->_M_impl._M_node;
}
```

### 查_front()\|back()

```c
reference front() _GLIBCXX_NOEXCEPT
{
    return *begin();
}

reference  back() _GLIBCXX_NOEXCEPT
{
    iterator __tmp = end();
    --__tmp;
    return *__tmp;
}
```



还有很多操作，看不过来了