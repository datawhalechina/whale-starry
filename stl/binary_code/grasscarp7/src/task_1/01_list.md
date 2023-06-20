# 一、知识点

## 1. std::bidirectional_iterator_tag

`std::bidirectional_iterator_tag` 是 C++ 标准库中定义的一个迭代器类型标签，用于标识支持双向遍历的迭代器类型。

在 C++ 中，迭代器是一种泛型指针，用于遍历容器中的元素。迭代器类型标签用于标识迭代器的特性，从而在算法中选择合适的迭代器类型。

`std::bidirectional_iterator_tag` 是迭代器类型标签中的一种，用于标识支持双向遍历的迭代器类型。双向迭代器可以向前和向后遍历容器中的元素，支持 `++` 和 `--` 运算符。

标准库中的许多算法都要求迭代器支持特定的操作，例如 `std::reverse` 要求迭代器支持双向遍历，因此可以使用 `std::bidirectional_iterator_tag` 标签来限定迭代器的类型，从而保证算法的正确性。

```cpp
//迭代器类型标签，用于标识支持双向遍历的迭代器类型，支持++和--运算符
typedef std::bidirectional_iterator_tag    iterator_category;
```



## 2. ptrdiff_t

`ptrdiff_t` 是 C++ 标准库中定义的一个整型类型，用于表示指针之间的差值（即两个指针在内存中的地址差距）。在 C++ 中，指针的加减运算结果是一个 `ptrdiff_t` 类型的值。

`ptrdiff_t` 的实现是平台相关的，通常是一个带符号的整数类型。在 32 位平台上，`ptrdiff_t` 通常是一个 4 字节的整型，而在 64 位平台上，`ptrdiff_t` 通常是一个 8 字节的整型。

使用 `ptrdiff_t` 类型可以保证指针操作的安全性，因为指针之间的差值可能超出普通整型的表示范围。此外，使用 `ptrdiff_t` 类型还可以提高代码的可移植性，因为不同平台上指针的大小可能不同。

```cpp
//带符号整型，表示指针之间的差值（即两个指针在内存中的地址差距）
typedef ptrdiff_t                          difference_type;
```



## 3. #if __cplusplus >= 201103L

`#if __cplusplus >= 201103L`这行代码是C++中的条件编译指令，意思是如果当前编译器支持C++11标准或更高版本，则编译下面的代码。其中，`__cplusplus`是一个C++预定义宏，表示当前编译器所支持的C++标准版本，`201103L`表示C++11标准的版本号。因此，这行代码的作用是在编译时检查当前编译器是否支持C++11标准或更高版本，如果支持，则编译下面的代码，否则忽略。

## 4. explicit

`explicit` 是 C++ 中的一个关键字，用于修饰类的构造函数，表示该构造函数是显式的，不能进行隐式转换。

在 C++ 中，如果一个类的构造函数只有一个参数，那么这个构造函数可以被用于隐式转换。例如，如果有一个类 `A` 和一个构造函数 `A(int)`，那么可以使用 `A a = 1;` 的方式创建一个 `A` 类型的对象，这里的 `1` 会被自动转换为 `A` 类型。

使用 `explicit` 关键字可以禁止这种隐式转换，从而避免一些潜在的问题。例如，如果一个类 `B` 有一个构造函数 `B(int)`，但是我们希望这个构造函数只能显式调用，那么可以将其声明为 `explicit`，这样就不能再使用 `B b = 1;` 的方式创建一个 `B` 类型的对象，而必须显式地调用 `B b(1);`。

另外，`explicit` 关键字还可以用于修饰转换函数，表示该函数也是显式的，不能进行隐式转换。

## 5. stl_list.h结构

