# task1 list源码

## 前言

今天大概看了下list源码（gcc4.9版本的），其实对类型萃取和分配器这里还是很模糊，对list的结构大概有了个了解，实际上list的实现为一个双向链表。


## 链表节点

源码中分别有两个结构体作为链表节点的组成，其中一个是`struct _List_node_base`，这个结构体主要负责前后指针，包含了两个指针`_M_next`、`_M_prev`；另一个是`struct _List_node`，继承了这两个指针，同时又多了一个数据项`_M_data`，类型由模板传入参数确定，具体定义如下：

```c++
// node struct with two pointers.
struct _List_node_base {
  _List_node_base *_M_next;
  _List_node_base *_M_prev;
};

// node struct with data value.
template<typename _Tp>
  struct _List_node : public _List_node_base {
    _Tp _M_data;
  };
```

## 链表迭代器部分
然后就是链表遍历时的迭代器结构体。这部分我认为比较重要的是，链表迭代器中包含了一个指向链表节点的指针，这个指针可以通过运算符重载来遍历链表。

> Question: list的迭代器是否可以是一个指针？
> Answer: 不可以，指针进行++以后，地址肯定是有问题的，所以迭代器是一个对象，它不是一个指针，只不过迭代器的遍历是通过指针变量追溯的，这个指针由于被运算符重载了，所以指针所做的操作就是遍历链表。

链表迭代器部分的定义如下：
```c++
// list iterator
template<typename _Tp>
  struct _List_iterator {
    typedef _List_iterator<_Tp> _Self;
    typedef _List_node<_Tp> _Node;

    typedef _Tp value_type;
    typedef _Tp * pointer;
    typedef _Tp & reference;

    _List_node_base *_M_node;

    // two construct function.
    _List_iterator() : _M_node() {}
    explicit _List_iterator(_List_node_base *__x) : _M_node(__x) {}

    // return iterator self.
    _Self _M_const_cast() const { return *this; }

    // operator functions.
    reference operator*() const { return static_cast<_Node>(_M_node)->_M_data; }
    // pointer operator->() const { return }
    _Self &operator++() {
      _M_node = _M_node->_M_next;
      return *this;
    }
    _Self operator++(int) {
      _Self __tmp = *this;
      _M_node = _M_node->_M_next;
      return __tmp;
    }
    _Self &operator--() {
      _M_node = _M_node->_M_prev;
      return *this;
    }
    _Self operator--(int) {
      _Self __tmp = *this;
      _M_node = _M_node->_M_prev;
      return __tmp;
    }
    bool operator==(const _Self &__x) const { return _M_node == __x._M_node; }
    bool operator!=(const _Self &__x) const { return _M_node != __x._M_node; }
  };
```

可以看到`_List_node_base *_M_node;`即为我们定义的指针，这个指针经过`operator`重载运算符的操作，对链表进行遍历，比如对于前置++重载：
```c++
_Self &operator++() {
  _M_node = _M_node->_M_next;
  return *this;
}
```

让指针指向_M_next指向的节点即可，返回的是`_Self`，这个`_Self`就是一个list迭代器对象。

> 接下来就是我认为比较难的地方了，主要难点在于`_List_base`类中的这个`rebind`操作，我没太看懂。代码我直接放到`task_1/list.cc`下面吧。

## 链表类的实现

链表类`list`继承了`_List_base`类，这个`_List_base`类当中我先不去讨论内存的分配问题。这里面定义了一个`_M_impl`对象，其类型为`_List_impl`，这是个结构体类型，嵌套在了`_List_base`当中，这个`_M_impl`对象实际上对链表内存进行了申请，同时在当前内存上创建了一个只有前后指针的结构体对象`_M_node`，`_List_base`类的部分内容如下：

```c++
/* list */
template<typename _Tp, typename _Alloc>
  class _List_base {
    protected:
      ...
      struct _List_impl : public _Node_alloc_type {
        _List_node_base _M_node; // 这是哨兵，存在前后指针_M_next和_M_prev

        // construct function
        _List_impl() : _M_node() {}
        _List_impl(const _Node_alloc_type &__a) : Node_alloc_type(__a), _M_node() {}
      };

      _List_impl _M_impl; // 这个接口应该是用来分配内存的
      // _M_get_node function申请堆空间（配置一个节点并传回） _M_put_node function释放堆空间（释放一个节点）

    public:
      ...
      // 初始化单个哨兵节点，让这个节点的prev和next指针都指向自己。
      void _M_init() {
        this->_M_impl._M_node._M_next = &this->_M_impl._M_node;
        this->_M_impl._M_node._M_prev = &this->_M_impl._M_node;
      }
      ...

      // construct function
      _List_base() : _M_impl() { _M_init(); }
      _List_base(const _Node_alloc_type &__a) : _M_impl(__a) { _M_init(); }

      void _M_clear();
      ~_List_base() { _M_clear(); }
  };
```

可以看到`_List_base`主要分配了一个带有前后指针的哨兵节点，即类中定义的`_M_impl`对象。

接下来是真正的`list`链表类，继承了`_List_base<_Tp, _Alloc>`这个类。`list`链表类当中有各种链表的操作，还涉及到`list`的内存申请和释放。所以其实当创建`list`模板类对象时，由于继承了`_List_base<_Tp, _Alloc>`，因此也会首先创建出哨兵节点`_M_impl`对象。在`list`当中，可以看到它其实是一个环形双向列表，从`empty()`函数的写法就可以判断出来，代码如下：

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

目前以上就是我的个人收获，大概对`list`的结构有了了解，但对于一些类型萃取和分配器的内容还有些云里雾里，后续我会不断回顾这些所学，一点点补充。