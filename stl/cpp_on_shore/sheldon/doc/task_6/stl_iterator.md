# stl_iterator源码阅读

## iterator概述

STL中的算法和容器是分开实现的，iterator是将两者结合在一起的方式。iterator就是一种可以顺序访问容器中每个元素的方法。

iterator也是一种智能指针，可以进行 `*` 和 `->` 操作，`==` `!=` `++` `--`操作。



## traits

萃取，就是提取迭代器中的对应的型别，其实本次阅读的源码主要是关于萃取的。代码中主要萃取的步骤是

```cpp
  // class type
template<typename _Iterator>
    struct iterator_traits
    {
      typedef typename _Iterator::iterator_category iterator_category;
      typedef typename _Iterator::value_type        value_type;
      typedef typename _Iterator::difference_type   difference_type;
      typedef typename _Iterator::pointer           pointer;
      typedef typename _Iterator::reference         reference;
    };

  /// 偏特化 针对指针类型 point type
  template<typename _Tp>
    struct iterator_traits<_Tp*>
    {
      typedef random_access_iterator_tag iterator_category;
      typedef _Tp                         value_type;
      typedef ptrdiff_t                   difference_type;
      typedef _Tp*                        pointer;
      typedef _Tp&                        reference;
    };

  /// 偏特化 针对常指针类型 const pointer type
  template<typename _Tp>
    struct iterator_traits<const _Tp*>
    {
      typedef random_access_iterator_tag iterator_category;
      typedef _Tp                         value_type;
      typedef ptrdiff_t                   difference_type;
      typedef const _Tp*                  pointer;
      typedef const _Tp&                  reference;
    };
```

将上面的代码对比[参考链接](https://github.com/Light-City/CPlusPlusThings/blob/master/src_analysis/stl/traits.md)则可以理解，如果传入iterator_traits的是一个类，则会回答类的内嵌型别（形如 _Iterator::iterator_category），那为了支持原生指针，则对其进行了偏特化，这里注意默认返回了random_access_iterator_tag作为iterator_category。

## 迭代器的分类

```cpp
  ///  Marking input iterators.
  struct input_iterator_tag { };

  ///  Marking output iterators.
  struct output_iterator_tag { };

  /// Forward iterators support a superset of input iterator operations.
  struct forward_iterator_tag : public input_iterator_tag { };

  /// Bidirectional iterators support a superset of forward iterator
  /// operations.
  struct bidirectional_iterator_tag : public forward_iterator_tag { };

  /// Random-access iterators support a superset of bidirectional
  /// iterator operations.
  struct random_access_iterator_tag : public bidirectional_iterator_tag { };
```

### 输入迭代器（Input Iterator）

输入迭代器用于读取容器中的元素，但不能修改元素的值。其特点包括：

- 只能单向移动，不支持前进和后退操作
- 支持解引用操作（*）
- 可以进行逐个元素的读取

### 输出迭代器（Output Iterator）

输出迭代器用于写入容器中的元素，但不能读取元素的值。其特点包括：

- 只能单向移动，不支持前进和后退操作
- 支持解引用操作（*）
- 可以进行逐个元素的写入

### 前向迭代器（Forward Iterator）

前向迭代器支持输入和输出操作，并且可以向前移动迭代器。其特点包括：

- 可以单向移动，支持前进操作
- 支持解引用操作（*）
- 可以进行逐个元素的读写

### 双向迭代器（Bidirectional Iterator）

双向迭代器支持输入和输出操作，并且可以向前和向后移动迭代器。其特点包括：

- 可以双向移动，支持前进和后退操作
- 支持解引用操作（*）
- 可以进行逐个元素的读写

### 随机访问迭代器（Random Access Iterator）

随机访问迭代器具有最强的功能，支持输入和输出操作，并且可以在容器中进行随机访问。其特点包括：

- 可以双向移动，支持前进和后退操作
- 支持解引用操作（*）
- 支持元素的随机访问（使用类似指针的语法，如iter + n）
- 可以进行逐个元素的读写