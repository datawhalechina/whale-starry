# task1 list源码
[toc]
## 前言

今天大概看了下list源码（gcc4.9版本的），其实对类型萃取和分配器这里还是很模糊，对list的结构大概有了个了解，实际上list的实现为一个双向链表。在下文中，对`list`源码剖析时，简化了源码是为了便于分析关键信息，而不是被庞杂复杂的代码扰乱的找不到头绪。


## 链表节点类_List_node_base与_List_node

源码中分别有两个结构体作为链表节点的组成，其中一个是`struct _List_node_base`，这个结构体主要定义了前后两个指针，分别为`_M_next`、`_M_prev`；另一个是`struct _List_node`，继承了这两个指针的同时，又多了一个数据项`_M_data`，类型由模板传入参数确定，具体定义如下：

```c++
// node struct with two pointers.
struct _List_node_base {
  _List_node_base* _M_next;
  _List_node_base* _M_prev;
};

// node struct with data value.
template <typename _Tp>
  struct _List_node : public _List_node_base {
    _Tp _M_data;
  };
```
其实链表节点`_List_node_base`还有几个重要的成员函数如`_M_hook`并未在这里罗列，下文在介绍链表插入操作时，会单独对其进行介绍。

## 链表迭代器类_List_iterator
然后就是链表遍历时的迭代器结构体`_List_iterator`。这部分我认为比较重要的是，链表迭代器中包含了一个指向链表节点的指针，这个指针可以通过运算符重载来遍历整个链表。


链表迭代器部分的整体代码可参考如下示例：
```c++
// list iterator
template <typename _Tp>
struct _List_iterator {
  typedef _List_iterator<_Tp> _Self;
  typedef _List_node<_Tp> _Node;
  
  typedef _Tp value_type;
  typedef _Tp* pointer;
  typedef _Tp& reference;

  _List_node_base* _M_node;

  // two construct function.
  _List_iterator() : _M_node() { }
  explicit _List_iterator(_List_node_base* __x) : _M_node(__x) { }

  // return iterator self.
  _Self _M_const_cast() const { return *this; }

  // operator functions.
  reference operator*() const { return static_cast<_Node>(_M_node)->_M_data; }
  // pointer operator->() const { return }
  _Self& operator++() {
    _M_node = _M_node->_M_next;
    return *this;
  }
  _Self operator++(int) {
    _Self __tmp = *this;
    _M_node = _M_node->_M_next;
    return __tmp;
  }
  _Self& operator--() {
    _M_node = _M_node->_M_prev;
    return *this;
  }
  _Self operator--(int) {
    _Self __tmp = *this;
    _M_node = _M_node->_M_prev;
    return __tmp;
  }
  bool operator==(const _Self& __x) const { return _M_node == __x._M_node; }
  bool operator!=(const _Self& __x) const { return _M_node != __x._M_node; }
};
```

迭代器类中定义了节点指针`_M_node`：
```c++
template <typename _Tp>
struct _List_iterator {
  ...
  _List_node_base* _M_node;
};
```

可以通过`_M_node`遍历每一个链表节点。而且指针类型虽然为`_List_node_base`，但是若当前迭代器指针真想访问链表节点元素值，只需强转类型为`_List_node*`即可，因为每个带有数据项的链表节点均为`_List_node`类型。我们通常在使用迭代器取值时，操作与指针很像，直接进行`*`解引用操作即可，在迭代器中通过对`*`进行运算符重载，已经实现了这一点，重载实现依然在`_List_iterator`类中，代码如下：
```c++
template<typename _Tp>
struct _List_iterator {
  ...
  _Tp& operator*() const {
    return static_cast<_Node*>(_M_node)->_M_data;
  }
  ...
};
```


> Q：`list`的迭代器本身是否可以是一个指针？
> A：不可以，指针对链表进行`++`操作以后，地址肯定是有问题的，所以迭代器是一个对象，它不是一个指针，只不过迭代器的遍历是通过指针变量追溯的，这个迭代器对象由于进行了运算符重载操作，所以迭代器对象中的指针所做的加减操作都只是逻辑上的操作，而非连续地址上的加减，因此迭代器对象就可以实现遍历链表的操作了。


