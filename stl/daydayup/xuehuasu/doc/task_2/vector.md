# Vector

感觉`vector`没有`list`那么复杂

## 数据结构：

和`list`的基类管理指针，子类管理数据的方式不同，`vector`的数据是在连续的空间上，`vector_base` 存储的是指针`point`，通过指针访问数据的内容（数据就是指针类型）。

```c++
template <typename _Tp, typename _Alloc>
struct _Vector_base {
  typedef typename __gnu_cxx::__alloc_traits<_Alloc>::template rebind<_Tp>::other
      _Tp_alloc_type;
  typedef typename __gnu_cxx::__alloc_traits<_Tp_alloc_type>::pointer pointer;

  struct _Vector_impl_data {
    pointer _M_start;           // 头指针
    pointer _M_finish;          // 尾指针
    pointer _M_end_of_storage;  // 已分配空间的最后一个指针

    _Vector_impl_data() _GLIBCXX_NOEXCEPT : _M_start(),
                                            _M_finish(),
                                            _M_end_of_storage() {}
    _Vector_impl_data(_Vector_impl_data&& __x) noexcept
        : _M_start(__x._M_start),
          _M_finish(__x._M_finish),
          _M_end_of_storage(__x._M_end_of_storage) {
      __x._M_start = __x._M_finish = __x._M_end_of_storage = pointer();
    }
  }

  struct _Vector_impl : public _Tp_alloc_type,
                        public _Vector_impl_data {
    /*...用来管理_Vector_impl_data的增删改操作，总之就是内存管理....*/
  }
}

```

## 迭代器

和`list`的双向迭代器不同， `vector` 用的是随机访问迭代器（正向），还有一个反向迭代器，就不看了

```c
typedef __gnu_cxx::__normal_iterator<pointer, vector> iterator;
```

```c
template<typename _Iterator, typename _Container>
class __normal_iterator
{
protected:
    _Iterator _M_current;//当前指针

    typedef iterator_traits<_Iterator>		__traits_type;//看不懂，标记一下

public:
    typedef _Iterator					iterator_type;//迭代器类型

    // iterator必须包含的五种typedef
    typedef typename __traits_type::iterator_category iterator_category;
    typedef typename __traits_type::value_type  	value_type;
    typedef typename __traits_type::difference_type 	difference_type;
    typedef typename __traits_type::reference 	reference;
    typedef typename __traits_type::pointer   	pointer;

    _GLIBCXX_CONSTEXPR __normal_iterator() _GLIBCXX_NOEXCEPT
    : _M_current(_Iterator()) { }

    explicit
    __normal_iterator(const _Iterator& __i) _GLIBCXX_NOEXCEPT
    : _M_current(__i) { }

    // Allow iterator to const_iterator conversion
    template<typename _Iter>
    __normal_iterator(const __normal_iterator<_Iter,
            typename __enable_if<
            (std::__are_same<_Iter, typename _Container::pointer>::__value),
            _Container>::__type>& __i) _GLIBCXX_NOEXCEPT
    : _M_current(__i.base()) { }

    // Forward iterator requirements
    reference
    operator*() const _GLIBCXX_NOEXCEPT
    { return *_M_current; }

    pointer
    operator->() const _GLIBCXX_NOEXCEPT
    { return _M_current; }

    // 前置++
    __normal_iterator&
    operator++() _GLIBCXX_NOEXCEPT
    {
        ++_M_current;
        return *this;
    }

    // 后置++
    __normal_iterator
    operator++(int) _GLIBCXX_NOEXCEPT
    { return __normal_iterator(_M_current++); }

    // 前置--
    // Bidirectional iterator requirements
    __normal_iterator&
    operator--() _GLIBCXX_NOEXCEPT
    {
        --_M_current;
        return *this;
    }

    // 后置--
    __normal_iterator
    operator--(int) _GLIBCXX_NOEXCEPT
    { return __normal_iterator(_M_current--); }

    // 随机访问迭代器都要重载[]操作符
    // Random access iterator requirements
    reference
    operator[](difference_type __n) const _GLIBCXX_NOEXCEPT
    { return _M_current[__n]; }

    // +=操作符 跳跃n个difference_type
    __normal_iterator&
    operator+=(difference_type __n) _GLIBCXX_NOEXCEPT
    { _M_current += __n; return *this; }

    // +操作符 跳跃n个difference_type
    __normal_iterator
    operator+(difference_type __n) const _GLIBCXX_NOEXCEPT
    { return __normal_iterator(_M_current + __n); }

    // -=操作符 后退n个difference_type
    __normal_iterator&
    operator-=(difference_type __n) _GLIBCXX_NOEXCEPT
    { _M_current -= __n; return *this; }

    // -操作符 后退n个difference_type
    __normal_iterator
    operator-(difference_type __n) const _GLIBCXX_NOEXCEPT
    { return __normal_iterator(_M_current - __n); }

    const _Iterator&
    base() const _GLIBCXX_NOEXCEPT
    { return _M_current; }
};
```



