# This is the second class -- vector
1. `vector`分配的是动态的空间，可以根据新元素的加入，内部机制允许可以扩容并添加新元素，相比于stl库中的array是分配的静态空间不能改变的特点要更灵活

2. `vector`定义的成员变量如下

```
// allocator 用于为vector数据结构分配内存空间
  template<typename _Tp, typename _Alloc = std::allocator<_Tp> >
    class vector : protected _Vector_base<_Tp, _Alloc>
    {
      // Concept requirements.
      typedef typename _Alloc::value_type                _Alloc_value_type;
      
      typedef _Vector_base<_Tp, _Alloc>			 _Base;
      typedef typename _Base::_Tp_alloc_type		 _Tp_alloc_type;
      typedef __gnu_cxx::__alloc_traits<_Tp_alloc_type>  _Alloc_traits;

    public:
      typedef _Tp					 value_type;
      typedef typename _Base::pointer                    pointer;
      typedef typename _Alloc_traits::const_pointer      const_pointer;
      typedef typename _Alloc_traits::reference          reference;
      typedef typename _Alloc_traits::const_reference    const_reference;
      // 定义iterator
      typedef __gnu_cxx::__normal_iterator<pointer, vector> iterator;
      typedef __gnu_cxx::__normal_iterator<const_pointer, vector>
      const_iterator;
      typedef std::reverse_iterator<const_iterator>  const_reverse_iterator;
      typedef std::reverse_iterator<iterator>		 reverse_iterator;
      typedef size_t					 size_type;
      typedef ptrdiff_t					 difference_type;
      typedef _Alloc  

    ...
    };  
```

3. `vector`中的`iterator`迭代器，可以用来访问vector中的元素，本质上和list内部的iterator一样，是一个指针，但是与list不同之处在于，list有prev和next两个指针，但是vector只需要一个,定义如下

```
typedef __gnu_cxx::__normal_iterator<pointer, vector> iterator;
      typedef __gnu_cxx::__normal_iterator<const_pointer, vector>
      const_iterator;
```

4. `vector`中的初始化函数是, 使用了`_M_fill_initialize`, 这是从uninitialize_stl库中调用的`__uninitialized_fill_n_a`函数, 定义如下：

```
vector(initializer_list<value_type> __l,
	     const allocator_type& __a = allocator_type())
      : _Base(__a)
      {
	_M_range_initialize(__l.begin(), __l.end(),
			    random_access_iterator_tag());
      }
```

需要先实现begin(), end()两个迭代器指针，其中的`random_access_iterator`在下一条会讲到

5. **vector**维护的是一个**连续线性**空间，所以不管其元素型别为何，普通指针都可以作为vector的迭代器而满足所有必要条件，因为vector迭代器所有的操作行为，如`operator*, operator->, operator++, operator--, operator+, operator-, operator+=, operator-=`, 这些普通指针都具备。另外, **vector**支持随机存取, 而普通指针有着这样的能力, 所以**vector**提供**Random Access Iterator**, 初始化类型也就使用**Random Access Iterator**了
e.g. 

```
template <class T, class Alloc = alloc>
class vector{
public:
typedef T value_type;
typedef value_type* iterator; //vector迭代器都是普通指针
};
```

因此，代码中出现例如`vector<int>::iterator number_ptr`，number_ptr的类型就是int*

6. **vector**采用的数据结构是一块**线性**且**连续**的内存空间, 其中有两个迭代器`_M_start`和`_M_finish`分别指向配置得来的连续空间中所被使用的范围，另外还实用迭代器`_M_end_of_storage`指向整块连续空间(其中也包含备用空间)的尾端
迭代器寻址范围, 左闭右开
这三个iterator定义在`_Vector_base`下

```
template<typename _Tp, typename _Alloc>
    struct _Vector_base
    {
      typedef typename __gnu_cxx::__alloc_traits<_Alloc>::template
        rebind<_Tp>::other _Tp_alloc_type;
      typedef typename __gnu_cxx::__alloc_traits<_Tp_alloc_type>::pointer
       	pointer;

      struct _Vector_impl 
      : public _Tp_alloc_type
      {
	pointer _M_start; // 连续空间开始的迭代器，也是指针
	pointer _M_finish; // vector数据结束的迭代器，也是指针
	pointer _M_end_of_storage; // 整块连续空间的尾端

	_Vector_impl()
	: _Tp_alloc_type(), _M_start(0), _M_finish(0), _M_end_of_storage(0)
	{ }
    ...
    }
    ...
};
```