可以看到`_List_node_base* _M_node;`即为迭代器当中定义的节点指针，这个指针经过`operator`重载运算符的操作，对链表进行遍历，再比如对于前置`++`重载：
```c++
typedef _List_iterator<_Tp> _Self;

_Self& operator++() {
  _M_node = _M_node->_M_next;
  return *this;
}
```

让指针指向`_M_next`指向的节点即可，返回的是`_Self`，这个`_Self`就是一个`list`迭代器对象。

> 接下来就是我认为比较难的地方了，主要难点在于`_List_base`类中的这个`rebind`操作，我没太看懂，所以涉及到空间配置器的代码我先跳过了。只需要大致了解`allocate(1)`表示申请一个当前类型的空间，模板如果传入的是`int`，那就申请一个`int`类型空间，如果传入的是一个类，那就申请一个能够存放下当前类这么大的内存空间，`deallocate(__p, 1)`中`__p`为指向申请内存空间的指针，该函数则对应释放一个类型大小的空间。

## 链表类的实现
### list_base类
链表类`list`继承了`_List_base`类，这个`_List_base`类当中我先不去讨论内存的分配问题。这里面定义了一个`_M_impl`对象，其类型为`_List_impl`，这是个结构体类型，嵌套在了`_List_base`当中，形如：
```c++
struct _List_impl : public _Node_alloc_type {
  _List_node_base _M_node; // 这是哨兵节点，存在前后指针_M_next和_M_prev
  
  // 构造函数
  List_impl() : _Node_alloc_type(), _M_node() { }
  List_impl(const _Node_alloc_type& __a) : _Node_alloc_type(__a), _M_node() { }
};
```

这个`_M_impl`对象实际上对链表内存进行了申请，同时在当前内存上创建了一个只有前后指针的结构体对象`_M_node`，`_List_base`类的部分内容如下：

```c++
/* list */
template <typename _Tp, typename _Alloc>
class _List_base {
protected:
  ...
  struct _List_impl : public _Node_alloc_type {
    _List_node_base _M_node; // 这是哨兵节点，存在前后指针_M_next和_M_prev

    // 构造函数
    _List_impl() : _M_node() {}
    _List_impl(const _Node_alloc_type& __a) : Node_alloc_type(__a), _M_node() {}
  };

  _List_impl _M_impl; // 这里定义的是一个分配内存后的哨兵节点实例

  // _M_get_node function申请堆空间（配置一个节点并传回）
  _List_node<_Tp>* _M_get_node() { return _M_impl._Node_alloc_type::allocate(1); }
  // _M_put_node function释放堆空间（释放一个节点）
  void _M_put_node(_List_node<_Tp>* __p) { _M_impl._Node_alloc_type::deallocate(__p, 1); }

public:
  ...
  // 初始化单个哨兵节点，让这个节点的prev和next指针都指向自己。
  void _M_init() {
    this->_M_impl._M_node._M_next = &this->_M_impl._M_node;
    this->_M_impl._M_node._M_prev = &this->_M_impl._M_node;
  }
  ...

  // 构造函数
  _List_base() : _M_impl() { _M_init(); }
  _List_base(const _Node_alloc_type& __a) : _M_impl(__a) { _M_init(); }

  void _M_clear();
  ~_List_base() { _M_clear(); }
};
```

可以看到`_List_base`主要分配了一个带有前后指针的哨兵节点，即类中定义的`_M_impl`对象。

### list类

接下来是真正的`list`链表类，继承了`_List_base<_Tp, _Alloc>`这个类。`list`链表类当中有各种链表的操作（比如在某个迭代器位置插入节点、删除节点等），还涉及到`list`的内存申请和释放。所以其实当创建`list`模板类对象时，由于继承了`_List_base<_Tp, _Alloc>`，因此也会首先创建出哨兵节点`_M_impl`对象。

#### 构造函数
`list`类的构造函数主要分为四种，分别为：
* 无参构造空链表
* 初始化分配器对象，构造空链表
* 创建填充`n`个`value`的链表
* 创建一个链表，该链表所有元素由迭代器`[__first, __end]`区间中所有元素组成

关于构造函数部分的定义，可查看如下代码：

