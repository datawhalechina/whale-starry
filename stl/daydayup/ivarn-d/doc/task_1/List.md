## 源码读取建议

>今天发现vscode有个很好用的功能，对于阅读源码感觉很重要，所以推荐一下，也可能好多人都用过

vscode有个大纲视图，可以用来查看源码结构，对于了解整体结构有很大帮助，例如：list有三个部分很重要，分别是链表节点(`_List_node，_List_node_base`) ，迭代器（`_List_iterator`），链表数据结构（`_List_base<_Tp, _Alloc>`），了解了这三个部分之后，根据这三部分去看它们的类继承关系与组合关系，然后去看各个地方的调用，接着去看实现的细节，以此来去学习，共勉。
![1.png](https://files.catbox.moe/j4kus4.png)

## 1.介绍

stl_list是一个双向循环链表。
分析gcc中的stl_list，把握住整体结构即可，实现总共由三部分组成，链表节点(`struct _List_node : public __detail::_List_node_base`) ，迭代器（`struct _List_iterator`），链表数据结构（`class list : protected _List_base<_Tp, _Alloc>`）。

*注意：list 源码里其实分了两个部分，一个部分是 list 结构，另一部分是 list 节点的结构。*
***
## 2.解析
### 2.1链表结点结构
基类_List_node_base只有`_M_prev`, `_M_prev`，分别指向前置节点和后继节点
```
struct _List_node_base
      _List_node_base* _M_next;
      _List_node_base* _M_prev;
```
派生类_List_node还有_M_data，用于存放数据项
```
template <class _Tp>
struct _List_node : public _List_node_base {
  _Tp _M_data;
}
```

***list基本结构图片***

### 2.2List接口
#### 2.2.1 默认构造

#### 2.2.2 析构函数

#### 2.2.3 resize
遍历一遍list, 得到链表长度，然后根据长度判断是否对链表进行增长或减短 - 如果是前者，调用insert在链表尾部插入new_size - __len个值为__x的节点 - 如果是后者，调用erase在链表尾部删除__len - new_size个节点 因为会遍历全部节点，最好不要执行list::resize, list.size同理
```
template <class _Tp, class _Alloc>
void list<_Tp, _Alloc>::resize(size_type __new_size, const _Tp& __x)
{
  iterator __i = begin();
  size_type __len = 0;
  for ( ; __i != end() && __len < __new_size; ++__i, ++__len)
    ;
  if (__len == __new_size)
    erase(__i, end());
  else                          // __i == end()
    insert(end(), __new_size - __len, __x);
}
```

#### 2.2.4 reverse
从头节点_M_node开始逆序遍历链表，交换所有节点的prev和next指针
```
inline void __List_base_reverse(_List_node_base* __p)
{
  _List_node_base* __tmp = __p;
  do {
    __STD::swap(__tmp->_M_next, __tmp->_M_prev);
    __tmp = __tmp->_M_prev;     // Old next node is now prev.
  } while (__tmp != __p);
}
```

#### 2.2.5 sort
sort代码如下所示。list排序时，链表中维护临时链表__carry和链表数组__counter[64], 和__fill状态。其中__counter用于暂存当前的排序结果，\_\_fill表示当前已被使用的__counter数组中的链表数量。
*注意：代码中调用merge方法时，会将两个list按照顺序合并成一个。假设有两个list a与b, 调用a.merge(b)之后，b中所有节点都被转移到a中，b为空链表，a中包含了所有的节点，并按照顺序排列。*
```
template <class _Tp, class _Alloc>
void list<_Tp, _Alloc>::sort()
{
  // Do nothing if the list has length 0 or 1.
  if (_M_node->_M_next != _M_node && _M_node->_M_next->_M_next != _M_node) {
    list<_Tp, _Alloc> __carry;
    list<_Tp, _Alloc> __counter[64];
    int __fill = 0;
    while (!empty()) {
      __carry.splice(__carry.begin(), *this, begin());
      int __i = 0;
      while(__i < __fill && !__counter[__i].empty()) {
        __counter[__i].merge(__carry);
        __carry.swap(__counter[__i++]);
      }
      __carry.swap(__counter[__i]);         
      if (__i == __fill) ++__fill;
    } 

    for (int __i = 1; __i < __fill; ++__i)
      __counter[__i].merge(__counter[__i-1]);
    swap(__counter[__fill-1]);
  }
}
```

***
## 3.实现





***
## 4.总结
>data：6/20
>没有写完，最近太忙了，之后补吧，希望不会是一句空话





***
## 5.参考
[STL源码分析与实现-stl_list容器 - 还在 - 博客园 (cnblogs.com)](https://www.cnblogs.com/blog-yejy/p/9535840.html)
[侯捷C++ 视频](https://github.com/ZachL1/Bilibili-plus)
[超硬核 | 2 万字+20 图带你手撕 STL 序列式容器源码](https://zhuanlan.zhihu.com/p/359213877)
[C++ STL源码剖析之双向环形链表list - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/85807484)
[STL源码分析--list - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/342531696)
[STL源码分析--内存分配器 (qq.com)](https://mp.weixin.qq.com/s/ASlSuFJT4uJ22xateeNP1g)
[STL源码学习（4）- list详解 - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/613974966)
[(25条消息) 初识STL 剖析list部分源码_weixin_34234823的博客-CSDN博客](https://blog.csdn.net/weixin_34234823/article/details/92285844)
***
