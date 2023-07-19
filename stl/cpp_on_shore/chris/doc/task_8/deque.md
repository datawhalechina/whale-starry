# This is to learn Deque stl sequential container.
1. *deque*和*vector*, *list*一样, 是stl中的**序列化**容器(*Sequential Container*). 与*vector*是单向开口的连续线性空间不同, *deque*则是一种**双向开口**的**线性连续**空间: 即可以在**头**、**尾**两端分别做元素的**插入**和**删除**操作.
<br>

2. *deque*和*vector*的最大差异在于: *deque*允许常数时间内对头端进行元素的插入或移除操作; *deque*没有容量(*capacity*)概念, 而是**动态分段连续空间**组合, 并且可以**随时增加**一段新的等长内存空间实现扩容; 这与*vector*扩容机制不同, *deque*不会因旧内存空间不足再重新配置一块更大的空间, 然后复制元素再释放旧空间, *deque*也没有空间保留(*reserve*)功能.<br>

3. *deque*的存储空间与我们之间接触到的stl *vector*有类似但是不同: 相同之处是*deque*和*vector*存储元素时内存空间都是**连续**的, 不同之处是*deque*在**两块分段连续空间**的设置, 是"不连续"的, 更像是一个**二维数组**(二维数组的存储空间在一行之内是连续的, 但是行和行之间的存储是不连续的), 如图所示的map数组可以很形象说明deque的存储方式 <br>
   ![deque](deque.png "deque")
于是, 我们可以转换一下思路, deque的结构就可以类比为一个存放**指向不同连续内存空间** *node*的*vector*(我们把它叫做 ***map*** ), 其中的每个*node*都指向了一块连续的内存空间, 被指向的内存空间按叫做 ***buffer*** . 如果*buffer*满, 则*iterator*对应往前或后移动到新的*map*的*node*节点指向的内存空间就可以; 如果*map*满, 则类似于*vector*, 会选取一块内存空间更大的位置, 然后将其中的元素拷贝到新的内存位置上.<br>