## vector

```c
template <typename _Tp, typename _Alloc = std::allocator<_Tp>>
    class vector : protected _Vector_base<_Tp, _Alloc>
{    
    typedef _Vector_base<_Tp, _Alloc> _Base;
    typedef typename _Base::_Tp_alloc_type _Tp_alloc_type;
    typedef __gnu_cxx::__alloc_traits<_Tp_alloc_type> _Alloc_traits;

  public:
    typedef _Tp value_type;
    typedef typename _Base::pointer pointer;
    typedef typename _Alloc_traits::const_pointer const_pointer;
    typedef typename _Alloc_traits::reference reference;
    typedef typename _Alloc_traits::const_reference const_reference;
    typedef __gnu_cxx::__normal_iterator<pointer, vector> iterator;
    typedef __gnu_cxx::__normal_iterator<const_pointer, vector>
        const_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef _Alloc allocator_type;
};
```

### 构造与析构函数

```c++
vector(const vector &__x)
    : _Base(__x.size(),
            _Alloc_traits::_S_select_on_copy(__x._M_get_Tp_allocator()))
    {
        this->_M_impl._M_finish =
            std::__uninitialized_copy_a(__x.begin(), __x.end(),
                                        this->_M_impl._M_start,
                                        _M_get_Tp_allocator());
    }
```

```c++
~vector() _GLIBCXX_NOEXCEPT
{
    std::_Destroy(this->_M_impl._M_start, this->_M_impl._M_finish,
                  _M_get_Tp_allocator());
}
```

至于`__uninitialized_copy_a`和`_Destroy`具体实现是什么，先不管，从结果上看可以知道它们的做用是拷贝和释放内存。

### 删除

`pop_back`

```c++
void
    pop_back() _GLIBCXX_NOEXCEPT
{
    __glibcxx_requires_nonempty();
    --this->_M_impl._M_finish;
    _Alloc_traits::destroy(this->_M_impl, this->_M_impl._M_finish);
    _GLIBCXX_ASAN_ANNOTATE_SHRINK(1);
}
```

`erase` 直接调用了`_M_erase` 

单个删除

```c++
template<typename _Alloc>
typename vector<bool, _Alloc>::iterator
    vector<bool, _Alloc>::
_M_erase(iterator __position)
{
    if (__position + 1 != end())
        std::copy(__position + 1, end(), __position);//把 [pos+1, end) 移动到 pos上
    --this->_M_impl._M_finish;//尾指针减一，注意这里不需要释放内存
    return __position;
}
```

删除连续的

```c++
template<typename _Alloc>
typename vector<bool, _Alloc>::iterator
    vector<bool, _Alloc>::
_M_erase(iterator __first, iterator __last)
{
    if (__first != __last)
        _M_erase_at_end(std::copy(__last, end(), __first));
    /*
    	假设[__last, finish) 替换 [__first, __last)的位置，copy返回的正是删除后finish应该所在位置pos
    	_M_erase_at_end的工作是清空[pos finish)，然后用finish=pos
    */
    return __first;
}

void
    _M_erase_at_end(pointer __pos) _GLIBCXX_NOEXCEPT
{
    if (size_type __n = this->_M_impl._M_finish - __pos)
    {
        std::_Destroy(__pos, this->_M_impl._M_finish,
                      _M_get_Tp_allocator());
        this->_M_impl._M_finish = __pos;//更新finish
        _GLIBCXX_ASAN_ANNOTATE_SHRINK(__n);
    }
}
```