![image-20230619171128863](https://images.weserv.nl/?url=https://article.biliimg.com/bfs/article/e492d702bf926316534a6bd6d549f653f27b316d.png)

## 6. 迭代器相关



![image-20230619165831457](https://images.weserv.nl/?url=https://article.biliimg.com/bfs/article/588ee35bccc8d19e06032dab22e58b50929dba9b.png)

![image-20230619170323354](https://images.weserv.nl/?url=https://article.biliimg.com/bfs/article/9b53397719315b1eefa13f94a73f63d26b649e25.png)

![image-20230619170740384](https://images.weserv.nl/?url=https://article.biliimg.com/bfs/article/6c1304535fc4200af0aa75b9a8d86cc042d6b651.png)

## 7. 迭代器 traits

![image-20230619210901175](https://images.weserv.nl/?url=https://article.biliimg.com/bfs/article/d557e6cba943c3a8f91bac1fe34c15f0a80fa988.png)

![image-20230619210958775](https://images.weserv.nl/?url=https://article.biliimg.com/bfs/article/418664d10f0ed6673630c024dac8270809a28a62.png)



# 二、源码

## _List_node_base

主要提供了prev和next指针，以及一些方法。

```cpp
struct _List_node_base
{
      _List_node_base* _M_next;
      _List_node_base* _M_prev;
      ...
};
```

## _List_node

继承自_List_node_base，主要提供了一个存放数据的 _M_data

```cpp
/// An actual node in the %list.
  template<typename _Tp>
    struct _List_node : public __detail::_List_node_base
    {
      ///< User's data.
      _Tp _M_data;
//检查编译器是否支持c++11及以上版本
#if __cplusplus >= 201103L
      template<typename... _Args>
        _List_node(_Args&&... __args)     //万能引用
	: __detail::_List_node_base(), _M_data(std::forward<_Args>(__args)...) //完美转发
        { }
#endif
    };
```

## _List_iterator

迭代器

### 1. 必须定义的五个typedef

```cpp
      //下面这五个typedef是每个iterator必须有的
      //带符号整型，表示指针之间的差值（即两个指针在内存中的地址差距）
      typedef ptrdiff_t                          difference_type;
      //迭代器类型标签，用于标识支持双向遍历的迭代器类型，支持++和--运算符
      typedef std::bidirectional_iterator_tag    iterator_category;
      typedef _Tp                                value_type;
      typedef _Tp*                               pointer;
      typedef _Tp&                               reference;
```

### 2. 两个构造器

```cpp
      _List_iterator() _GLIBCXX_NOEXCEPT
      : _M_node() { }

      explicit //修饰类的构造函数，表示该构造函数是显式的，不能进行隐式转换
      _List_iterator(__detail::_List_node_base* __x) _GLIBCXX_NOEXCEPT
      : _M_node(__x) { }      //_M_node:_List_node_base
```

### 3.  运算符重载

个人理解，此处之所以能够向下造型，应该是传入参数的时候已经做过一次向上造型

```cpp
// Must downcast from _List_node_base to _List_node to get to _M_data.
      reference
      operator*() const _GLIBCXX_NOEXCEPT 
      { return static_cast<_Node*>(_M_node)->_M_data; }     //_M_node:_List_node_base, _Node:_List_node<_Tp>

      pointer
      operator->() const _GLIBCXX_NOEXCEPT      //2.9版本中是return &(operator*())
      { return std::__addressof(static_cast<_Node*>(_M_node)->_M_data); }      //__addressof用于取地址

```



前++运算符与后++运算符

此处模仿了整数的++i与i++，所以前++运算符需要返回一个引用。

个人理解：与左值和右值有关，如++i是左值，i++是右值。

```cpp
	  _Self&
      operator++() _GLIBCXX_NOEXCEPT      //++item
      {
	_M_node = _M_node->_M_next;
	return *this;	//由于只有一个成员变量_M_node，因此能返回*this
      }

      _Self
      operator++(int) _GLIBCXX_NOEXCEPT   //item++ 感觉跟左值右值有关
      {
	_Self __tmp = *this;
	_M_node = _M_node->_M_next;
	return __tmp;
      }
```

## _List_base

### 1. typedef

```cpp
	  //rebind是一个模板类，它接受一个模板参数T和一个新的类型U，然后定义一个新的类型
      //这个新的类型是将T中的模板参数全部替换为U后得到的结果
      typedef typename _Alloc::template rebind<_List_node<_Tp> >::other
        _Node_alloc_type;

      typedef typename _Alloc::template rebind<_Tp>::other _Tp_alloc_type;
```

### 2. _List_impl

> 这段代码定义了一个结构体 `_List_impl`，它是 `list` 类的一个内部实现，用于存储 `list` 对象中的元素和节点信息。
>
> 该结构体中使用了两个模板别名：`_Node_alloc_type` 和 `_Tp_alloc_type`，它们分别表示节点和元素的分配器类型。这里使用了 `typename` 关键字来指示 `_Alloc::template rebind<_List_node<_Tp> >::other` 和 `_Alloc::template rebind<_Tp>::other` 是类型别名，而不是静态成员变量或函数。
>
> 接下来，`_List_impl` 结构体中定义了一个 `_M_node` 成员变量，它是一个 `_List_node_base` 类型的对象，用于存储 `list` 对象中的头节点和尾节点信息。
>
> `_List_impl` 结构体还有三个构造函数，分别用于构造一个空的 `_List_impl` 对象、使用指定的节点分配器构造 `_List_impl` 对象，以及使用右值引用的节点分配器构造 `_List_impl` 对象。
>
> 需要注意的是，`_List_impl` 结构体中的 `_Node_alloc_type` 和 `_Tp_alloc_type` 类型别名都是使用 `_Alloc` 类型的 `template rebind` 成员模板生成的，这是因为 `list` 类需要支持自定义分配器，因此需要使用 `rebind` 将原始分配器重新绑定到节点和元素类型上。

```cpp
      struct _List_impl
      : public _Node_alloc_type
      {
	__detail::_List_node_base _M_node;

	_List_impl()
	: _Node_alloc_type(), _M_node()
	{ }

	_List_impl(const _Node_alloc_type& __a) _GLIBCXX_NOEXCEPT   //底层const
	: _Node_alloc_type(__a), _M_node()
	{ }

#if __cplusplus >= 201103L
	_List_impl(_Node_alloc_type&& __a) _GLIBCXX_NOEXCEPT  //右值引用
	: _Node_alloc_type(std::move(__a)), _M_node()   //转为右值，移动语义
	{ }
#endif
      };
```

### 3. 构造函数

> 这是 `list` 类的一个基类 `_List_base`，包含了 `list` 类的一些基本实现。这里的三个构造函数都是 `_List_base` 类的构造函数。
>
> 第一个构造函数 ` _List_base()` 是默认构造函数，它使用默认的节点分配器构造了一个 `_List_impl` 对象 `_M_impl`，并调用了 `_M_init()` 函数来初始化 `_List_base` 对象。
>
> 第二个构造函数 `_List_base(const _Node_alloc_type& __a)` 则接受一个节点分配器 `__a`，用于构造一个 `_List_impl` 对象 `_M_impl`，并调用了 `_M_init()` 函数来初始化 `_List_base` 对象。
>
> 第三个构造函数 `_List_base(_List_base&& __x)` 是移动构造函数，它接受一个右值引用 `__x`，用于构造一个 `_List_impl` 对象 `_M_impl`。在构造过程中，它使用 `__x._M_get_Node_allocator()` 获取 `__x` 对象的节点分配器，并将其作为参数传递给 `_M_impl` 的构造函数，从而实现了节点分配器的移动。接着，它调用了 `_M_init()` 函数来初始化 `_List_base` 对象，并使用 `__detail::_List_node_base::swap` 函数交换了 `_M_impl._M_node` 和 `__x._M_impl._M_node` 两个节点的信息，实现了 `_List_base` 对象的移动构造。需要注意的是，该构造函数只在 C++11 及以上版本中被定义。

```cpp
      _List_base()
      : _M_impl()
      { _M_init(); }

      _List_base(const _Node_alloc_type& __a) _GLIBCXX_NOEXCEPT
      : _M_impl(__a)
      { _M_init(); }

#if __cplusplus >= 201103L
      _List_base(_List_base&& __x) noexcept     //右值引用
      : _M_impl(std::move(__x._M_get_Node_allocator()))     //移动构造函数
      {
	_M_init();
	__detail::_List_node_base::swap(_M_impl._M_node, __x._M_impl._M_node);
      }
#endif
```

### 4. _M_init()

```cpp
	  void
      _M_init() _GLIBCXX_NOEXCEPT
      {
        //头节点的prev与next指向自身
        this->_M_impl._M_node._M_next = &this->_M_impl._M_node;
        this->_M_impl._M_node._M_prev = &this->_M_impl._M_node;
      }
```

## list

### 1. typedef

```cpp
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

### 2. 一些函数

#### _M_create_node

```cpp
      template<typename... _Args>
        _Node*
        _M_create_node(_Args&&... __args)	//万能引用
	{
	  _Node* __p = this->_M_get_node();       //获取新的节点指针
	  __try
	    {
	      _M_get_Node_allocator().construct(__p,
						std::forward<_Args>(__args)...); //使用完美转发构造节点中的数据
	    }
	  __catch(...)
	    {
	      _M_put_node(__p);
	      __throw_exception_again;
	    }
	  return __p;	//返回指针
	}
```

#### assign

> `assign` 函数是 `list` 类的成员函数，用于将新的元素赋值给当前的 `list` 对象。该函数有多个重载版本，其中一个版本接受一个初始化列表作为参数，用于将初始化列表中的元素赋值给当前的 `list` 对象。
>
> 具体地，该函数接受一个初始化列表 `__l`，它将 `__l` 中的元素替换当前 `list` 对象中的元素。为此，它调用另一个 `assign` 函数，该函数接受两个迭代器参数，分别指向初始化列表的第一个元素和最后一个元素。`assign` 函数用这些元素来替换当前 `list` 对象中的元素。
>
> 需要注意的是，该函数只在 C++11 及以上版本中被定义。

```cpp
#if __cplusplus >= 201103L
      /**
       *  @brief  Assigns an initializer_list to a %list.
       *  @param  __l  An initializer_list of value_type.
       *
       *  Replace the contents of the %list with copies of the elements
       *  in the initializer_list @a __l.  This is linear in __l.size().
       */
      void
      assign(initializer_list<value_type> __l)
      { this->assign(__l.begin(), __l.end()); }
#endif
```



### 3. 构造函数

> 第一个构造函数 `list()` 是默认构造函数，它创建一个空的 `list` 对象，使用默认的节点分配器。
>
> 第二个构造函数 `list(const allocator_type& __a)` 接受一个节点分配器 `__a`，用于创建一个空的 `list` 对象。
>
> 第三个构造函数 `list(size_type __n)` 接受一个整数参数 `__n`，用于创建一个包含 `__n` 个默认构造的元素的 `list` 对象。
>
> 第四个构造函数 `list(size_type __n, const value_type& __value, const allocator_type& __a = allocator_type())` 接受一个整数参数 `__n` 和一个元素值 `__value`，用于创建一个包含 `__n` 个值为 `__value` 的元素的 `list` 对象。
>
> 第五个构造函数 `list(const list& __x)` 是拷贝构造函数，用于创建一个与 `__x` 相同的 `list` 对象。
>
> 第六个构造函数 `list(list&& __x) noexcept` 是移动构造函数，用于创建一个与 `__x` 相同的 `list` 对象，并将 `__x` 中的元素移动到新的 `list` 对象中。
>
> 第七个构造函数 `list(initializer_list<value_type> __l, const allocator_type& __a = allocator_type())` 接受一个初始化列表 `__l`，用于创建一个包含 `__l` 中元素的 `list` 对象。
>
> 第八个构造函数 `list(_InputIterator __first, _InputIterator __last, const allocator_type& __a = allocator_type())` 接受两个迭代器参数 `__first` 和 `__last`，用于创建一个包含从 `__first` 到 `__last` 的所有元素的 `list` 对象。需要注意的是，该构造函数只在 C++11 及以上版本中被定义。

```cpp
list()
#if __cplusplus >= 201103L
      noexcept(is_nothrow_default_constructible<_Node_alloc_type>::value) //判断是否支持默认构造函数
#endif
      : _Base() { }

      /**
       *  @brief  Creates a %list with no elements.
       *  @param  __a  An allocator object.
       */
      explicit
      list(const allocator_type& __a) _GLIBCXX_NOEXCEPT
      : _Base(_Node_alloc_type(__a)) { }

#if __cplusplus >= 201103L
      /**
       *  @brief  Creates a %list with default constructed elements.
       *  @param  __n  The number of elements to initially create.
       *
       *  This constructor fills the %list with @a __n default
       *  constructed elements.
       */
      explicit
      list(size_type __n)
      : _Base()
      { _M_default_initialize(__n); }

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
      : _Base(_Node_alloc_type(__a))
      { _M_fill_initialize(__n, __value); }
#else
      /**
       *  @brief  Creates a %list with copies of an exemplar element.
       *  @param  __n  The number of elements to initially create.
       *  @param  __value  An element to copy.
       *  @param  __a  An allocator object.
       *
       *  This constructor fills the %list with @a __n copies of @a __value.
       */
      explicit
      list(size_type __n, const value_type& __value = value_type(),
	   const allocator_type& __a = allocator_type())
      : _Base(_Node_alloc_type(__a))
      { _M_fill_initialize(__n, __value); }
#endif

      /**
       *  @brief  %List copy constructor.
       *  @param  __x  A %list of identical element and allocator types.
       *
       *  The newly-created %list uses a copy of the allocation object used
       *  by @a __x.
       */
      list(const list& __x)
      : _Base(__x._M_get_Node_allocator())
      { _M_initialize_dispatch(__x.begin(), __x.end(), __false_type()); }

#if __cplusplus >= 201103L
      /**
       *  @brief  %List move constructor.
       *  @param  __x  A %list of identical element and allocator types.
       *
       *  The newly-created %list contains the exact contents of @a __x.
       *  The contents of @a __x are a valid, but unspecified %list.
       */
      list(list&& __x) noexcept
      : _Base(std::move(__x)) { }

      /**
       *  @brief  Builds a %list from an initializer_list
       *  @param  __l  An initializer_list of value_type.
       *  @param  __a  An allocator object.
       *
       *  Create a %list consisting of copies of the elements in the
       *  initializer_list @a __l.  This is linear in __l.size().
       */
      list(initializer_list<value_type> __l,
           const allocator_type& __a = allocator_type())
      : _Base(_Node_alloc_type(__a))
      { _M_initialize_dispatch(__l.begin(), __l.end(), __false_type()); }
#endif

      /**
       *  @brief  Builds a %list from a range.
       *  @param  __first  An input iterator.
       *  @param  __last  An input iterator.
       *  @param  __a  An allocator object.
       *
       *  Create a %list consisting of copies of the elements from
       *  [@a __first,@a __last).  This is linear in N (where N is
       *  distance(@a __first,@a __last)).
       */
#if __cplusplus >= 201103L
      template<typename _InputIterator,
	       typename = std::_RequireInputIter<_InputIterator>>
        list(_InputIterator __first, _InputIterator __last,
	     const allocator_type& __a = allocator_type())
	: _Base(_Node_alloc_type(__a))
        { _M_initialize_dispatch(__first, __last, __false_type()); }
#else
```

### 4. 运算符重载

移动赋值运算符

```cpp
      list&
      operator=(list&& __x)   //移动赋值运算符
      {
        // NB: DR 1204.
        // NB: DR 675.
        this->clear();
        this->swap(__x);
        return *this;
      }
```

初始化列表

> 这是 `list` 类的赋值运算符重载，它允许使用初始化列表来为 `list` 对象赋值。
>
> 该运算符重载接受一个初始化列表 `__l`，它将 `__l` 中的元素赋值给当前的 `list` 对象。具体地，它调用 `assign` 函数，该函数接受两个迭代器参数，分别指向初始化列表的第一个元素和最后一个元素。`assign` 函数用这些元素来替换当前 `list` 对象中的元素。
>
> 该运算符重载返回一个 `list` 的引用，以支持链式赋值。

```cpp
	 list&
      operator=(initializer_list<value_type> __l)
      {
	this->assign(__l.begin(), __l.end());
	return *this;
      }
```