4. *deque*的`iterator`, *deque*是一个双端队列, 因此*deque*的*iterator*也应该获取到*deque*的两端地址; 另外, *deque*是分段的连续空间, 它需要指出*buffer*在哪里, 且必须能够判断自己是否已经处于所在*buffer*的边缘, 如果在边缘, 则在前进或者后退时必须跳跃至上一个或者下一个*buffer*, 为了确保跳跃正确, *deque*还需要管理当前位置的*map_pointer*<br>
   以下是deque iterator的源码部分:
   ```
   // 默认的deque的buffer_size是512
   #ifndef _GLIBCXX_DEQUE_BUF_SIZE
   #define _GLIBCXX_DEQUE_BUF_SIZE 512
   #endif 

    // 如果__size小于512, 则返回512/__size
    // 否则, 传回1
    // 如果使用deque<int> dq, 并指定buffer_size为32, 那么每个buffer
    // 可以容纳32/sizeof(int) = 8个元素
    _GLIBCXX_CONSTEXPR inline size_t
     __deque_buf_size(size_t __size)
    { return (__size < _GLIBCXX_DEQUE_BUF_SIZE
	    ? size_t(_GLIBCXX_DEQUE_BUF_SIZE / __size) : size_t(1)); }


    template<typename _Tp, typename _Ref, typename _Ptr>
    struct _Deque_iterator
    {
    private:
      template<typename _Up>
	    using __ptr_to = typename pointer_traits<_Ptr>::template rebind<_Up>;
      template<typename _CvTp>
	    using __iter = _Deque_iterator<_Tp, _CvTp&, __ptr_to<_CvTp>>;
    public:
      typedef __iter<_Tp>		iterator;
      typedef __iter<const _Tp>		const_iterator;
      typedef __ptr_to<_Tp>		_Elt_pointer;
      typedef __ptr_to<_Elt_pointer>	_Map_pointer;
      
      // 一个node指向buffer的存储空间的大小
      static size_t _S_buffer_size() _GLIBCXX_NOEXCEPT
      { return __deque_buf_size(sizeof(_Tp)); }
      
      // iterator部分与之前stl的容器形式一致
      typedef std::random_access_iterator_tag	iterator_category;
      typedef _Tp				value_type;
      typedef _Ptr				pointer;
      typedef _Ref				reference;
      typedef size_t				size_type;
      typedef ptrdiff_t				difference_type;
      typedef _Deque_iterator			_Self;

      // deque iterator的构造函数
      _Deque_iterator(_Elt_pointer __x, _Map_pointer __y) _GLIBCXX_NOEXCEPT
      : _M_cur(__x), _M_first(*__y),
	    _M_last(*__y + _S_buffer_size()), _M_node(__y) { }
      
      // deque iterator的无参构造函数
      _Deque_iterator() _GLIBCXX_NOEXCEPT
      : _M_cur(), _M_first(), _M_last(), _M_node() { }

      ......
      
      // 如果到buffer边缘则需要跳过一个buffer到下一个buffer
      // 也是保证operator ++ -- 可以成功的关键
      void
      _M_set_node(_Map_pointer __new_node) _GLIBCXX_NOEXCEPT
      {
	    _M_node = __new_node;
	    _M_first = *__new_node;
	    _M_last = _M_first + difference_type(_S_buffer_size());
      }
      
      // 可以看到
      _Self&
      operator++() _GLIBCXX_NOEXCEPT
      {
	    ++_M_cur;
	    if (_M_cur == _M_last)
	    {
	        _M_set_node(_M_node + 1); // 移动到下一个buffer
	        _M_cur = _M_first;        // 的第一个元素
	    }
	    return *this;
      }

      _Self&
      operator--() _GLIBCXX_NOEXCEPT
      {
	    if (_M_cur == _M_first)
	    {
	        _M_set_node(_M_node - 1); // 移动到下一个buffer
	        _M_cur = _M_last;         // 的第一个元素
	    }
	    --_M_cur;
	    return *this;
      }

       _Self&
       operator+=(difference_type __n) _GLIBCXX_NOEXCEPT
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
	        _M_set_node(_M_node + __node_offset); // 如果移动过了buffer的界限, 则使用set函数
                                                  //后移
	        _M_cur = _M_first + (__offset - __node_offset
				    * difference_type(_S_buffer_size()));
	    }
	    return *this;
      }

      _Self&
      operator-=(difference_type __n) _GLIBCXX_NOEXCEPT
      { return *this += -__n; } // -=运算符使用了重载的+=来实现功能

    };
   ```