```c++
template <typename _Tp, typename _Alloc = std::allocator<_Tp>>
class list : protected _List_base<_Tp, _Alloc> {
  typedef _List_base<_Tp, _Alloc> _Base;
  typedef _Alloc allocator_type;
  ...

public:
  /* 构造函数 */
  // 创建一个没有任何元素的链表
  list() : _Base() { }
  
  // 创建一个没有任何元素的链表，__a为一个分配器对象
  explicit list(const allocator_type& __a) : _Base(_Node_alloc_type(__a)) { }
  
  // 该构造函数用于给list初始化填充__n个__value
  explicit list(size_type __n,
                const _value_type& __value=value=type(),
                const allocator_type& __a = allocator_type())
                : _Base(_Node_alloc_type(__a))
  {
    _M_fill_initialize(__n, __value);
  }

  //将迭代器__first到__last之间的所有元素拷贝并作为链表元素，__a为分配器对象
  template <typename _InputIterator>
  list(_InputIterator __first, _InputIterator __last,
       const allocator_type& __a = allocator_type())
       : _Base(_Node_alloc_type(__a))
  {
    typedef typename std::__is_integer<_InputIterator>::__type _Integral;
    _M_initialize_dispatch(__first, __last, _Integral());
  }
```

#### 节点创建

那么有了链表类的初始化构造函数，关于链表节点的创建，需要仔细看下`list`类，在`list`类中还实现了一个`_M_create_node`函数，该函数用于创建一个链表节点，其代码如下：
```c++
template <typename _Tp, typename _Alloc = std::allocator<_Tp>>
class list : protected _List_base<_Tp, _Alloc> {
  ...
protected:
  typedef _List_node<_Tp> _Node;

  _Node* _M_create_node(const value_type& __x) {
    _Node* __p = this->_M_get_node();
    __try
    {
      _M_get_Tp_allocator().construct(std::__addressof(__p->_M_data), __x);
    } __catch(...) {
      _M_put_node(__p);
      __throw_exception_again;
    }
    return __p;
  }
};
```

该创建节点函数`_M_create_node`函数主要完成了以下几个步骤：
1. 调用`_M_get_node()`，分配一个节点大小的空间
2. 在分配的地址空间上构造一个`_List_node`对象，设置节点的数据项为`_M_create_node`的传入参数
3. 返回指向这个内存空间的指针

这个`_M_get_node()`和`_M_put_node()`均在`_List_base`当中定义了，通过分配器对象调用`allocate`和`deallocate`函数申请内存空间。那么以上就是单一节点创建函数`_M_create_node`。这个创建节点的函数`_M_create_node`非常重要，因为对于链表的插入操作来说，其底层都涉及到`_M_insert`函数，插入操作通常会调用这个`_M_create_node`函数来创建一个节点，所以这里对这个函数了解后，有助于对`list`容器中用到的算法进行理解。


#### 插入操作

还记得在前面分析链表类`list`的构造函数时，有一个构造函数用于创建一个填充`n`个`value`的链表：
```c++
// 该构造函数用于给list初始化填充__n个__value
explicit list(size_type __n,
              const _value_type& __value=value=type(),
              const allocator_type& __a = allocator_type())
              : _Base(_Node_alloc_type(__a))
{
  _M_fill_initialize(__n, __value);
}
  
// Called by list(n,v,a), and the range constructor when it turns out
// to be the same thing.
void _M_fill_initialize(size_type __n, const value_type& __x) {
  for (; __n; --__n)
    push_back(__x);
}

void push_back(const value_type& __x) { this->_M_insert(end(), __x); }

void _M_insert(iterator __position, const value_type& __x) {
  _Node* __tmp = _M_create_node(__x);
  __tmp->_M_hook(__position._M_node);
}
```

那么通常创建一个链表，首先是通过分配器对单一节点开辟空间，在开辟的空间上进行对象的构造、初始化操作，然后重复进行多次，直到`n`个节点均插入至双向循环链表当中。本人将上述`list`构造函数要用到的调用链函数均贴了出来，这几个函数都在文件`/gcc/libstdc++-v3/include/bits/stl_list.h`当中定义。

可以看到，当要创建一个拥有节点数为`n`，节点数据项为`value`的链表时，会不断调用`push_back`函数插入`value`，而`push_back`又调用`_M_insert`函数。

仔细看`_M_insert`函数分别传入两个参数，第一个为要插入到的迭代器位置，第二个就是要插入节点的数据项。`_M_insert`分别进行了两步操作：
1. 创建一个链表节点。（开辟空间，在开辟的空间上对`_List_node<_Tp>`模板类对象进行构造，并返回开辟空间的地址）
2. 调用`_M_hook`钩子函数来将新创建好的链表节点前插至链表当中。

