## Task1 的学习笔记
### std::List默认构造器std::allocator<T>
std::allocator提供了一种类型感知的内存分配方法，它分配的内存是原始的、未构造的，当一个allocator对象分配内存时，它会根据给定的对象类型来确定恰当的内存大小和对齐位置。
```c++
std::allocator<T> a;

a.allocate(n) // 分配一段原始的、未构造的内存，保存n个类型为T的对象

/**
 * 释放从T*指针p中地址开始的内存，这块内存保存了n个类型为T的对象；p必
 * 须是前一个a.allocate()返回的指针，且n必须是P创建时所要求的的大小
 * 在这之前必须先调用a.destroy()。
 */
a.deallocate(p, n)

a.construct(p, args) // p指向一块原始内存，args传递给T的构造函数。

a.destroy(p) // 对保存对象进行析构
```
### 部分接口说明
1. splice
```c++
/**
 * 把list2中的内容剪切（不是复制）并从list1的position位置开始插入，意
 * 味着调用结束后list2将被置空。
*/
list1.splice (iterator position, list2)

/**
 * 把list2中从i位置的单个数据剪切（不是复制）并插入到list1的position位置
*/
list1.splice (iterator position, list2, iterator i)

/**
 * 把list2中从first开始到last结束 （不包括last位置），这段区间内的数据剪
 * 切并从list1的position的位置开始插入。
*/
list1.splice (iterator position, list2, iterator first, iterator last)
```
2. push_front & push_back & insert
所有关于插入的实现最终都指向了`_M_hook`函数，以`push_front`为例，函数调用关系如下：
>- void push_front( T&& value ); // (C++11 起)
>    - void _M_insert(begin(), std::move(value))
>        - void _M_hook(_List_node_base* const __position)

最终`_M_hook`函数进行插入，实现如下：
```c++
/**
 * this表示将要插入的node节点，__position表示当前链表中的需要插入的位置之后的节点
*/
void _List_node_base:: _M_hook(_List_node_base* const __position) _GLIBCXX_USE_NOEXCEPT
{
    this->_M_next = __position;
    this->_M_prev = __position->_M_prev;
    __position->_M_prev->_M_next = this;
    __position->_M_prev = this;
}
```
### 总结
* list使用动态存储，可以对任意位置快速插入和删除，但是遍历的速度比数组慢，占用空间比数组大。
* list的迭代器不会因为插入和删除操作而失效。
* 若想访问中间部分元素，可以使用`std::advance(it , n)`将迭代器it向后移动n个元素位置，n可以为负数。