5. *deque*的基类函数如下(其中包含了一些实现的细节, 如buffer_size, map数组的size等等)
   ```
    template<typename _Tp, typename _Alloc>
    class _Deque_base
    {
      protected:
        ......
        typedef typename _Alloc_traits::pointer		_Ptr;
        typedef typename _Alloc_traits::const_pointer	_Ptr_const;

      public:
        typedef _Alloc		  allocator_type;

        allocator_type
        get_allocator() const _GLIBCXX_NOEXCEPT
        { return allocator_type(_M_get_Tp_allocator()); }  

        _Deque_base()
        : _M_impl()
        { _M_initialize_map(0); }

        _Deque_base(size_t __num_elements)
        : _M_impl()
        { _M_initialize_map(__num_elements); }

        ......

      protected:
        typedef typename iterator::_Map_pointer _Map_pointer;


        // 这个struct里面包含了一些deque的具体实现方法
        struct _Deque_impl
        : public _Tp_alloc_type
        {
	        _Map_pointer _M_map;
	        size_t _M_map_size;
	        iterator _M_start;
	        iterator _M_finish;

	        _Deque_impl()
	        : _Tp_alloc_type(), _M_map(), _M_map_size(0),
	          _M_start(), _M_finish() { }

            void _M_swap_data(_Deque_impl& __x) _GLIBCXX_NOEXCEPT
	        {
	        using std::swap;
	        swap(this->_M_start, __x._M_start);
	        swap(this->_M_finish, __x._M_finish);
	        swap(this->_M_map, __x._M_map);
	        swap(this->_M_map_size, __x._M_map_size);
	        }
        };

        // 分配node的内存(map数组中存放的node ptr节点)
        _Ptr
        _M_allocate_node()
        {
	        typedef __gnu_cxx::__alloc_traits<_Tp_alloc_type> _Traits;
	        return _Traits::allocate(_M_impl, __deque_buf_size(sizeof(_Tp)));
        }

        // 释放node的内存(map数组中存放的node ptr节点)
        void
        _M_deallocate_node(_Ptr __p) _GLIBCXX_NOEXCEPT
        {
	        typedef __gnu_cxx::__alloc_traits<_Tp_alloc_type> _Traits;
	        _Traits::deallocate(_M_impl, __p, __deque_buf_size(sizeof(_Tp)));
        }

        // 分配整个map数组(存放全部node指针)的内存
        _Map_pointer
        _M_allocate_map(size_t __n)
        {
	        _Map_alloc_type __map_alloc = _M_get_map_allocator();
	        return _Map_alloc_traits::allocate(__map_alloc, __n);
        }
        
        // 释放整个map数组(存放全部指针)的内存
        void
        _M_deallocate_map(_Map_pointer __p, size_t __n) _GLIBCXX_NOEXCEPT
        {
	        _Map_alloc_type __map_alloc = _M_get_map_allocator();
	        _Map_alloc_traits::deallocate(__map_alloc, __p, __n);
        }


      protected:
        void _M_initialize_map(size_t);
        void _M_create_nodes(_Map_pointer __nstart, _Map_pointer __nfinish);
        void _M_destroy_nodes(_Map_pointer __nstart,
			    _Map_pointer __nfinish) _GLIBCXX_NOEXCEPT;

        // 初始的map_size设置为8
        enum { _S_initial_map_size = 8 };

        _Deque_impl _M_impl;
    ...
    };
   ```