### 增加

增加元素是重头戏，这里涉及到`vector`的动态内存管理。

`push_back`

```c++
void push_back(const value_type &__x) 
{
    if (this->_M_impl._M_finish != this->_M_impl._M_end_of_storage) //内存足够
    {
        _Alloc_traits::construct(this->_M_impl, this->_M_impl._M_finish, __x);
        ++this->_M_impl._M_finish;
    }
    else
        _M_realloc_insert(end(), __x);//内存不够，这个函数内部实现在下面提到
}
```

`insert` 

```c++
template <typename _Tp, typename _Alloc>
typename vector<_Tp, _Alloc>::iterator
    vector<_Tp, _Alloc>::

insert(iterator __position, const value_type &__x)
{
    const size_type __n = __position - begin();
    if (this->_M_impl._M_finish != this->_M_impl._M_end_of_storage)//内存够
    {
        if (__position == end())//直接插在末尾
        {
            _Alloc_traits::construct(this->_M_impl, this->_M_impl._M_finish, __x);
            ++this->_M_impl._M_finish;
        }
        else
        {
            _M_insert_aux(__position, __x);//不插在末尾
        }
    }
    else
        _M_realloc_insert(__position, __x);//内存不够。

    return iterator(this->_M_impl._M_start + __n);
}
```

`_M_insert_aux`

```c++
template <typename _Tp, typename _Alloc>
void vector<_Tp, _Alloc>::
_M_insert_aux(iterator __position, const _Tp &__x)
{
    //在finish处加一个元素
    _Alloc_traits::construct(this->_M_impl, this->_M_impl._M_finish,
                             _GLIBCXX_MOVE(*(this->_M_impl._M_finish - 1)));
    ++this->_M_impl._M_finish;
	//[pos, finish-1) 后移一位
    _GLIBCXX_MOVE_BACKWARD3(__position.base(),
                            this->_M_impl._M_finish - 2,
                            this->_M_impl._M_finish - 1);
    _Tp __x_copy = __x;
    *__position = __x_copy;
}
```

`_M_realloc_insert`

```c++
template <typename _Tp, typename _Alloc>
void vector<_Tp, _Alloc>::
    _M_realloc_insert(iterator __position, const _Tp &__x)

{
  //重新分配内存，原理见下方
  const size_type __len =
      _M_check_len(size_type(1), "vector::_M_realloc_insert");
        
  pointer __old_start = this->_M_impl._M_start;
  pointer __old_finish = this->_M_impl._M_finish;
  const size_type __elems_before = __position - begin();
        
  pointer __new_start(this->_M_allocate(__len));//创建新的内存区域
  pointer __new_finish(__new_start);
  __try
  {
      //在pos = __new_start + __elems_before上创建x
    _Alloc_traits::construct(this->_M_impl,
                             __new_start + __elems_before, __x);
      //将 __new_finish 指针设置为空指针,避免在后面的代码中使用未初始化的指针。
    __new_finish = pointer(); 
      
      //将 [__old_start, __position.base) 之间的元素移动到新的内存区域[__new_start,...)中，并返回一个指向新内存中最后一个元素的下一个位置的指针
    __new_finish = std::__uninitialized_move_if_noexcept_a(__old_start, __position.base(), __new_start, _M_get_Tp_allocator());

    ++__new_finish;
	//将 [__position.base, __old_finish) 之间的元素移动到新的内存区域[__new_finish,..)中，并返回一个指向新内存中最后一个元素的下一个位置的指针
    __new_finish = std::__uninitialized_move_if_noexcept_a(__position.base(), __old_finish, __new_finish, _M_get_Tp_allocator());
      
  }
  __catch(...)//出错了，释放创建的新内存
  {
    if (!__new_finish)
      _Alloc_traits::destroy(this->_M_impl,
                             __new_start + __elems_before);
    else
      std::_Destroy(__new_start, __new_finish, _M_get_Tp_allocator());
    _M_deallocate(__new_start, __len);
    __throw_exception_again;
  }
	//释放原来的vector	
  std::_Destroy(__old_start, __old_finish, _M_get_Tp_allocator());
  _GLIBCXX_ASAN_ANNOTATE_REINIT;
  _M_deallocate(__old_start,
                this->_M_impl._M_end_of_storage - __old_start);
        
  this->_M_impl._M_start = __new_start;
  this->_M_impl._M_finish = __new_finish;
  this->_M_impl._M_end_of_storage = __new_start + __len;
}
```

