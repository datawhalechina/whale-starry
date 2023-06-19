## The learning record of stl_list.

### 简洁

list 是一个双向环形链表，只需要一个指针，便可完整表现整个链表；在list中任何位置插入和删除的速度都很快。

### 迭代器与容器

迭代器是_List_node_base型的指针，每个Containers含有一个容器成员，实现了一系列的操作符重载，用于模拟指针操作。操作符重载中有 operator++，operator--，operator==，operator!=，operator*，operator->。

### 成员函数

其中主要的成员函数有push_back，pop_front，emplace_back，pop_back等。push_back中的_M_hook是将this指针放到position前面。

```C++
    template<typename... _Args>
    void
    _M_insert(iterator __position, _Args&&... __args)
    {
    _Node* __tmp = _M_create_node(std::forward<_Args>(__args)...);
    __tmp->_M_hook(__position._M_node);
    this->_M_inc_size(1);
    }
```

size() 函数就是返回list大小，复杂度是O(1)的。

```C++
      size_type
      size() const _GLIBCXX_NOEXCEPT
      { return _M_node_count(); }

      size_t _M_node_count() const { return _M_get_size(); }

      size_t _M_get_size() const { return _M_impl._M_node._M_size; }
```

begin()，end()，empty() 函数通过尾结点和哨兵结点即可容易得到结果。