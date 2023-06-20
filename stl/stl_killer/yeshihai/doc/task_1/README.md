# List源码阅读

目标：过一下List的源码

## List源码

首先是**`template<typename_Tp, typename_Alloc = std::allocator<_Tp> >`**

模板的使用，创建一个具有两个类型参数的模板类或函数。其中：

* `typename _Tp`：
  * `typename` 表示 `_Tp` 是一个类型参数。
  * `_Tp` 是第一个类型参数的名称。
* `typename _Alloc = std::allocator<_Tp>`：
  * `typename` 表示 `_Alloc` 是一个类型参数。
  * `_Alloc` 是第二个类型参数的名称，它表示分配器（allocator）的类型。
  * `= std::allocator<_Tp>` 给第二个类型参数提供了默认值，即使用 `std::allocator` 作为默认的分配器。

通过上述代码，创建的模板允许在实例化时指定两个类型参数。第一个参数（`_Tp`）可以是任意类型，而第二个参数（`_Alloc`）是一个可选参数，默认情况下它将使用 `std::allocator<_Tp>` 作为分配器类型。这种灵活性允许用户根据需要自定义分配器或者使用默认分配器。

再来看List的定义：

`class  list : protected _List_base<_Tp, _Alloc>{........}`

可见，list继承自 `_List_base`


## 构造函数


### 无元素

    `  *  @brief  Creates a %list with no elements.`

    ` *  @param  __a  An allocator object.`

    `   explicit`

    ` list(constallocator_type&__a) _GLIBCXX_NOEXCEPT`

    `: _Base(_Node_alloc_type(__a)) { }`

这其中，`explicit` 是一个关键字，用于修饰构造函数。它的作用是禁止编译器进行隐式类型转换，要求显式地调用构造函数来创建对象。

当构造函数被标记为 `explicit` 时，它将阻止编译器执行任何隐式转换，只能通过显式调用构造函数来创建对象。这可以避免一些意外的类型转换，增加代码的清晰性和可读性，以及减少潜在的错误

### 有n个元素且赋初值


```
      /**
       *  @brief  %List copy constructor.
       *  @param  __x  A %list of identical element and allocator types.
       *
       *  The newly-created %list uses a copy of the allocation object used
       *  by @a __x (unless the allocator traits dictate a different object).
       */
      list(const list& __x)
      : _Base(_Node_alloc_traits::
	      _S_select_on_copy(__x._M_get_Node_allocator()))
      { _M_initialize_dispatch(__x.begin(), __x.end(), __false_type()); }
```

参数解释：

1. `const list& __x`：这是一个参数，表示要复制的原始 `list` 对象。它是一个常量引用，以避免不必要的复制。
2. `: _Base(_Node_alloc_traits:: _S_select_on_copy(__x._M_get_Node_allocator()))`：这是构造函数初始化列表。在此处，我们使用了基类 `_Base` 的构造函数，并传递了一个参数。
3. `_Node_alloc_traits:: _S_select_on_copy(__x._M_get_Node_allocator())`：这里调用了一个静态成员函数 `_S_select_on_copy()` 来为 `_Base` 构造函数提供参数。该函数从传入的原始对象 `__x` 中获取其节点分配器（node allocator）。该函数具体实现可能根据具体的 `_Node_alloc_traits` 实现而有所不同。
4. `{ _M_initialize_dispatch(__x.begin(), __x.end(), __false_type()); }`：这是构造函数的主题部分，它调用了一个名为 `_M_initialize_dispatch()` 的辅助函数，通过调用传入 `__x` 对象的 `begin()` 和 `end()` 成员函数来初始化新创建的 `list` 对象。这个辅助函数的具体实现可能涉及迭代器和算法来复制原始对象的元素。

综上所述，这个构造函数可以理解为通过复制原始 `list` 对象来创建新的 `list` 对象。它使用 `_Base` 的构造函数并传递原始对象的节点分配器，然后调用辅助函数来复制原始对象的元素到新对象中。

### **从一个范围中进行初始化list**


```
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
      template<typename _InputIterator>
	list(_InputIterator __first, _InputIterator __last,
	     const allocator_type& __a = allocator_type())
	: _Base(_Node_alloc_type(__a))
	{
	  // Check whether it's an integral type.  If so, it's not an iterator.
	  typedef typename std::__is_integer<_InputIterator>::__type _Integral;
	  _M_initialize_dispatch(__first, __last, _Integral());
	}
#endif
```

这个构造函数从一个范围（`__first` 到 `__last`）创建一个 `list` 对象。具体解释如下：

