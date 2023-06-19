<!--
 * @Author: XavierZXY zxy_xavier@163.com
 * @Date: 2023-06-19 13:18:50
 * @LastEditors: XavierZXY zxy_xavier@163.com
 * @Description: 学习stl中list实现
 * 
 * Copyright (c) 2023 by zxy_xavier@163.com, All Rights Reserved. 
-->
# stl-list学习
> [stl源码](https://github.com/gcc-mirror/gcc/blob/releases/gcc-9/libstdc%2B%2B-v3/include/bits/stl_list.h)
> [stl源码分析](https://github.com/FunctionDou/STL)
## 基本结构
- `_list_node`: 双向链表的节点类，包含了指向前驱节点和后继节点的指针，以及节点的值。该类的实例通常作为 std::list 类中的元素存储在链表中。
- `_list_iterator`: 双向链表的迭代器类，包含了指向当前节点的指针。该类的实例可以用于遍历 std::list 类中的元素，支持前进、后退、取值等操作。
- `_list_const_iterator`: 双向链表的常量迭代器类，与 _list_iterator 类似，但是不能修改迭代器指向的元素。该类的实例可以用于遍历 std::list 类中的元素，支持前进、后退、取值等操作。

### `_list_node`
```c++
template <typename _Tp>
struct _list_node
{
    _list_node<_Tp>* _M_prev; // 指向前驱节点的指针
    _list_node<_Tp>* _M_next; // 指向后继节点的指针
    _Tp _M_data; // 节点的值
};
```

### `_list_iterator`
```c++
template<typename _Tp>
struct _List_iterator
{
    typedef _List_iterator<_Tp> iterator;
    typedef _List_const_iterator<_Tp> const_iterator;
    typedef _List_node<_Tp> _Node;

    // 迭代器的成员变量
    _Node* _M_node; // 指向当前节点的指针

    // 构造函数
    _List_iterator(_Node* __x) : _M_node(__x) {}

    // 重载运算符
    bool operator==(const iterator& __x) const
    {
        return _M_node == __x._M_node;
    }
    ......
};
```
_List_iterator 类包含了一个成员变量 _M_node，表示当前迭代器指向的节点。同时，该类还定义了一些重载运算符，包括 ==、!=、*、->、++、-- 等，支持迭代器的比较、取值、前进、后退等操作。

_List_iterator 类的实例可以用于遍历 std::list 类中的元素，支持前进、后退、取值等操作。在 std::list 类的实现中，每个迭代器都指向一个节点，通过这个节点可以访问到该节点所代表的元素的值。

### `_list_const_iterator`
```c++
template<typename _Tp>
struct _List_const_iterator
{
    typedef _List_const_iterator<_Tp> const_iterator;
    typedef _List_iterator<_Tp> iterator;
    typedef _List_node<_Tp> _Node;

    // 迭代器的成员变量
    const _Node* _M_node; // 指向当前节点的指针

    // 构造函数
    _List_const_iterator(const _Node* __x) : _M_node(__x) {}
    _List_const_iterator(const iterator& __x) : _M_node(__x._M_node) {}

    // 重载运算符
    bool operator==(const const_iterator& __x) const
    {
        return _M_node == __x._M_node;
    }
    ......
```
_List_const_iterator 类与 _List_iterator 类非常类似。
与 _List_iterator 类不同的是，_List_const_iterator 类的成员函数都是 const 的，即不能修改迭代器指向的元素。因此，_List_const_iterator 类的实例可以用于遍历 std::list 类中的元素，但不能修改元素的值。
