#  Vector容器（动态数组）

##  1 视频中G2.9版



![image-20230624220318054](C:\Users\Ga\AppData\Roaming\Typora\typora-user-images\image-20230624220318054.png)

动态进行扩容：在内存找到另外的空间，二倍扩容。

关键部分：

![image-20230624220711540](C:\Users\Ga\AppData\Roaming\Typora\typora-user-images\image-20230624220711540.png)

32位电脑上，占12个字节的大小，因为protected的三个指针。容器是前闭后开区间

![image-20230624223240264](C:\Users\Ga\AppData\Roaming\Typora\typora-user-images\image-20230624223240264.png)

![image-20230624224116592](C:\Users\Ga\AppData\Roaming\Typora\typora-user-images\image-20230624224116592.png)

## 2 G4.9版

![image-20230624224428570](C:\Users\Ga\AppData\Roaming\Typora\typora-user-images\image-20230624224428570.png)

占用内存：三个指针，12个字节

![image-20230624230324365](C:\Users\Ga\AppData\Roaming\Typora\typora-user-images\image-20230624230324365.png)

## 3 具体函数定义

### （1） _Vector_base类



```
__gnu_cxx::__alloc_traits
```

`__gnu_cxx::__alloc_traits`是GNU C++标准库中的一个模板类，用于提供分配器的特性和功能。

在C++中，分配器是用于管理内存分配和释放的对象。`__gnu_cxx::__alloc_traits`模板类为分配器提供了一组标准化的接口，使得可以通过这些接口来访问和操作分配器的相关属性和行为。

`__gnu_cxx::__alloc_traits`模板类提供了以下几个重要的成员函数：

1. `static pointer allocate(allocator_type& __a, size_t __n)`：分配`__n`个对象的内存空间，并返回指向分配的内存的指针。
2. `static void deallocate(allocator_type& __a, pointer __p, size_t __n)`：释放通过分配器`__a`分配的指针`__p`指向的内存空间，该空间的大小为`__n`个对象。
3. `static void construct(allocator_type& __a, pointer __p, const _Tp& __val)`：在指定的内存位置`__p`上构造一个对象，使用给定的值`__val`进行初始化。
4. `static void destroy(allocator_type& __a, pointer __p)`：销毁指定内存位置`__p`上的对象，调用对象的析构函数进行清理。

通过使用`__gnu_cxx::__alloc_traits`模板类，可以使得分配器的使用更加灵活和统一。不同的分配器可以使用相同的接口进行内存管理，从而提供了更好的可移植性和可扩展性。



