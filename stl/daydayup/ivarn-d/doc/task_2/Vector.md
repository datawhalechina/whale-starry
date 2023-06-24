## 1.介绍
vector 基本能够支持任何类型的对象，同时也是一个可以动态增长的数组，使用起来非常的方便。

## 2.解析

### 2.1 基本数据结构

>基本上，STL 里面所有的容器的源码都包含至少三个部分：
>+ 迭代器，遍历容器的元素，控制容器空间的边界和元素的移动；
>+ 构造函数，满足容器的多种初始化；
>+ 属性的获取，比如 begin()，end()等；

vector是一个简单的线性连续空间。

它以两个迭代器 start 和 finish 分别表示vector的起始元素的地址和终止元素的地址。

并且还具有一个 end_of_storage 表示vector开辟的空间的终止位置。

```
struct _Vector_impl
      : public _Tp_alloc_type
      {
      pointer _M_start;
      pointer _M_finish;
      pointer _M_end_of_storage;
```


### 2.4 对元素的操作
#### 2.4.1 pop_back
弹出最后一个元素，代码实现如下：
```
      void
      pop_back() _GLIBCXX_NOEXCEPT
      {
      --this->_M_impl._M_finish;
      _Alloc_traits::destroy(this->_M_impl, this->_M_impl._M_finish);
```
把指向元素末尾的 finish 直接减一即可,然后调用全局的destroy销毁销毁最后一个元素的空间
#### 2.4.2 erase
代码实现如下：
```
iterator
erase(const_iterator __first, const_iterator __last)
      {
      const auto __beg = begin();
      const auto __cbeg = cbegin();
      return _M_erase(__beg + (__first - __cbeg), __beg + (__last - __cbeg));
      }
```
#### 2.4.3 insert
```
template<typename _InputIterator,
             typename = std::_RequireInputIter<_InputIterator>>
        iterator
        insert(const_iterator __position, _InputIterator __first,
             _InputIterator __last)
        {
        difference_type __offset = __position - cbegin();
        _M_insert_dispatch(begin() + __offset,

                       __first, __last, __false_type());
        return begin() + __offset;
      }
```

#### 2.4.4 push_back
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
```


## 3.实现
[ivarnd_vector.h](src/task_2/ivarnd_vector.h)

[main.cpp](src/task_2/main.cpp)

## 参考

[(25条消息) 【C++】STL——vector 深度剖析 及 模拟实现_c++ vector 实现原理_YIN_尹的博客-CSDN博客](https://blog.csdn.net/m0_70980326/article/details/130139888)

[超硬核 | 2 万字+20 图带你手撕 STL 序列式容器源码 - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/359213877)

[STL源码学习（3）- vector详解 - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/613974193)