# This is to learn the iterator method and design in stl containers.
1. 首先, iterator是一种抽象的设计概念. 它也是23种常用软件**设计模式**(*design patterns*)中的一个. 其中, 迭代器(*iterator*)设计模式指的是: 提供一种方法, 使其能够按照顺序遍历某个容器中所包含的全部元素, 但又不需要暴露该容器内部的组成.
2. STL容器在设计时, 使用了**高内聚低耦合**的设计思路, 即将数据容器和算法分开, 彼此独立设计没有耦合部分, 最后使用iterator方法建立二者之间的联系, 这也是体现了iterator设计方法之处
实例代码如下:
```
// 我们以find函数为例
template<class InputIterator, class T>
InputIterator find(InputIterator first, InputIterator last, const T& value) {
    while (first != last && *first != value)
        first++;
    return first;
}
```
3. iterator的定义 iterator的定义较为简单, 但是必须包含以下5项: ***iterator_category***, ***value_type***, ***difference_type***, ***pointer***, ***reference***
```
// to define the iterator class
template<typename _Category, typename _Tp, typename _Distance = ptrdiff_t, typename _Pointer = _Tp*, typename _Reference = _Tp&>
    struct iterator
    {
      /// One of the @link iterator_tags tag types@endlink.
      typedef _Category  iterator_category;
      /// The type "pointed to" by the iterator.
      typedef _Tp        value_type;
      /// Distance between iterators is represented as this type.
      typedef _Distance  difference_type;
      /// This type represents a pointer-to-value_type.
      typedef _Pointer   pointer;
      /// This type represents a reference-to-value_type.
      typedef _Reference reference;
    };
```
除了iterator本身之外, 还需要定义iterator_traits, 至于为什么定义traits这个类, 在后面会提到
```
// to define the traits class
template<typename _Iterator, typename = __void_t<>>
    struct __iterator_traits { };

template<typename _Iterator>
    struct __iterator_traits<_Iterator,
			     __void_t<typename _Iterator::iterator_category,
				      typename _Iterator::value_type,
				      typename _Iterator::difference_type,
				      typename _Iterator::pointer,
				      typename _Iterator::reference>>
    {
      typedef typename _Iterator::iterator_category iterator_category;
      typedef typename _Iterator::value_type        value_type;
      typedef typename _Iterator::difference_type   difference_type;
      typedef typename _Iterator::pointer           pointer;
      typedef typename _Iterator::reference         reference;
    };

template<typename _Iterator>
    struct iterator_traits
    : public __iterator_traits<_Iterator> { };

```
4. iterator中相对应的几个类别. 
   
   - value type: 很简单, 就是指代的iterator所指向对象的类别.  
   - difference type: 用来表示两个迭代器之间的距离, 因此也可以表示一个容器的最大容量(对于连续空间的容器, 尾指针-头指针的距离就是最大容量)
   e.g. stl中的count()函数, 其返回值就必须为iterator的difference_type
   ```
   template <class I, class T>
   Typename iterator_traits<I>::difference_type
   count(I first, I last, const T& value) {
        Typename iterator_traits<I>::difference_type n = 0;
        for(; first != last; first++)
            if (*first == value)
                n++;
        return n;
   }
   ```
   - reference type: 在c++中, 函数如果要传回左值, 则都是以by reference的方式进行, 根据值是否可变, 可以分为 T& 或const T&
   - pointer type: 指针型, 不必多说
   - iterator_category: iterator有以下五种分类, input iterator 这种iterator所指向的对象, 不允许外界改变, 是只读类型的; output iterator 只允许写入; forward iterator 允许读写; bidirectional iterator 可双向移动的iterator; random access iterator 前四种iterator都只提供一部分的指针算术能力(前三种支持operator++, 第四种又多支持operator--), 第五种则涵盖全部指针的算术能力, 包括p+n, p-n, p[n], p1-p2, p1 < p2...
5. iterator 中的traits: 类型萃取器. 主要是用来萃取指针所指向的元素type. 