`const size_type __len = _M_check_len(size_type(1), "vector::_M_realloc_insert");` 

这行代码是在 `vector` 类的 `_M_realloc_insert()` 函数中，用于计算重新分配内存后的容器大小。

该行代码调用了 `_M_check_len()` 函数，该函数用于检查重新分配内存后容器的大小是否超出了容器的最大尺寸。如果超出了最大尺寸，会抛出 `length_error` 异常。

`_M_check_len()` 函数的第一个参数是重新分配内存后的容器大小。第二个参数是一个字符串，用于表示检查的上下文信息。

在这里，`size_type(1)` 表示要插入的元素个数，即重新分配内存后容器的大小为 1。如果容器的最大尺寸为 0，则会抛出 `length_error` 异常。否则，如果容器的最大尺寸不为 0，且重新分配内存后容器的大小超出了最大尺寸，则也会抛出 `length_error` 异常。

需要注意的是，`size_type` 类型是 `vector` 类的一个公共类型，表示容器大小的无符号整数类型。在 `vector` 类的实现中，通常使用 `size_type` 来表示容器的大小。

`_M_check_len`

```c++
size_type
_M_check_len(size_type __n, const char* __s) const
{
    if (max_size() - size() < __n) //判断当前剩余空间是否能添加n个
        __throw_length_error(__N(__s));

    const size_type __len = size() + std::max(size(), __n); //如果n小于当前size，内存加倍，否则内存增长n。
    return (__len < size() || __len > max_size()) ? max_size() : __len;
}
```



如果插入的是一串，`insert`的一个重载版本会调用下面这个函数：