7. 为了降低分配内存空间时的数独成本, **vector**实际配置的大小可能会比客户端需求的大小更大一些，以备将来可能会出现的扩充(这点也解释了为什么`_M_end_of_storage_`迭代器虽然指向连续空间的尾端但是何`_M_finish`不会永远一致), 这就是容量(capacity), **vector**的容量永远大于或者等于他存放数据量的大小(也就是`_M_finish`).当容量等于大小时会发生满载，下次再有新增元素时，整个**vector**就需要再分配其他的内存空间

8. 有了`_M_start, _M_finish, _M_end_of_storage`, 三个迭代器, vector的功能就可以轻松的实现了，包括首位标识, 大小, 容量, 空容器检查, 运算符, 首元素, 尾元素等等
e.g. 实现

```
iterator
      begin() _GLIBCXX_NOEXCEPT
      { return iterator(this->_M_impl._M_start); }

iterator
      end() _GLIBCXX_NOEXCEPT
      { return iterator(this->_M_impl._M_finish); }
// 使用_M_finish指针减去_M_start指针即可得出整个大小
size_type
      size() const _GLIBCXX_NOEXCEPT
      { return size_type(this->_M_impl._M_finish - this->_M_impl._M_start); } 

size_type
      capacity() const _GLIBCXX_NOEXCEPT
      { return size_type(this->_M_impl._M_end_of_storage
			 - this->_M_impl._M_start); } // capacity实现时使用了_M_end_of_storage是vector实际的内存
             // 大小，与_M_finish指向最后元素的下一个地址不同

bool
      empty() const _GLIBCXX_NOEXCEPT
      { return begin() == end(); }

reference
      operator[](size_type __n) _GLIBCXX_NOEXCEPT
      { return *(this->_M_impl._M_start + __n); } // []操作符返回元素的引用
```

9. at和[]运算符的区别, at比[]运算符更为安全，因为他检查了索引和合法性 定义如下:

```
void
      _M_range_check(size_type __n) const
      {
	if (__n >= this->size())
	  __throw_out_of_range_fmt(__N("vector::_M_range_check: __n "
				       "(which is %zu) >= this->size() "
				       "(which is %zu)"),
				   __n, this->size());
      }
...
 reference
      at(size_type __n)
      {
	_M_range_check(__n);
	return (*this)[__n]; 
      }
```

而运算符如第9条，没有进行检查，所以用at会更安全(access safety)一些

10. **vector**的`push_back`: 当我们用`push_back`将新元素插入`vector`尾端时，该函数会先检查是否还有备用空间，如果有备用空间就会直接在备用空间上构造元素，并且调整迭代器finish, 使得vector变大，如果没有足够的备用空间，就扩充空间(重新配置、移动数据、释放原空间), 定义如下

```
void push_back(const value_type& __x)
      {
	if (this->_M_impl._M_finish != this->_M_impl._M_end_of_storage)
	  {
	    _Alloc_traits::construct(this->_M_impl, this->_M_impl._M_finish,
	                             __x);
	    ++this->_M_impl._M_finish;
	  }
	else
#if __cplusplus >= 201103L
	  _M_emplace_back_aux(__x);
#else
	  _M_insert_aux(end(), __x);
#endif
      }
```

11. **vector**元素操作: pop_back, erase, clear等

```

// e.g 简化后的版本
void pop_back(){
    --_M_finish;
    destroy(_M_finish);
}

iterator erase(iterator first, iterator last){
    iterator i = copy(last, _M_finish, first); // 拷贝 全局函数
    destroy(i, _M_finish); // 销毁 全局函数
    _M_finish = finish - (last - first);
    return first;
}

iterator erase(iterator position) {
    if(position + 1 != end())
        copy(position + 1, _M_finish, position); // 拷贝
    --_M_finish;
    destroy(_M_finish);
    return position;
}

void clear(){ erase(begin(), end()); }

```

