# List

**stl::list is a doubly linked list with iterator** 

@brief A standard container with linear time access to elements, and fixed time insertion/deletion at any point in the sequence.

### Design & Structure

1. relationship 

function signature： 

`template<typename _Tp, typename _Alloc = std::allocator<_Tp> >
class list : protected _List_base<_Tp, _Alloc>`

嗯关系非常复杂： 

![Untitled](List%20d5eb667438694baa958d19ddf522c9d8/Untitled.png)

我认为需要在意的是 list 继承_List_base, _List_base 里面有_List_impl, 实际上我们平时写的listnode就在里面，很怪的地方是_List_node数据层居然是继承指针层，我不理解的。 

 list里面需要注意的参数： 

```cpp
template<typename _Tp, typename _Alloc = std::allocator<_Tp> >
    class list : protected _List_base<_Tp, _Alloc>
    {
// C++11 stl用了overhead很大的allocator, 还蛮浪费空间的
typedef typename _Alloc::value_type		_Alloc_value_type; 
typedef _List_base<_Tp, _Alloc>			_Base; // 这个_Base就很重要
protected:
      // Note that pointers-to-_Node's can be ctor-converted to
      // iterator types.
      typedef _List_node<_Tp>				 _Node;

      using _Base::_M_impl;
      using _Base::_M_put_node;
      using _Base::_M_get_node;
      using _Base::_M_get_Node_allocator;

// 实际上就跟上图一样有个_M_impl, 然后能够拿到prev, next, _M_data
```

到了4.9 （c++11）有好多继承，很怪。 

1. shape of data structure 

![Untitled](List%20d5eb667438694baa958d19ddf522c9d8/Untitled%201.png)

实际上list有存这个dummy empty node,  因此才会有判断list empty()和 begin() & end()

```cpp

// head node point next to itself, means it is empty
bool empty() const _GLIBCXX_NOEXCEPT
{ return this->_M_impl._M_node._M_next == &this->_M_impl._M_node; }
// iterators
      /**
       *  Returns a read/write iterator that points to the first element in the
       *  %list.  Iteration is done in ordinary element order.
       */
iterator begin() _GLIBCXX_NOEXCEPT
{ return iterator(this->_M_impl._M_node._M_next); }

/**
       *  Returns a read/write iterator that points one past the last
       *  element in the %list.  Iteration is done in ordinary element
       *  order.
       */
iterator end() _GLIBCXX_NOEXCEPT
      { return iterator(&this->_M_impl._M_node); }

// so it means &this->_M_impl._M_node is the empty one. 
```

```cpp
//based on implementatin, it is easy to know interator begin() is head node 
// end() is an implicit empty node to get exist tail node
// (but what about the list is empty???)

// element access
      /**
       *  Returns a read/write reference to the data at the first
       *  element of the %list.
       */
reference front() _GLIBCXX_NOEXCEPT
      { return *begin(); }
/**
       *  Returns a read/write reference to the data at the last element
       *  of the %list.
       */
reference back() _GLIBCXX_NOEXCEPT
{
	iterator __tmp = end();
	--__tmp;
	return *__tmp;
 }
```

### Some funny Functions

1. **push_font & emplace_front** 

```cpp
// usage 
list<pair<int,int>> li;
li.push_front({23,1});　　　　//
li.emplace_front(23,1);　　　　//
//li.push_front(1,2) ;　　　　//invalid 
//li.emplace_front({1,2}) ;　//invalid
li.emplace_front(make_pair(1,2));　　//
li.push_front(make_pair(23,1));　　　//

// the reason why they are different:
// based on implementation: 
void push_front(value_type&& __x)
      { this->_M_insert(begin(), std::move(__x)); }

// difference results from ... _args
void emplace_front(_Args&&... __args) 
	{
	  this->_M_insert(begin(), std::forward<_Args>(__args)...);
```

### Iterator

Attention: iterator is a smart pointer.  iterator ++  应该是到node里面去找到指向next node的指针，然后移动，与array && vector的内存空间移动不同， 主要原因是运算符重载。 

```cpp
// Example for Operator Overloading 
_Self& operator++() _GLIBCXX_NOEXCEPT
{
	_M_node = _M_node->_M_next;
	return *this;
}
// it return a reference, because ++++iter == ++(++iter) is valid

_Self operator++(int) _GLIBCXX_NOEXCEPT
{
	_Self __tmp = *this;
	_M_node = _M_node->_M_next;
	return __tmp;
 }
// it return an object, because iter++++ is invalid 

/**
attention
The iterator for == and != are friend functions
**/
friend bool operator==(const _Self& __x, const _Self& __y) _GLIBCXX_NOEXCEPT
  { return __x._M_node == __y._M_node; }

friend bool operator!=(const _Self& __x, const _Self& __y) _GLIBCXX_NOEXCEPT
  { return __x._M_node != __y._M_node; }
// NO_EXCEPT only works after C++11
```


未完待续。


Reference: 

[https://github.com/gcc-mirror/gcc/blob/releases/gcc-9/libstdc%2B%2B-v3/include/bits/stl_list.h](https://github.com/gcc-mirror/gcc/blob/releases/gcc-9/libstdc%2B%2B-v3/include/bits/stl_list.h)

[https://www.bilibili.com/video/BV1wh4y1o7e9?p=15&spm_id_from=pageDriver&vd_source=230d747e48c21fec29929cea8d7c91de](https://www.bilibili.com/video/BV1wh4y1o7e9?p=15&spm_id_from=pageDriver&vd_source=230d747e48c21fec29929cea8d7c91de)