```c++
template <typename _Tp, typename _Alloc>
template <typename _ForwardIterator>
void
vector<_Tp, _Alloc>::
_M_range_insert(iterator __position, _ForwardIterator __first,
                _ForwardIterator __last, std::forward_iterator_tag)
{
    if (__first != __last) //先判断插入的是否为空
    {
        const size_type __n = std::distance(__first, __last); //计算要插入的串的大小
        if (size_type(this->_M_impl._M_end_of_storage - this->_M_impl._M_finish) >= __n)//当前空间足够
        {
            const size_type __elems_after = end() - __position;
            pointer __old_finish(this->_M_impl._M_finish);
            //情况一，[pos,end) 的个数大于插入的数量
            if (__elems_after > __n)
            {
                //[finish-n, finish) --> [finish, ...)
                std::__uninitialized_move_a(this->_M_impl._M_finish - __n,
                                            this->_M_impl._M_finish,
                                            this->_M_impl._M_finish,
                                            _M_get_Tp_allocator());
                this->_M_impl._M_finish += __n;
                //这里我不理解为什么还要移动一次
                _GLIBCXX_MOVE_BACKWARD3(__position.base(),  __old_finish - __n, __old_finish);
                
                std::copy(__first, __last, __position);
            }
            //情况二，[pos,end) 的个数小于等于插入的数量
            else
            {
                _ForwardIterator __mid = __first;
                std::advance(__mid, __elems_after);// 将mid移动__elems_after到finish的位置
                
                //[mid, last) --> [finish,...)
                std::__uninitialized_copy_a(__mid, __last,
                                            this->_M_impl._M_finish,
                                            _M_get_Tp_allocator());
                this->_M_impl._M_finish += __n - __elems_after;
                
                //[pos, old_finish) --> [new_finish,...)
                std::__uninitialized_move_a(__position.base(),
                                            __old_finish,
                                            this->_M_impl._M_finish,
                                            _M_get_Tp_allocator());
                this->_M_impl._M_finish += __elems_after;
                
                std::copy(__first, __mid, __position);
            }
        }
        //情况三，空间不够，这里和_M_realloc_insert很像
        else
        {
            const size_type __len =
                _M_check_len(__n, "vector::_M_range_insert");
            pointer __new_start(this->_M_allocate(__len));
            pointer __new_finish(__new_start);
            
            __try
            {
                // [start, pos) --> [new_start, ...)
                __new_finish = std::__uninitialized_move_if_noexcept_a(this->_M_impl._M_start, __position.base(), __new_start, _M_get_Tp_allocator());
                
                // [first, last) --> [new_finish, ...)
                __new_finish = std::__uninitialized_copy_a(__first, __last, __new_finish, _M_get_Tp_allocator());
                
                // [pos, finish) --> [new_finish)
                __new_finish = std::__uninitialized_move_if_noexcept_a(__position.base(), this->_M_impl._M_finish,  __new_finish, _M_get_Tp_allocator());
            }
            __catch(...)
            {
                std::_Destroy(__new_start, __new_finish,
                              _M_get_Tp_allocator());
                _M_deallocate(__new_start, __len);
                __throw_exception_again;
            }
            std::_Destroy(this->_M_impl._M_start, this->_M_impl._M_finish,
                          _M_get_Tp_allocator());
            _GLIBCXX_ASAN_ANNOTATE_REINIT;
            _M_deallocate(this->_M_impl._M_start,
                          this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
            this->_M_impl._M_start = __new_start;
            this->_M_impl._M_finish = __new_finish;
            this->_M_impl._M_end_of_storage = __new_start + __len;
        }
    }
}
```



## 应用

我们来验证下**动态扩容机制**：

```c++
int main() {
  std::vector<int> vec;

  // 输出vector的初始容量和大小
  std::cout << "Initial capacity: " << vec.capacity() << std::endl;
  std::cout << "Initial size: " << vec.size() << std::endl;

  // 添加1000个整数到vector中，并在容量发生变化时输出相关信息
  int mlen = 0;
  for (int i = 0; i < 1000; i++) {
    vec.push_back(i);
    if (vec.capacity() != mlen) {
      std::cout << "size: " << vec.size() << " capacity: " << vec.capacity() << std::endl;
      mlen = vec.capacity();
    }
  }

  // 手动控制内存分配和释放
  vec.reserve(2000);  // 预分配2000个元素的空间
  std::cout << "Capacity after reserving 2000 elements: " << vec.capacity() << std::endl;

  vec.shrink_to_fit();  // 释放多余的内存
  std::cout << "Capacity after shrinking to fit: " << vec.capacity() << std::endl;

  return 0;
}
```

<img src="./image-1.png" alt="image-20230624130838244" style="zoom:80%;" />

可以看到在内存足够的情况下，预分配空间大小确实是倍增的，通过源码分析我们知道，如果一次性添加的元素大于剩余空间并且比原`vector`还大，会直接分配`n`的空间，刚好够插入多个元素。

**效率对比**

`list`使用的是双向迭代器，而`vector`使用的是随机访问迭代器，很明显在增删上`list`会比`vector`快，因为`vector`几乎每次都要进行多个元素的拷贝，同样的道理`list`在改查方面一定比`vector`慢，毕竟`vector`在空间上连续。写个程序探讨下效率究竟差多少。

