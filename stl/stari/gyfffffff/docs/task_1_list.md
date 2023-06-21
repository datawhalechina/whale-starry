# list 源码学习

## 0. 说明

源码：[gcc4.9](https://github.com/gcc-mirror/gcc/blob/releases/gcc-4.9/libstdc++-v3/include/bits/stl_list.h#L81)

参考侯捷老师的课程、插图来源：[课程视频](https://www.bilibili.com/video/BV1wh4y1o7e9/?p=14&spm_id_from=333.1007.top_right_bar_window_history.content.click&vd_source=0d00fb4c5464c5c00d00e5815bfaf112)

UML绘制：[visual paradigm](https://online.visual-paradigm.com/cn/diagrams/features/uml-tool/)

## 1. 源码结构

![](.\images\uml.png)

**链表节点**    `_List_node`, 继承自`_List_node_base`。

**链表**            `list`,  继承自`_List_base`

**迭代器 **       `_List_iterator` 结构体

## 2. 链表设计

list 是一个双向环状链表，头尾之间有一个占位节点。

#### 2.1 list class 声明

```C++
  template<typename _Tp, typename _Alloc = std::allocator<_Tp> > // _Tp: 链表节点数据类型；Alloc:内存分配的分配器类型， 默认std::allocator<_Tp>
    class list : protected _List_base<_Tp, _Alloc>
    {
    };
```

list 继承_List_base， 这个类里定义了一些更加基础的函数，在list中将会用到，如list类在开头所写：

```C++
typedef _List_base<_Tp, _Alloc>			_Base;    
// ...
using _Base::_M_impl;
using _Base::_M_put_node;
using _Base::_M_get_node;
using _Base::_M_get_Node_allocator;
```

接下来，按照list中实现的函数顺序，依次学习每个成员函数。

#### 2.2 list 成员函数

首先是三个私有成员变量，分别提供了编译器为C++11前后两个版本的实现：

**_M_create_node**  创建一个节点；

**_S_distance**   		计算两个迭代器之间的元素数量；

**_M_node_count**    计算容器中存储的元素数量。

【构造函数1】 没有任何元素的list

```C++
      explicit
      list(const allocator_type& __a) _GLIBCXX_NOEXCEPT
      : _Base(_Node_alloc_type(__a)) { }
```

这里的构造使用了C++11的委托构造函数，委托list的基类的构造函数。[参考](https://blog.csdn.net/K346K346/article/details/81703862)

【构造函数2】创建有n个默认元素的list

```C++
      explicit
      list(size_type __n, const allocator_type& __a = allocator_type())
      : _Base(_Node_alloc_type(__a))
      { _M_default_initialize(__n); }
```

【构造函数3】创建有n个指定元素的list

```C++
      list(size_type __n, const value_type& __value,
	   const allocator_type& __a = allocator_type())
      : _Base(_Node_alloc_type(__a))
      { _M_fill_initialize(__n, __value); }
```

【构造函数4】通过复制创建list

```C++
      list(const list& __x)
      : _Base(_Node_alloc_traits::
	      _S_select_on_copy(__x._M_get_Node_allocator()))
      { _M_initialize_dispatch(__x.begin(), __x.end(), __false_type()); }
```

【构造函数5】暂没看懂

【构造函数6】暂没看懂

【构造函数7】从一个范围内构造list

```C++
      template<typename _InputIterator,
	       typename = std::_RequireInputIter<_InputIterator>>
	list(_InputIterator __first, _InputIterator __last,
	     const allocator_type& __a = allocator_type())
	: _Base(_Node_alloc_type(__a))
	{ _M_initialize_dispatch(__first, __last, __false_type()); }
```

经测试，发现范围使用的是前闭后开区间。

【操作符`=`重载】复制list

```C++
      list&
      operator=(const list& __x);
```

【操作符`=`重载】将初始化列表赋值给list

```C++
      list&
      operator=(initializer_list<value_type> __l)
      {
	this->assign(__l.begin(), __l.end());
	return *this;
      }
```

初始化列表是`{}`包裹的，用逗号分割的数据结构。

【**assign**】根据参数不同，可以分配n个val；可以指定赋成从一个迭代器到下一个迭代器的内容；可以用初始化列表分配。会改变list元素数量。

```C++
    void
      assign(size_type __n, const value_type& __val)
      { _M_fill_assign(__n, __val); }
      
    template<typename _InputIterator,
	       typename = std::_RequireInputIter<_InputIterator>>
	void
	assign(_InputIterator __first, _InputIterator __last)
	{ _M_assign_dispatch(__first, __last, __false_type()); }

    void
          assign(initializer_list<value_type> __l)
          { this->_M_assign_dispatch(__l.begin(), __l.end(), __false_type()); }
```

【**get_allocator**】获取分配器对象内存地址（？

```C++
      allocator_type
      get_allocator() const _GLIBCXX_NOEXCEPT
      { return allocator_type(_Base::_M_get_Node_allocator()); }
```

【**begin**】

```C++
      iterator
      begin() _GLIBCXX_NOEXCEPT
      { return iterator(this->_M_impl._M_node._M_next); }
```

【**end**】

```C++
      end() _GLIBCXX_NOEXCEPT
      { return iterator(&this->_M_impl._M_node); }
```

从上面代码推断，STL list是一个环状链表且_M_node是环状链表的空白节点。

【**empty**】

```C++
      _GLIBCXX_NODISCARD bool
      empty() const _GLIBCXX_NOEXCEPT
      { return this->_M_impl._M_node._M_next == &this->_M_impl._M_node; }
```

如果空白节点的下一个是自己，说明list为空。

【**size**】元素个数

```C++
      size_type
      size() const _GLIBCXX_NOEXCEPT
      { return _M_node_count(); }
```

返回头结点（空白节点）的一个成员变量，里面记录了元素个数。

【**front&back**】返回第一个/最后一个节点内的数据的引用。

【**push_front&emplace_front**】在最前面加一个元素

```C++
      void
      push_front(const value_type& __x)
      { this->_M_insert(begin(), __x); }

      void
      push_front(value_type&& __x)
      { this->_M_insert(begin(), std::move(__x)); }

      void
      emplace_front(_Args&&... __args)
      {
        this->_M_insert(begin(), std::forward<_Args>(__args)...);
```

两者实现了同样的功能,区别在于emplace_front不会改变变量属性，而使用`std::move`的函数效率更高。

【**emplace&insert**】

```C++
    template<typename... _Args>
	iterator
	emplace(const_iterator __position, _Args&&... __args);

	iterator
      insert(const_iterator __position, const value_type& __x);
```

区别在于使用emplace使用了变参模板和完美转发，当list中元素是类类型时，emplace可以不产生临时变量而构造一个对象并插入指定位置。

【**merge、sort**】

暂时没找到在哪实现的。



### 3. 迭代器的设计

几乎所有容器都有自己的iterator。

迭代器相当于一个泛化的指针。

但是当我们想要对链表的指针做加一（减一）操作，实际上是想得到next（prev）节点，而不是当前节点的下一个(前一个)地址。所以在迭代器的实现中要重载各种运算符。

以`++`为例：

![](.\images\iterator visualize.png)



```C++
      _List_iterator() _GLIBCXX_NOEXCEPT  // 构造函数，_M_node初始化为空指针
      : _M_node() { }

// postfix form, i++, int参数是占位符
	  _Self
      operator++(int) _GLIBCXX_NOEXCEPT
      {
	_Self __tmp = *this;   // 记录原值
	_M_node = _M_node->_M_next;  // 前进一个节点
	return __tmp;  // 返回原值
      }

// prefix form, ++i, 
      _Self&
      operator++() _GLIBCXX_NOEXCEPT
      {
	_M_node = _M_node->_M_next;  // 取出_M_node所指向的节点里的next指针，赋给_M_node本身，相当于让_M_node指向下一个节点
	return *this;
      }

	...
// The only member points to the %list element.
      __detail::_List_node_base* _M_node;
```

- 其中`_GLIBCXX_NOEXCEPT`表示不会抛出异常

```C++
#define _GLIBCXX_NOEXCEPT noexcept
```

- 源代码将` _M_node`这个唯一的成员变量的定义放在最后，这是一种编程风格。这个变量指向一个链表节点，是一个指针，大小应该是8。

- 合理推测`--`操作就是让`_M_node`指向`prev`

- 前加加的返回值是引用，而后加加的返回值不是引用，是为了统一C++中`++`的用法：

  ![](.\images\++using in cpp.png)

对迭代器取`*`是想取出里面的data，而不是一个节点。所以对\*也要重载

```C++
	  typedef _List_node<_Tp>			_Node;      
	  typedef _Tp&				reference;
	  typedef _Tp*				pointer;	
// Must downcast from _List_node_base to _List_node to get to value.
      reference
      operator*() const _GLIBCXX_NOEXCEPT
      { return *static_cast<_Node*>(_M_node)->_M_valptr(); }

      pointer
      operator->() const _GLIBCXX_NOEXCEPT
      { return static_cast<_Node*>(_M_node)->_M_valptr(); }
	...
// The only member points to the %list element.
      __detail::_List_node_base* _M_node;
```
```C++
_Tp*       _M_valptr()       { return _M_storage._M_ptr(); }
```
`static_cast`将 `_M_node` 转换为 `_Node*` 类型，这是链表节点的指针类型。接下来，通过访问 `_M_valptr()`，它将得到指向当前节点中的值的指针，值的类型是`_Tp`。因为函数返回引用类型，所以\* 作用于这个`_Tp`变量。`->`重载将\*去掉即可。



### 4. 节点设计

链表节点  `_List_node`, 继承自`_List_node_base`。 前者存放数据，后者存放前后指针和list中需要用到的方法。

其中，_List_node 会根据编译器支持的C++标准版本使用不同的方式来存储节点数据：

```c++
  /// An actual node in the %list.
  // 模板结构体 _List_node的定义, 根据编译器支持的C++标准版本使用不同的方式来存储节点数据
template <typename _Tp>  // _Tp is data type
struct _List_node: public __detail::_List_node_base  // derived from _List_node_base
{
      // condition lines, choose a way to compiling
#if __cplusplus >= 201103L  // 如果编译器支持 C++11 及以上的标准版本
      __gnu_cxx::__aligned_membuf<_Tp> _M_storage;
      _Tp*       _M_valptr()       { return _M_storage._M_ptr(); }
      _Tp const* _M_valptr() const { return _M_storage._M_ptr(); }
#else
      _Tp _M_data;
      _Tp*       _M_valptr()       { return std::__addressof(_M_data); }
      _Tp const* _M_valptr() const { return std::__addressof(_M_data); }
#endif
};
```
_List_node_base 代码：

```C++
struct _List_node_base
    {
      _List_node_base* _M_next;
      _List_node_base* _M_prev;

      ... // 后面要用的方法
    };

```



### 6. 收获&复习

1. uml 图。https://www.bilibili.com/video/BV1P741127u7/?vd_source=0d00fb4c5464c5c00d00e5815bfaf112

2. C++ 继承语法、访问修饰符

   > C++ 继承语法：
   >
   > ```C++
   > class derived-class: access-specifier base-class
   > ```

3. noexcept 关键字。在 C++ 中，使用 `noexcept` 关键字可以标记一个函数为不抛出异常的函数。

4. 运算符重载。

   > 重载的运算符本质上是**函数**，函数名是由关键字 operator 和其后要重载的运算符符号构成的。与其他函数一样，重载运算符有一个返回类型和一个参数列表。

5. 构造函数。

6. C++引用。https://www.runoob.com/cplusplus/cpp-references.html

   > 试想变量名称是变量附属在内存位置中的标签，您可以把引用当成是变量附属在内存位置中的第二个标签。因此，您可以通过原始变量名称或引用来访问变量的内容。例如：
   >
   > ```C++
   > int i = 17;
   > // 我们可以为 i 声明引用变量:
   > int&  r = i;  // "r 是一个初始化为 i 的整型引用"
   > ```

7. static_cast。https://www.cnblogs.com/QG-whz/p/4509710.html

   > static_cast是一个强制类型转换操作符。强制类型转换，也称为显式转换，C++中强制类型转换操作符有static_cast、dynamic_cast、const_cast、reinterpert_cast四个。
   >
   > - 编译器隐式执行的任何类型转换都可以由static_cast来完成，比如int与float、double与char、enum与int之间的转换等。
   >
   >   ```C++
   >   double a = 1.999;
   >   int b = static_cast<double>(a); //相当于a = b ;
   >   ```
   >   - 把精度大的类型转换为精度小的类型，static_cast使用位截断进行处理。
   >   使用static_cast可以找回存放在void\*指针中的值。
   >   ```C++
   >    double a = 1.999;
        void * vptr = & a;
        double * dptr = static_cast<double*>(vptr);
        cout<<*dptr<<endl;//输出1.999  
   >   ```

8. STL的容器，都遵循前闭后开区间。STL的容器都有begin()和end()。begin()指向容器首元素，而end()指向容器最后一个元素的下一个元素。
9. C++ 类的大小。[](https://www.cnblogs.com/ZY-Dream/p/10016731.html)
10. `explicit`关键字。
11. 初始化列表，C++11委托构造函数[参考](https://blog.csdn.net/K346K346/article/details/81703862)。
12. C++左值右值。[参考](https://nettee.github.io/posts/2018/Understanding-lvalues-and-rvalues-in-C-and-C/)
13. C++ std::move 。
14. C++ 完美转发。[参考](https://zhuanlan.zhihu.com/p/469607144)


### 7. 验证&测试（unfinished）

1. 如果用sizeof取一个链表大小，结果应该是？[test1_1](https://github.com/gyfffffff/whale-starry/blob/main/stl/stari/gyfffffff/src/task_1/test1_1.cpp)
2. 可以将成员变量的声明放在最后，即先使用后声明。[test1_2](https://github.com/gyfffffff/whale-starry/blob/main/stl/stari/gyfffffff/src/task_1/test1_2.cpp)

### 感受

 极大缓解了对源码的恐惧；即使读不完收获也很多。