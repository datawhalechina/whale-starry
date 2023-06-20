# 简介
源码版本：gcc-4.9.1
```cpp
template<typename _Tp, typename _Alloc = std::allocator<_Tp> >
class list : protected _List_base<_Tp, _Alloc>
{
...
}
```
list 不仅是**双向链表**，而且是**环形双向链表**，继承自_List_base；_Alloc 缺省使用 std::allocator 为配置器。
list 的元素操作包括：push_front, push_back, erase, pop_front, pop_back, clear, remove, unique, splice, merge, reverse, sort
list 相比于 vector 的优点：

1. 每次插入或删除一个元素，就配置或释放一个元素空间。因此，list 对于空间的运用有绝对的精准，不浪费。
2. 对于任何位置的元素插入或元素移除，list 是常数时间。
# list 的节点 
## 节点的结构
```cpp
/// Common part of a node in the %list. 
struct _List_node_base
{
  _List_node_base* _M_next;
  _List_node_base* _M_prev;
}
```
```cpp
/// An actual node in the %list.
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
_List_node 继承自 _List_node_base，其中_List_node 存放数据（_M_data）, _List_node_base 存放双向链表的前指针（_M_prev）和后指针（_M_next）
## 创建节点
```cpp
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
#endif
```
## list 的内存管理
```cpp
// 配置一个节点并传回
_List_node<_Tp>*
_M_get_node()
{ return _M_impl._Node_alloc_type::allocate(1); }
```
```cpp
// 释放一个节点
void
_M_put_node(_List_node<_Tp>* __p) _GLIBCXX_NOEXCEPT
{ _M_impl._Node_alloc_type::deallocate(__p, 1); }
```
# list 的迭代器
```cpp
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
  : _M_node() { }

  explicit
  _List_iterator(__detail::_List_node_base* __x) _GLIBCXX_NOEXCEPT
  : _M_node(__x) { }

  _Self
  _M_const_cast() const _GLIBCXX_NOEXCEPT
  { return *this; }

  // Must downcast from _List_node_base to _List_node to get to _M_data.
  // 对迭代器取值，取的是节点的数据值
  // 为了正确获取节点的数据值，需要进行显示转换，
  // 把基类指针 _List_node_base 转换成 派生类指针 _List_node
  reference
  operator*() const _GLIBCXX_NOEXCEPT
  { return static_cast<_Node*>(_M_node)->_M_data; }

  // 重载->, 迭代器的成员存取运算子
  pointer
  operator->() const _GLIBCXX_NOEXCEPT
  { return std::__addressof(static_cast<_Node*>(_M_node)->_M_data); }

  _Self&
  operator++() _GLIBCXX_NOEXCEPT
  {
	_M_node = _M_node->_M_next;
	return *this;
  }

  // 后+,返回原值的拷贝，然后原值++
  _Self
  operator++(int) _GLIBCXX_NOEXCEPT
  {
	_Self __tmp = *this;
	_M_node = _M_node->_M_next;
	return __tmp;
  }

  _Self&
  operator--() _GLIBCXX_NOEXCEPT
  {
	_M_node = _M_node->_M_prev;
	return *this;
  }

  //后-,返回原值的拷贝，然后原值--
  _Self
  operator--(int) _GLIBCXX_NOEXCEPT
  {
	_Self __tmp = *this;
	_M_node = _M_node->_M_prev;
	return __tmp;
  }

  bool
  operator==(const _Self& __x) const _GLIBCXX_NOEXCEPT
  { return _M_node == __x._M_node; }

  bool
  operator!=(const _Self& __x) const _GLIBCXX_NOEXCEPT
  { return _M_node != __x._M_node; }

  // The only member points to the %list element.
  __detail::_List_node_base* _M_node;
};

```
不同于 vector，list 不支持随机访问，它提供的迭代器是一个 Bidirectional Iterators，支持前移、后移（双向链表）
list 的一个重要性质：插入操作（insert）和接合操作（splice）都不会造成原有的 list 迭代器失效，删除操作（erase）也只有“指向被删除元素”的迭代器失效，其他迭代器不受影响。这个也跟 vector 不同（vector 的插入操作可能造成空间重新配置，导致原有的迭代器全部失效）。
# list 的数据结构
list 不仅是一个双向链表，还是一个环形双向链表。因此只需要一个指针就能完整表示整个链表。list 继承自 _List_base。
```cpp
template<typename _Tp, typename _Alloc = std::allocator<_Tp> >
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
  // 指向-Node的指针可以被ctor转换为迭代器类型。
  // 这个指针 node 如果指向尾端的一个空白节点，
  // 便符合 STL 对于前闭后开区间的要求，成为 last 迭代器
  typedef _List_node<_Tp>				 _Node;

  using _Base::_M_impl;
  using _Base::_M_put_node;
  using _Base::_M_get_node;
  using _Base::_M_get_Tp_allocator;
  using _Base::_M_get_Node_allocator;
...
}
```
# 构造函数
```cpp
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
# list 的常用操作
```cpp
// 插入一个节点作为头节点
void
push_front(const value_type& __x)
{ this->_M_insert(begin(), __x); }
```
```cpp
// 移除头节点
void
pop_front() _GLIBCXX_NOEXCEPT
{ this->_M_erase(begin()); }
```
```cpp
// 插入一个节点作为尾节点
void
push_back(const value_type& __x)
{ this->_M_insert(end(), __x); }
```
```cpp
// 移除尾节点
void
pop_back() _GLIBCXX_NOEXCEPT
{ this->_M_erase(iterator(this->_M_impl._M_node._M_prev)); }
```
```cpp
// 在指定位置插入一个值
iterator
insert(iterator __position, const value_type& __x);

// 在指定位置插入多个值
void
insert(iterator __position, size_type __n, const value_type& __x)
{
list __tmp(__n, __x, get_allocator());
splice(__position, __tmp);
}
```
由于 list 不像 vector 那样有可能在空间不足时做重新配置、数据移动的操作，所以插入前的所有迭代器在插入操作之后都仍然有效。
# sort
由于STL的sort算法只接收RandomAccessIterator，故list需要定义自己的sort函数，并且实现是一个非递归的归并排序。参考：[https://www.cnblogs.com/avota/archive/2016/04/13/5388865.html](https://www.cnblogs.com/avota/archive/2016/04/13/5388865.html)
等价外部实现：
```cpp
void sortList(list<int> &a) {
	if(a.size() <= 1){
		return;
	}
	
	list<int> carry;       // 辅助链表，用于从a中提取元素以及临时保存两个链表的合并结果
	list<int> counter[64]; // 保存着当前每一个归并层次的结果, i号链表保存的元素个数为2的i次方或者0
	int fill = 0;          // 表示当前最大归并排序的层次，while循环之后fill变成log2(a.size())
	
	while (!a.empty()) {
		carry.splice(carry.begin(), a, a.begin()); // 将链表a中的第一个元素移动至carry开头
		int i = 0;
		// 从小往大不断合并非空归并层次直至遇到空层或者到达当前最大归并层次
		while (i < fill && !counter[i].empty()) {  
			counter[i].merge(carry);    // 链表合并，结果链表是有序的，必须保证合并前两个链表是有序的
			carry.swap(counter[i++]);   // 链表元素互换
		}
		carry.swap(counter[i]);
		if (i == fill) {       // i到达当前最大归并层次，说明得增加一层
			++fill;
		}
	}
	
	for (int i = 1; i < fill; ++i) {  // 将所有归并层次的结果合并得到最终结果counter[fill - 1]
		counter[i].merge(counter[i - 1]);
	}
	a.swap(counter[fill - 1]);
}

```
