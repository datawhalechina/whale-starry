## 双端队列-Deque学习笔记

- **关于Deque的基础概念**

	deque是一个双端队列，连续容器，支持随机访问，支持在头尾两端插入和删除元素，支持动态扩容，支持自动缩容。但实际上所谓连续只是在外界看上去连续，底层的机制是当我们进行头插或者尾插超过了预定义的容量时，会申请一块内存缓冲区存放新插入的元素，然后有点类似链表的形式把内存连接起来，看上去就是连续的，实际上是通过一个map存储每段连续空间的地址，然后通过表查找。

	因此我们可以看到deque只有4个成员变量：
	```c++
	struct _Deque_impl: public _Tp_alloc_type
    {
	  _Map_pointer _M_map;  // 储存连续空间地址
	  size_t _M_map_size;   // 连续空间的数量
	  iterator _M_start;    // 迭代器start标记实现push_front
	  iterator _M_finish;   // 迭代器finish标记实现push_back
	}
	```

	而deque_iterator主要是四个成员变量：
	```c++
	template<typename _Tp, typename _Ref, typename _Ptr>
    struct _Deque_iterator{
	  typedef _Tp*   _Elt_pointer;
      typedef _Tp**  _Map_pointer;
	  _Elt_pointer _M_cur;     // 指向要插入的位置
      _Elt_pointer _M_first;   // 指向连续空间的头
      _Elt_pointer _M_last;    // 指向连续空间的尾
      _Map_pointer _M_node;    // 指向map
	}
	```

	map的初始化：
	```c++
	template<typename _Tp, typename _Alloc>
    void
    _Deque_base<_Tp, _Alloc>::
    _M_initialize_map(size_t __num_elements)
    {
      const size_t __num_nodes = (__num_elements / __deque_buf_size(sizeof(_Tp))
				  + 1);

      this->_M_impl._M_map_size = std::max((size_t) _S_initial_map_size,
					   size_t(__num_nodes + 2));
      this->_M_impl._M_map = _M_allocate_map(this->_M_impl._M_map_size);

      _Map_pointer __nstart = (this->_M_impl._M_map
			       + (this->_M_impl._M_map_size - __num_nodes) / 2);
      _Map_pointer __nfinish = __nstart + __num_nodes;

      __try
	  { _M_create_nodes(__nstart, __nfinish); }
      __catch(...)
	  {
	    _M_deallocate_map(this->_M_impl._M_map, this->_M_impl._M_map_size);
	    this->_M_impl._M_map = _Map_pointer();
	    this->_M_impl._M_map_size = 0;
	    __throw_exception_again;
	  }

      this->_M_impl._M_start._M_set_node(__nstart);
      this->_M_impl._M_finish._M_set_node(__nfinish - 1);
      this->_M_impl._M_start._M_cur = _M_impl._M_start._M_first;
      this->_M_impl._M_finish._M_cur = (this->_M_impl._M_finish._M_first
					+ __num_elements
					% __deque_buf_size(sizeof(_Tp)));
    }
	```

- **基本迭代器实现**

	```c++
	reference operator*() const _GLIBCXX_NOEXCEPT
    { return *_M_cur; }

    pointer operator->() const _GLIBCXX_NOEXCEPT
    { return _M_cur; }

    _Self& operator++() _GLIBCXX_NOEXCEPT
    {
	  ++_M_cur;
	  if (_M_cur == _M_last)
	  {
	    _M_set_node(_M_node + 1);
	    _M_cur = _M_first;
	  }
	  return *this;
    }

    _Self operator++(int) _GLIBCXX_NOEXCEPT
    {
	  _Self __tmp = *this;
	  ++*this;
	  return __tmp;
    }

    _Self& operator--() _GLIBCXX_NOEXCEPT
    {
	  if (_M_cur == _M_first)
	  {
	    _M_set_node(_M_node - 1);
	    _M_cur = _M_last;
	  }
	  --_M_cur;
	  return *this;
    }

    _Self operator--(int) _GLIBCXX_NOEXCEPT
    {
	  _Self __tmp = *this;
      --*this;
      return __tmp;
    }

	_Self& operator+=(difference_type __n) _GLIBCXX_NOEXCEPT
    {
	  const difference_type __offset = __n + (_M_cur - _M_first);
	  if (__offset >= 0 && __offset < difference_type(_S_buffer_size()))
	    _M_cur += __n;
	  else
	  {
	    const difference_type __node_offset =
	      __offset > 0 ? __offset / difference_type(_S_buffer_size())
			   : -difference_type((-__offset - 1)
					      / _S_buffer_size()) - 1;
	    _M_set_node(_M_node + __node_offset);
	    _M_cur = _M_first + (__offset - __node_offset
				 * difference_type(_S_buffer_size()));
	  }
	  return *this;
    }

    _Self operator+(difference_type __n) const _GLIBCXX_NOEXCEPT
    {
	  _Self __tmp = *this;
	  return __tmp += __n;
    }

    _Self& operator-=(difference_type __n) _GLIBCXX_NOEXCEPT
    { return *this += -__n; }

    _Self operator-(difference_type __n) const _GLIBCXX_NOEXCEPT
    {
	  _Self __tmp = *this;
	  return __tmp -= __n;
    }

    reference operator[](difference_type __n) const _GLIBCXX_NOEXCEPT
    { return *(*this + __n); }
	```

- **一些关键函数接口**

	- 头插push_front

		```c++
		void push_front(const value_type& __x)
		{
		if (this->_M_impl._M_start._M_cur != this->_M_impl._M_start._M_first)
		{
			_Alloc_traits::construct(this->_M_impl,
						this->_M_impl._M_start._M_cur - 1,
						__x);
			--this->_M_impl._M_start._M_cur;
		}
		else
		_M_push_front_aux(__x);
		}

		// 注意
		/*如果第一缓冲区并无备用空间，那么就会调用push_front_aux()此函式一开始即呼叫 _M_reserve_map_at_front()，后者用来判断是否需要扩充map，如有需要就付诸行动。稍后我会呈现_M_reserve_map_at_front() 的函式内容*/
		```
	
	- 尾插push_buck

		```c++
		void push_back(const value_type& __x)
        {
		  if (this->_M_impl._M_finish._M_cur != this->_M_impl._M_finish._M_last - 1)
		  {
		    _Alloc_traits::construct(this->_M_impl, this->_M_impl._M_finish._M_cur, __x);
			++this->_M_impl._M_finish._M_cur;
		  }
		  else
			_M_push_back_aux(__x);
		}
		/*
			当尾端没有剩余空间或者只剩一个元素备用空间，push_back()就会调用push_back_aux()， 先配置一整块新的缓冲区，再设妥新元素内容，然后更改迭代器finish的状态
		*/
		```
	
	- 其他类似与vector一致的接口就不一一记录了