```c++
namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_CONTAINER

  /// See bits/stl_deque.h's _Deque_base for an explanation.
  template<typename _Tp, typename _Alloc>
    struct _Vector_base
    {
      typedef typename __gnu_cxx::__alloc_traits<_Alloc>::template
        rebind<_Tp>::other _Tp_alloc_type;//分配器类型,用于为vector容器分配内存
      typedef typename __gnu_cxx::__alloc_traits<_Tp_alloc_type>::pointer
       	pointer;

      struct _Vector_impl 
      : public _Tp_alloc_type//一个存储容器的实际数据的结构体，包括指向容器起始位置、结束位置和存储空间末尾的指针。
      {
	pointer _M_start;
	pointer _M_finish;
	pointer _M_end_of_storage;

	_Vector_impl()
	: _Tp_alloc_type(), _M_start(0), _M_finish(0), _M_end_of_storage(0)
	{ }

	_Vector_impl(_Tp_alloc_type const& __a) _GLIBCXX_NOEXCEPT
	: _Tp_alloc_type(__a), _M_start(0), _M_finish(0), _M_end_of_storage(0)
	{ }

#if __cplusplus >= 201103L
	_Vector_impl(_Tp_alloc_type&& __a) noexcept
	: _Tp_alloc_type(std::move(__a)),
	  _M_start(0), _M_finish(0), _M_end_of_storage(0)
	{ }
#endif

	void _M_swap_data(_Vector_impl& __x) _GLIBCXX_NOEXCEPT
	{
	  std::swap(_M_start, __x._M_start);
	  std::swap(_M_finish, __x._M_finish);
	  std::swap(_M_end_of_storage, __x._M_end_of_storage);
	}
      };
      
    public:
      typedef _Alloc allocator_type;
        
        //用于获取分配器对象
      _Tp_alloc_type&
      _M_get_Tp_allocator() _GLIBCXX_NOEXCEPT
      { return *static_cast<_Tp_alloc_type*>(&this->_M_impl); }

      const _Tp_alloc_type&
      _M_get_Tp_allocator() const _GLIBCXX_NOEXCEPT
      { return *static_cast<const _Tp_alloc_type*>(&this->_M_impl); }
        
        //用于获取容器的分配器
      allocator_type
      get_allocator() const _GLIBCXX_NOEXCEPT
      { return allocator_type(_M_get_Tp_allocator()); }

      _Vector_base()
      : _M_impl() { }

      _Vector_base(const allocator_type& __a) _GLIBCXX_NOEXCEPT
      : _M_impl(__a) { }

      _Vector_base(size_t __n)
      : _M_impl()
      { _M_create_storage(__n); }

      _Vector_base(size_t __n, const allocator_type& __a)
      : _M_impl(__a)
      { _M_create_storage(__n); }

#if __cplusplus >= 201103L
      _Vector_base(_Tp_alloc_type&& __a) noexcept
      : _M_impl(std::move(__a)) { }

      _Vector_base(_Vector_base&& __x) noexcept
      : _M_impl(std::move(__x._M_get_Tp_allocator()))
      { this->_M_impl._M_swap_data(__x._M_impl); }

      _Vector_base(_Vector_base&& __x, const allocator_type& __a)
      : _M_impl(__a)
      {
	if (__x.get_allocator() == __a)
	  this->_M_impl._M_swap_data(__x._M_impl);
	else
	  {
	    size_t __n = __x._M_impl._M_finish - __x._M_impl._M_start;
	    _M_create_storage(__n);
	  }
      }
#endif

      ~_Vector_base() _GLIBCXX_NOEXCEPT
      { _M_deallocate(this->_M_impl._M_start, this->_M_impl._M_end_of_storage
		      - this->_M_impl._M_start); }

    public:
      _Vector_impl _M_impl;
       //用于分配内存
      pointer
      _M_allocate(size_t __n)
      {
	typedef __gnu_cxx::__alloc_traits<_Tp_alloc_type> _Tr;
	return __n != 0 ? _Tr::allocate(_M_impl, __n) : 0;
      }
       //用于释放内存
      void
      _M_deallocate(pointer __p, size_t __n)
      {
	typedef __gnu_cxx::__alloc_traits<_Tp_alloc_type> _Tr;
	if (__p)
	  _Tr::deallocate(_M_impl, __p, __n);
      }

    private:
      //函数用于创建vector容器的存储空间。
      void
      _M_create_storage(size_t __n)
      {
	this->_M_impl._M_start = this->_M_allocate(__n);
	this->_M_impl._M_finish = this->_M_impl._M_start;
	this->_M_impl._M_end_of_storage = this->_M_impl._M_start + __n;
      }
    };
```

###  （2)）vector主类