1. `#if __cplusplus >= 201103L` 和 `#else`：这是预处理器条件语句，用于检查编译器是否支持 C++11 或更高版本的标准。
2. `template<typename _InputIterator, typename = std::_RequireInputIter<_InputIterator>>`：
   * 这是一个模板声明语句，定义了一个模板函数。模板参数 `_InputIterator` 表示输入迭代器的类型。
   * `typename = std::_RequireInputIter<_InputIterator>` 是一个默认模板参数，用于限制 `_InputIterator` 必须是输入迭代器（input iterator）类型。这是通过使用 `_RequireInputIter` 类型来实现的。
3. `list(_InputIterator __first, _InputIterator __last, const allocator_type& __a = allocator_type())`：
   * 这是构造函数的声明部分。
   * `__first` 和 `__last` 参数是输入迭代器，表示需要复制的范围。
   * `__a = allocator_type()` 是一个可选参数，用于指定分配器对象。
4. `: _Base(_Node_alloc_type(__a))`：
   * 这是构造函数的初始化列表，用于初始化基类 `_Base`。
   * `_Node_alloc_type(__a)` 通过将给定分配器对象 `__a` 传递给 `_Node_alloc_type` 构造函数来创建节点分配器。
5. `{ _M_initialize_dispatch(__first, __last, __false_type()); }`：
   * 这是构造函数的主体。
   * 调用了辅助函数 `_M_initialize_dispatch`，将 `__first` 和 `__last` 迭代器作为参数，并使用 `__false_type` 来指示不进行整型类型检查（即不是积分类型）。

以上代码使用了条件编译和模板特性，确保在编译器支持 C++11 或更高版本的情况下，在范围内复制元素，并通过提供分配器对象来初始化 `list` 对象。如果编译器不支持 C++11 或更高版本，则通过检查输入迭代器是否为整型类型来选择使用正确的辅助函数。

## 创造节点

```
#if __cplusplus < 201103L
      _Node*
      _M_create_node(const value_type& __x)
      {
	_Node* __p = this->_M_get_node();
	__try
	  {
	    _Tp_alloc_type __alloc(_M_get_Node_allocator());
	    __alloc.construct(__p->_M_valptr(), __x);
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
	  auto __p = this->_M_get_node();
	  auto& __alloc = _M_get_Node_allocator();
	  __allocated_ptr<_Node_alloc_type> __guard{__alloc, __p};
	  _Node_alloc_traits::construct(__alloc, __p->_M_valptr(),
					std::forward<_Args>(__args)...);
	  __guard = nullptr;
	  return __p;
	}
#endif
```

 `list` 类中的一个私有成员函数 `_M_create_node` 的定义，它用于创建一个 `_Node` 对象并返回

根据条件编译，分两种情况：

1. `#if __cplusplus < 201103L` 和 `#else`：条件编译语句检查 C++ 标准版本是否小于 C++11。
2. 在条件为真（C++ 标准版本小于 C++11）时：
   * `_Node* _M_create_node(const value_type& __x)` 是函数声明。
   * 这个函数接收一个参数 `__x`，表示要存储在节点中的值。
   * 首先，我们通过调用 `_M_get_node()` 获取一个节点，并将其赋值给指针 `__p`。
   * 然后，在使用 `_Tp_alloc_type` 类型的对象 `__alloc` 构造分配器时，调用 `__alloc.construct(__p->_M_valptr(), __x)` 将值构造到节点中。
   * 如果在构造过程中抛出异常，我们会释放节点并重新抛出异常。
   * 最后，返回创建的节点指针 `__p`。
3. 在条件为假（C++ 标准版本大于或等于 C++11）时：
   * `template<typename... _Args> _Node* _M_create_node(_Args&&... __args)` 是一个模板函数。
   * 这个函数是使用变长参数模板定义的，可以接受任意数量和类型的参数。
   * 首先，我们通过调用 `_M_get_node()` 获取一个节点，并将其赋值给自动推导类型的指针 `__p`。
   * 然后，通过引用方式获取节点分配器 `__alloc`。
   * 接下来，我们使用 `_Node_alloc_traits::construct` 成员函数以完美转发的方式将参数构造到节点的值中。
   * `_Allocated_ptr` 类型的对象 `__guard` 被初始化为 `__alloc` 和 `__p` 的组合，用于在函数执行完毕时自动释放节点。
   * 最后，将节点指针 `__p` 返回。

根据 C++ 标准版本的不同，这个函数的实现会有所变化。在 C++11 或更高版本中，利用了变长参数模板和完美转发的特性来支持更灵活的参数类型和数量。而在 C++11 之前的旧版本中，则采用了异常处理来保证分配节点和构造值的过程是安全的。

在这个过程中，通过 `_M_get_node()`获取新节点；在 `_M_create_node` 函数中，如果发生异常导致节点没有成功构造，我们调用了 `_M_put_node` 来将该节点返回给节点分配器，以确保内存资源得到正确释放和管理。

## 插入节点
