## list中的算法：

#### _M_transfer

将[first, last) 中的元素移到position之前。

[libstdc++-v3/include/bits/stl_list.h](https://sourcegraph.com/github.com/gyfffffff/gcc@7360cba833cd921631818428a851e03ea88f1e8a/-/blob/libstdc++-v3/include/bits/stl_list.h?L1988:7-1988:18)

      // Moves the elements from [first,last) before position.
      void
      _M_transfer(iterator __position, iterator __first, iterator __last)
      { __position._M_node->_M_transfer(__first._M_node, __last._M_node); }

[libstdc++-v3/src/c++98/list.cc](https://sourcegraph.com/github.com/gyfffffff/gcc@7360cba833cd921631818428a851e03ea88f1e8a/-/blob/libstdc++-v3/src/c++98/list.cc?L94:5-94:16)

```C++
void
_List_node_base::
_M_transfer(_List_node_base * const __first,
	_List_node_base * const __last) _GLIBCXX_USE_NOEXCEPT
{
  __glibcxx_assert(__first != __last);

  if (this != __last)
{
  // Remove [first, last) from its old position.
  __last->_M_prev->_M_next  = this;
  __first->_M_prev->_M_next = __last;
  this->_M_prev->_M_next    = __first;

  // Splice [first, last) into its new position.
  _List_node_base* const __tmp = this->_M_prev;
  this->_M_prev                = __last->_M_prev;
  __last->_M_prev              = __first->_M_prev;
  __first->_M_prev             = __tmp;
}
}
```
![](./images/_M_transfer visualization.jpg)

#### merge 

include/bits/list.tcc

```C++
  template <typename _Tp, typename _Alloc>
  void list<_Tp, _Alloc>::
#if __cplusplus >= 201103L
      merge(list && __x)
#else
      merge(list & __x)
#endif
  {
    // _GLIBCXX_RESOLVE_LIB_DEFECTS
    // 300. list::merge() specification incomplete
    if (this != &__x) {
      _M_check_equal_allocators(__x);

      iterator __first1 = begin();
      iterator __last1 = end();
      iterator __first2 = __x.begin();
      iterator __last2 = __x.end();
      while (__first1 != __last1 && __first2 != __last2)
        if (*__first2 < *__first1) {
          iterator __next = __first2;
          _M_transfer(__first1, __first2, ++__next);
          __first2 = __next;
        } else
          ++__first1;
      if (__first2 != __last2) _M_transfer(__last1, __first2, __last2);
    }
  }
```

first2永远指向第二条list第一个元素。

比较两条链表第一个数据大小，如果first2小，就把first2指向的元素移到first1之前；如果first2大于等于，就后移first1指针。如果任何一个first和last重合，就退出while循环。

坚持是不是第二条链表重合，如果是，就把第二条链表的剩余部分全部插入第一条末尾。

#### sort 排序

```C++
  template <typename _Tp, typename _Alloc>
  void list<_Tp, _Alloc>::sort() {
    // Do nothing if the list has length 0 or 1.
    if (this->_M_impl._M_node._M_next != &this->_M_impl._M_node &&
        this->_M_impl._M_node._M_next->_M_next != &this->_M_impl._M_node) {
      list __carry;
      list __tmp[64];  // 链表数组
      list* __fill = &__tmp[0];  // 指向链表数组
      list* __counter;   // 链表指针

      do {
        __carry.splice(__carry.begin(), *this, begin());

        for (__counter = &__tmp[0]; __counter != __fill && !__counter->empty();
             ++__counter) {
          __counter->merge(__carry);
          __carry.swap(*__counter);
        }
        __carry.swap(*__counter);
        if (__counter == __fill) ++__fill;
      } while (!empty());

      for (__counter = &__tmp[1]; __counter != __fill; ++__counter)
        __counter->merge(*(__counter - 1));
      swap(*(__fill - 1));
    }
  }

       /**
       *  @brief  Insert element from another %list.
       *  @param  __position  Const_iterator referencing the element to
       *                      insert before.
       *  @param  __x  Source list.
       *  @param  __i  Const_iterator referencing the element to move.
       *
       *  Removes the element in list @a __x referenced by @a __i and
       *  inserts it into the current list before @a __position.
       */
      void
      splice(const_iterator __position, list&& __x, const_iterator __i) noexcept
          
       /**
       *  @brief  Swaps data with another %list.
       *  @param  __x  A %list of the same element and allocator types.
       *
       *  This exchanges the elements between two lists in constant
       *  time.  Note that the global std::swap() function is
       *  specialized such that std::swap(l1,l2) will feed to this
       *  function.
       */
      void
      swap(list& __x)
```

有点难以理解，还没看完。。