`_M_hook`函数声明在`_List_node_base`结构体当中，作为链表节点的成员函数，定义在`/gcc/libstdc++-v3/src/c++98/list.cc`当中，其实现代码如下：
```c++
void _List_node_base::_M_hook(_List_node_base* const __position) {
  this->_M_next = __position;
  this->_M_prev = __position->_M_prev;
  __position->_M_prev->_M_next = this;
  __position->_M_prev = this;
}
```

那看到了`_M_hook`钩子函数的实现后，其实就易于理解了，这就是常用的双向链表插入操作，经过层层查找，终于找到了链表插入操作的精华。假设想在链表尾部插入元素，参数传入的是`end()`返回的迭代器指向的节点，即哨兵节点，经过`_M_hook`的操作后，新插入的节点移至哨兵节点之前。

那么以上就是对插入操作的分析。

#### 删除操作

关于删除操作，主要以`_M_unhook`函数为例讲解吧，这个函数故名思义，「脱钩」，它和`_M_hook`一样均定义至`_List_node_base`结构体中，作为链表节点的成员函数，其实现与`_M_hook`均在同一路径下，代码如下：

```c++
void _List_node_base::_M_unhook() {
  _List_node_base* const __next_node = this->_M_next;
  _List_node_base* const __prev_node = this->_M_prev;
  __prev_node->_M_next = __next_node;
  __next_node->_M_prev = __prev_node;
}
```

暂且不去考虑内存的释放，这本不该由`_M_unhook`去做，就像内存的申请也不由`_M_hook`去做一样。`_M_unhook`函数的「脱钩」意味着将链表上的某个节点从链表上移除掉。上面代码的操作指的是，在要删除的链表前后分别进行指针指向，然后将两个指针分别只想对方，从而忽略中间要删除的链表节点，总的来说，还挺巧妙的。




### 补充，如empty()、begin()、end()等list成员方法
`list`本身是一个环形双向列表，从`_List_node_base`中对链表节点的定义就可以看出来，具有`_M_prev`指针和`_M_next`指针；同时从`empty()`函数的写法也可以判断出来，代码如下：

```c++
bool empty() const {
  return this->_M_impl._M_node._M_next == & this->_M_impl._M_node;
}
```

可以看出，当哨兵节点的`next`指向自己的时候，说明链表中只剩下一个哨兵节点了，因此就为`nullptr`。

再比如STL的`list`当中，存在`begin()`和`end()`，通常使用的时候，我们知道它返回的是迭代器，在`list`类当中，对链表迭代器进行了`typedef`，不过这无关紧要，`list`类的部分内容如下：

```c++
template<typename _Tp, typename _Alloc = std::allocator<_Tp>>
  class list : protected _List_base<_Tp, _Alloc> {
    ...
    typedef _List_base<_Tp, _Alloc> _Base;
    ...

  public:
    ...
    typedef _List_iterator<_Tp> iterator;
    ...

  protected:
    ...
    using _Base::_M_impl;
    ...

    // 返回末尾空指针的next 作为当前的首节点 因此是个环形链表
    iterator begin() {
        return iterator(this->_M_impl._M_node._M_next);
    }
    iterator end() {
        return iterator(&this->_M_impl._M_node);
    }
    // next指向自己的时候 说明只有一个空哨节点 所以为空
    bool empty() const {
      return this->_M_impl._M_node._M_next == & this->_M_impl._M_node;
    }
    size_type size() const {
        return std::distance(begin(), end());
    }
  };
```

观察`begin()`和`end()`，分别返回的是一个迭代器对象，即`_List_iterator<_Tp>`模板类对象，`begin()`指向了这个`list`的`_M_next`位置，`list`的`_M_impl`对象为哨兵节点，其`_M_next`代表首位置的话，这可以推测出是环形链表。同时，`end()`返回的就是指向哨兵节点的迭代器，在日常使用时，`end()`代表的本身就是链表末尾节点的下一个位置，这也符合推测的逻辑。因此这里认为链表为一个双向循环链表。


## 阶段性总结

目前以上就是我的个人收获，大概对`list`的结构有了了解，但对于一些类型萃取和分配器的内容还有些云里雾里，后续我会不断回顾这些所学，一点点补充。争取先把`list`总结完。