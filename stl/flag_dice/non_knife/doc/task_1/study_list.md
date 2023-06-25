## 类结构
`list`继承`_List_base`
```C++
template<typename _Tp, typename _Alloc = std::allocator<_Tp> >
class list : protected _List_base<_Tp, _Alloc> {}
```

## 链表节点结构
从基类`_List_node_base`中可知STL list是双向链表，`_M_prev`指向前置节点，`_M_next`指向后继节点。
```C++
struct _List_node_base{
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
}
```
同时，派生类`_List_node`中`_M_data`用于存放数据项
```C++
struct _List_node : public __detail::_List_node_base{
    _Tp _M_data;
}
```
链表的首节点为空
```C++
_List_node_header() _GLIBCXX_NOEXCEPT{ _M_init(); }
```
```C++
void _M_init() _GLIBCXX_NOEXCEPT {
	this->_M_next = this->_M_prev = this;
#if _GLIBCXX_USE_CXX11_ABI
	this->_M_size = 0;
#endif
}
```
## list接口
### 默认构造
用户可自定义内存分配器，内存分配器类型通过模板参数传入，内存分配器实例通过函数参数传入。`_M_get_node`从内存分配器中申请一个链表节点`_List_node<_Tp>`大小的内存。使用默认构造函数时，`list`中并没有任何元素，因此`_M_get_node`中只申请内存，但并不在其上构造`_Tp`对象，即该节点是空的哨兵节点。
```C++
explicit list(const allocator_type& __a) _GLIBCXX_NOEXCEPT
: _Base(_Node_alloc_type(__a)) { }

_List_base(const allocator_type&) {
    _M_node = _M_get_node();
    _M_node->_M_next = _M_node;
    _M_node->_M_prev = _M_node;
}
```
### 从元素个数与值构造
连续向`list`中插入`__n`个值为`__value`的元素
```C++
// 包含批量版本，一次插入n条数据和非批量版本，一次插入1条数据
explicit list(size_type __n, const value_type &__value = value_type(),const allocator_type &__a = allocator_type()) : _Base(_Node_alloc_type(__a)) { _M_fill_initialize(__n, __value); }
```
### 从容器区间构造
遍历容器区间`[__first, __last]`，依次将元素插入`list`中
```C++
template<typename _InputIterator,typename = std::_RequireInputIter<_InputIterator>>
	list(_InputIterator __first, _InputIterator __last,
	     const allocator_type& __a = allocator_type())
	: _Base(_Node_alloc_type(__a))
	{ _M_initialize_dispatch(__first, __last, __false_type()); }
```
## 成员函数和成员变量
- 迭代器（Iterators）：`begin()`、`end()`、`rbegin()`、`rend()`等函数返回迭代器，用于遍历链表中的元素。  

- 容量（Capacity）：`empty()`函数用于检查链表是否为空，`size()`函数返回链表中元素的个数。  

- 访问元素（Element Access）：`front()`函数返回第一个元素的引用，`back()`函数返回最后一个元素的引用。  

- 修改容器（Modifier）：`push_back()`和`push_front()`函数用于在链表的末尾和开头插入元素，`pop_back()`和`pop_front()`函数用于移除末尾和开头的元素。  

- 插入和删除（Insertion and Deletion）：`insert()`函数用于在指定位置插入元素，erase()函数用于移除指定位置的元素。  

- 清空容器（Clearing the Container）：`clear()`函数用于移除链表中的所有元素。  

- 其他操作：`swap()`函数用于交换两个链表容器的内容，`splice()`函数用于将一个链表的元素移动到另一个链表中。