```c++
template<typename _Tp, typename _Alloc = std::allocator<_Tp> >
    class vector : protected _Vector_base<_Tp, _Alloc>
    {
      // Concept requirements.
      typedef typename _Alloc::value_type                _Alloc_value_type;
      __glibcxx_class_requires(_Tp, _SGIAssignableConcept)
      __glibcxx_class_requires2(_Tp, _Alloc_value_type, _SameTypeConcept)
      
      typedef _Vector_base<_Tp, _Alloc>			 _Base;
      typedef typename _Base::_Tp_alloc_type		 _Tp_alloc_type;
      typedef __gnu_cxx::__alloc_traits<_Tp_alloc_type>  _Alloc_traits;

    public:
      typedef _Tp					 value_type;
      typedef typename _Base::pointer                    pointer;
      typedef typename _Alloc_traits::const_pointer      const_pointer;
      typedef typename _Alloc_traits::reference          reference;
      typedef typename _Alloc_traits::const_reference    const_reference;
       //迭代器
      typedef __gnu_cxx::__normal_iterator<pointer, vector> iterator;
      typedef __gnu_cxx::__normal_iterator<const_pointer, vector>
      const_iterator;
      typedef std::reverse_iterator<const_iterator>  const_reverse_iterator;
      typedef std::reverse_iterator<iterator>		 reverse_iterator;
      typedef size_t					 size_type;
      typedef ptrdiff_t					 difference_type;
      typedef _Alloc                        		 allocator_type;

    protected:
      using _Base::_M_allocate;
      using _Base::_M_deallocate;
      using _Base::_M_impl;
      using _Base::_M_get_Tp_allocator;
```

### （3）主要函数

构造函数

```
 //默认构造函数
 vector()
 
 //显式构造函数，创建一个空的vector对象，并使用指定的分配器__a。
 vector(const allocator_type& __a) 
 
 //带有初始元素个数的构造函数，创建一个具有__n个默认构造的元素的vector对象。
  vector(size_type __n, const allocator_type& __a = allocator_type())
  
//带有初始元素个数和初始值的构造函数
   vector(size_type __n, const value_type& __value = value_type(),const allocator_type& __a = allocator_type())
   
//拷贝构造函数，创建一个新的vector对象，使用与__x相同的元素类型和分配器。
 vector(const vector& __x)
 

 ..........
```



`assign`函数

```
 assign(size_type __n, const value_type& __val)
      { _M_fill_assign(__n, __val); }

```

`assign(size_type __n, const value_type& __val)`：将`__n`个值为`__val`的元素赋值给`vector`对象。

```
assign(_InputIterator __first, _InputIterator __last)
        {
	  // Check whether it's an integral type.  If so, it's not an iterator.
	  typedef typename std::__is_integer<_InputIterator>::__type _Integral;
	  _M_assign_dispatch(__first, __last, _Integral());
	}
```

将范围为`[__first, __last)`的元素赋值给`vector`对象。



`begin()`函数

```
      iterator
      begin() _GLIBCXX_NOEXCEPT
      { return iterator(this->_M_impl._M_start); }
```

返回一个迭代器，指向`vector`容器中的第一个元素。



`end()`函数

```
     iterator
      end() _GLIBCXX_NOEXCEPT
      { return iterator(this->_M_impl._M_finish); }
```

返回一个迭代器，指向容器末尾的下一个位置



`rbegin()`函数

```
      reverse_iterator
      rbegin() _GLIBCXX_NOEXCEPT
      { return reverse_iterator(end()); }
```

返回一个逆序迭代器，指向容器末尾。



`rend()`函数

```
     reverse_iterator
     rend() _GLIBCXX_NOEXCEPT
      { return reverse_iterator(begin()); }
```

返回一个逆序迭代器，指向容器起始位置前一个位置



`cbegin()`函数

```
      const_iterator
      cbegin() const noexcept
      { return const_iterator(this->_M_impl._M_start); }
```

返回一个常量迭代器，指向容器起始位置



`cend()`函数

```
      const_iterator
      cend() const noexcept
      { return const_iterator(this->_M_impl._M_finish); }
```

返回一个常量迭代器，指向容器末尾位置的下一个位置



`size()`函数

```
      size_type
      size() const _GLIBCXX_NOEXCEPT
      { return size_type(this->_M_impl._M_finish - this->_M_impl._M_start); }
```

通过计算容器中结束迭代器与起始迭代器之间的距离来确定容器中的元素数量



`max_size()`函数