6. *deque*的`map_initializer`函数: 这个函数是完成存放指向连续内存空间的*node*指针的*map*指针数组的初始化的, 其中的参数`__num_elements`指一开始需要给多少个存储在deque中的元素分配内存空间
   ```
    template<typename _Tp, typename _Alloc>
      void
        _Deque_base<_Tp, _Alloc>::
        _M_initialize_map(size_t __num_elements)
        {
            const size_t __num_nodes = (__num_elements/ __deque_buf_size(sizeof(_Tp))
				  + 1);

            this->_M_impl._M_map_size = std::max((size_t) _S_initial_map_size,
					   size_t(__num_nodes + 2));
            this->_M_impl._M_map = _M_allocate_map(this->_M_impl._M_map_size);
            /*
                对于"需要的节点内存空间小于整个map所存放节点数目"的小map, 
                内存分配开始于中间的元素并且向外生长
                因此nStart可能起始于_M_map,
                但是对于小map来说, nStart可能开始于_M_map+3
            */

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

7. *deque*的构造和内存管理: *deque*的*buffer*扩充操作是比较复杂的, 其中主要是用到`_M_fill_initialize()`函数来初始化*deque*的结构, 并且将元素的初始值赋好. `_M_fill_initialize()`函数的定义如下:
   ```
    template <typename _Tp, typename _Alloc>
    void
    deque<_Tp, _Alloc>::
    _M_fill_initialize(const value_type& __value)
    {
      _Map_pointer __cur;
      __try
        {
          for (__cur = this->_M_impl._M_start._M_node;
	       __cur < this->_M_impl._M_finish._M_node;
	       ++__cur)
                std::__uninitialized_fill_a(*__cur, *__cur + _S_buffer_size(),
					    __value, _M_get_Tp_allocator());
          std::__uninitialized_fill_a(this->_M_impl._M_finish._M_first,
				      this->_M_impl._M_finish._M_cur,
				      __value, _M_get_Tp_allocator());
        }
      __catch(...)
        {
          std::_Destroy(this->_M_impl._M_start, iterator(*__cur, __cur),
			_M_get_Tp_allocator());
          __throw_exception_again;
        }
    }
   ```
8. `push_back()`和`pop_back()`等deque常用函数
   ```
    // push_back() definition
    // 越过buffer的会调用_M_push_back_aux()函数, 保证整体deque的相对位置
    void
    push_back(const value_type& __x)
    {
	    if (this->_M_impl._M_finish._M_cur
	        != this->_M_impl._M_finish._M_last - 1)
	    {
	        _Alloc_traits::construct(this->_M_impl,
			    	     this->_M_impl._M_finish._M_cur, __x);
	        ++this->_M_impl._M_finish._M_cur;
	    }
	    else
	        _M_push_back_aux(__x);
    }

    // pop_back() definition
    void
    pop_back() _GLIBCXX_NOEXCEPT
    {
        // 首先检查deque内部是否为空
	    __glibcxx_requires_nonempty();
	    if (this->_M_impl._M_finish._M_cur
	        != this->_M_impl._M_finish._M_first) // 检查是否pop过后当前buffer内的元素是否为空
	    {
	        --this->_M_impl._M_finish._M_cur;
	        _Alloc_traits::destroy(this->_M_impl,
			    	   this->_M_impl._M_finish._M_cur);
	    }
	    else
	        _M_pop_back_aux();
    }

   ```
9. *deque*的`at`和`[]`：
   - 都用于访问deque中某个索引的元素
   - 两者之间有明显的区别, `[]`没有索引检查, 而`at`有索引检查<br>

   代码如下：
   ```
    // []的源码
    reference
    operator[](size_type __n) _GLIBCXX_NOEXCEPT
    {
	    __glibcxx_requires_subscript(__n);
	    return this->_M_impl._M_start[difference_type(__n)];
    }

    // at的源码 其中调用的_M_range_check函数如下
    // Safety check used only from at().
    void
    _M_range_check(size_type __n) const
    {
	    if (__n >= this->size())
        // 如果越界, 会抛出异常
	    __throw_out_of_range_fmt(__N("deque::_M_range_check: __n "
				       "(which is %zu)>= this->size() "
				       "(which is %zu)"),
				    __n, this->size());
    }

    // at definition
    reference
    at(size_type __n)
    {
	    _M_range_check(__n);
	    return (*this)[__n];
    }

   ```

10. deque的`insert()`函数:
    源码实现如下, 以_M_range_insert_aux为例(*bits/stl_deque.h*和*bits/deque.tcc*)
    ```
    // from bits/deque.tcc
    template <typename _Tp, typename _Alloc>
    template <typename _ForwardIterator>
      void
      deque<_Tp, _Alloc>::
      _M_range_insert_aux(iterator __pos,
                          _ForwardIterator __first, _ForwardIterator __last,
                          std::forward_iterator_tag)
      {
        const size_type __n = std::distance(__first, __last);
        if (__pos._M_cur == this->_M_impl._M_start._M_cur)
	  {
	    iterator __new_start = _M_reserve_elements_at_front(__n);
	    __try
	      {
		std::__uninitialized_copy_a(__first, __last, __new_start,
					    _M_get_Tp_allocator());
		this->_M_impl._M_start = __new_start;
	      }
	    __catch(...)
	      {
		_M_destroy_nodes(__new_start._M_node,
				 this->_M_impl._M_start._M_node);
		__throw_exception_again;
	      }
	  }
        else if (__pos._M_cur == this->_M_impl._M_finish._M_cur)
	  {
	    iterator __new_finish = _M_reserve_elements_at_back(__n);
	    __try
	      {
		std::__uninitialized_copy_a(__first, __last,
					    this->_M_impl._M_finish,
					    _M_get_Tp_allocator());
		this->_M_impl._M_finish = __new_finish;
	      }
	    __catch(...)
	      {
		_M_destroy_nodes(this->_M_impl._M_finish._M_node + 1,
				 __new_finish._M_node + 1);
		__throw_exception_again;
	      }
	  }
        else
          _M_insert_aux(__pos, __first, __last, __n);
      }

    // from stl_deque.h
    iterator
    insert(const_iterator __p, initializer_list<value_type> __l)
    {
	    auto __offset = __p - cbegin();
	    _M_range_insert_aux(__p._M_const_cast(), __l.begin(), __l.end(),
		    	    std::random_access_iterator_tag());
	    return begin() + __offset;
    }
    ```
    