```c++
// 定义一个计时器类
class Timer {
 public:
  Timer() : m_start(std::chrono::high_resolution_clock::now()) {}
  void reset() { m_start = std::chrono::high_resolution_clock::now(); }
  double elapsed() const {
    return std::chrono::duration_cast<std::chrono::duration<double>>(
               std::chrono::high_resolution_clock::now() - m_start)
        .count();
  }

 private:
  std::chrono::time_point<std::chrono::high_resolution_clock> m_start;
};

void test_vector(const int N, const int M) {
  std::vector<int> v;  // 定义一个vector

  // 生成随机数
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(1, 100);

  // 插入元素
  double insert_time = 0;
  {
    Timer t;
    for (int i = 0; i < N; ++i) {
      v.push_back(dis(gen));
    }
    insert_time += t.elapsed();
  }
  std::cout << "time to insert " << N
            << " elements into vector: " << insert_time << " s\n";

  // 随机访问
  double access_time = 0;
  for (int i = 0; i < M; ++i) {
    long long sum = 0;
    Timer t;
    for (int i = 0; i < N; ++i) {
      sum += v[i];
    }
    access_time += t.elapsed();
  }
  std::cout << "time to access " << N << " elements in vector: " << access_time
            << " s\n";

  // 删除元素
  double erase_middle_time = 0;
  for (int i = 0; i < 10000; ++i) {
    auto it = v.begin();
    std::advance(it, N / 2);//查找"中间"那个，虽然不是真的中间，但不影响测试结果
    Timer t;
    v.erase(it);
    erase_middle_time += t.elapsed();
  }
  std::cout << "time to erase an element from the middle of vector: "
            << erase_middle_time << " s\n";

  //排序
  {
    Timer t;
    std::sort(v.begin(), v.end());
    double sort_time = t.elapsed();
    std::cout << "time to sort of vector: " << sort_time << " s\n";
  }
}

void test_list(const int N, const int M) {
  std::list<int> l;  // 定义一个list

  // 生成随机数
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(1, 100);

  // 插入元素
  double insert_time = 0;
  {
    Timer t;
    for (int i = 0; i < N; ++i) {
      l.push_back(dis(gen));
    }
    insert_time += t.elapsed();
  }
  std::cout << "time to insert " << N << " elements into list: " << insert_time
            << " s\n";

  // 随机访问
  double access_time = 0;
  for (int i = 0; i < M; ++i) {
    long long sum = 0;
    Timer t;
    for (auto it = l.begin(); it != l.end(); ++it) {
      sum += *it;
    }
    access_time += t.elapsed();
  }
  std::cout << "time to access " << N << " elements in list: " << access_time
            << " s\n";

  // 删除元素
  double erase_middle_time = 0;
  for (int i = 0; i < 10000; ++i) {
    auto it = l.begin();
    std::advance(it, N / 2);
    Timer t;
    l.erase(it);
    erase_middle_time += t.elapsed();
  }
  std::cout << "time to erase an element from the middle of list: "
            << erase_middle_time << " s\n";

  //排序
  {
    Timer t;
    l.sort();
    double sort_time = t.elapsed();
    std::cout << "time to sort of list: " << sort_time << " s\n";
  }
}
void Compare() {
  const int N = 1000000;  // 数据量大小
  const int M = 10;       // 每个操作执行的次数
  std::cout << "Testing list:\n";
  test_list(N, M);

  std::cout << "\nTesting vector:\n";
  test_vector(N, M);
}
```

![image-20230624153727093](./image-2.png)

经过多次测试，`list`在增删上的时间很优秀，尤其是删除（其实我觉得增上面，`list`理应也甩开`vector`一段距离，可能是创建节点这块消耗时间比较多），而`list`查找方面就逊色了，如果删除的时候把定位中间元素的时间也算上的话，会发现`list`比`vector`慢10倍

![image-20230624154401699](./image-3.png)

如果增删操作的频率**远高于**查找的话，我们使用`list`，否则的话使用`vector`。而且通过源码分析我们得出，`vector`占用的空间会比`list`少，`list`每一个数据至少都要前后两个指针维护，而`vector`不需要这么多冗余的数据。

综上，除非对`list`有特殊的需求，否则的话使用`vector`是更好的选择。