```
      size_type
      max_size() const _GLIBCXX_NOEXCEPT
      { return _Alloc_traits::max_size(_M_get_Tp_allocator()); }
```

通过调用底层分配器的`max_size()`函数来确定容器可容纳的最大元素数量。底层分配器是通过`_M_get_Tp_allocator()`函数获取的



`resize()`函数

```
     resize(size_type __new_size, value_type __x = value_type())
      {
	if (__new_size > size())
	  insert(end(), __new_size - size(), __x);
	else if (__new_size < size())
	  _M_erase_at_end(this->_M_impl._M_start + __new_size);
      }
```

调整容器的大小为指定的新大小`__new_size`。

- 如果`__new_size`大于当前容器的大小，那么在容器末尾插入`__new_size - size()`个值为`__x`的元素。

- 如果`__new_size`小于当前容器的大小，那么删除容器尾部的`size() - __new_size`个元素。

  

`capacity()`函数

```
      size_type
      capacity() const _GLIBCXX_NOEXCEPT
      { return size_type(this->_M_impl._M_end_of_storage
			 - this->_M_impl._M_start); }
```

返回当前容器的容量，即可存储元素的内存空间大小。



`empty()`函数

```
     bool
     empty() const _GLIBCXX_NOEXCEPT
      { return begin() == end(); }
```

返回一个`bool`值，表示容器是否为空。



`operator[]`函数

```
      reference
      operator[](size_type __n) _GLIBCXX_NOEXCEPT
      { return *(this->_M_impl._M_start + __n); }
```

用于通过下标访问容器中的元素。

`operator[]`返回指定下标位置的元素的引用。

- 参数：`__n`，表示要访问的元素的下标。
- 返回值：返回对应下标位置的元素的引用。
- 注意：`operator[]`不会执行越界检查，因此使用时需要确保访问的下标在有效范围内。



`push_back`函数

```
      void
      push_back(const value_type& __x)
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

用于在容器的末尾添加一个元素。

`push_back`：将元素添加到容器的末尾。

- 参数：`__x`，表示要添加的元素的值。
- 返回值：无。
- 注意：如果容器的内部存储空间足够容纳新元素，则直接在末尾构造新元素并将容器的大小增加1。如果容器的内部存储空间已满，则会自动扩展容器的容量，并在扩展后的空间末尾构造新元素。



`pop_back`函数

```
  void
      pop_back() _GLIBCXX_NOEXCEPT
      {
	--this->_M_impl._M_finish;
	_Alloc_traits::destroy(this->_M_impl, this->_M_impl._M_finish);
      }
```

用于删除容器的末尾元素。



`insert`函数

```
     iterator
      insert(const_iterator __position, const value_type& __x);
```

`insert`：在指定位置插入一个元素。

- 参数：
  - `__position`：插入位置的迭代器。
  - `__x`：要插入的元素的值。
- 返回值：一个迭代器，指向插入的元素。
- 注意：`insert`函数将在指定位置之前插入新元素，并使得容器的大小增加1。

```
      void
      insert(iterator __position, size_type __n, const value_type& __x)
      { _M_fill_insert(__position, __n, __x); }
```

`insert`：在指定位置插入一定数量的相同元素。

- 参数：
  - `__position`：插入位置的迭代器。
  - `__n`：要插入的元素的数量。
  - `__x`：要插入的元素的值。
- 注意：`insert`函数将在指定位置之前插入`__n`个值为`__x`的元素，并使得容器的大小增加`__n`。



`erase`函数

```
      erase(iterator __first, iterator __last)
      { return _M_erase(__first, __last); }
```

`erase`：删除指定范围的元素。

- 参数：
  - `__first`：要删除的元素范围的起始位置的迭代器。
  - `__last`：要删除的元素范围的终止位置的迭代器。
- 返回值：指向删除操作后紧接着被删除元素之后的元素的迭代器。
- 注意：`erase`函数将删除从`__first`到`__last`之间的所有元素，并更新容器的大小。