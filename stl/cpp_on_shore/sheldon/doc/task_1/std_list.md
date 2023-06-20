## STL: list

### list概述

list的底层实现是双头链表。

他的查询的时间复杂度是线性的，插入和删除元素的时间复杂度是常数级的，无论发生变化的元素位置在哪。不像`std::vector` 和 `std::deque`，list没有提供 random-access iterator，所以 subscripting access（即[]）是不允许的。

list可以表示为

```
A <---> B <---> C <---> D
```

list会有一个私有成员变量`list::iterator`，**指向 D，而不是 A**。而且真实情况是list是一个环状双头链表，D的next指向了A。这一点可以从源码的`list::begin()`函数中看出端倪。

### list源码阅读

#### 基础list_node结构

##### **struct _List_node_base**

这是list结点的基结构体

```cpp
_List_node_base* _M_next; //后一个节点
_List_node_base* _M_prev; //前一个节点
swap(_List_node_base& __x, 
    _List_node_base& __y);// 交换节点
_M_transfer(_List_node_base* const __first,
    _List_node_base* const __last);// ????
_M_reverse(); // 反转
_M_hook();
_M_unhook();
```



##### struct **_List_node_header**

```cpp
_M_size; // 记录list的大小C++11才有
_M_init(); // list初始化，主要是next、pre、size这几项
_M_base(); // 返回this
_M_move_nodes(_List_node_header&& __x); // 把__x替换了？
```



##### struct **_List_node**

这是list中的真实结点结构

```cpp
_Tp*   _M_valptr(); // 返回该node对应的指针
```



#### List的迭代器

##### **_List_iterator/List_const_iterator**

这是list中的迭代器。所有的函数都是运算符重载(op overloads)

```cpp
_List_node_base* _M_node; //唯一的成员变量，迭代器指向的list_node
operator*(); // iterator的解引用
operator->(); // 返回的是iter指向的node的指针
operator++(); // 后移
operator--(); // 前移
operator==(); // 迭代器 x 和迭代器 y指向的node是否相等。
operator!=()；// 迭代器 x 和迭代器 y指向的node是否不相等。
```



#### List的类

##### **class _List_base**

list的基类，该类的构造函数和析构函数会进行内存的分配与释放，但不会进行初始化。

```cpp
_List_base(const _Node_alloc_type& __a); % 分配 a 个node大小的空间；
~_List_base(); % 销毁，不过貌似啥也没销毁？
```



##### **class list**

```cpp
构造函数；// 初始化
析构函数；
begin(); //这个迭代器指向首元素
end(); //这个迭代器指向最后一个元素的下一个空节点
empty(); // 判断是否为空
size(); // 返回list大小
max_size(); //返回list可能的最大大小
resize(size_type __new_size); // 重新设置list的大小，size变小则删除，变大则以默认值添加
resize(size_type __new_size, const value_type& __x); // 重新设置list的大小，size变小则删除，变大则以给定值添加
front(); // 返回首元素
back(); // 返回最后一个元素
push_front(); // 从首元素前插入元素
emplace_front(); // 从首元素前插入元素
push_back();
emplace_back();
pop_front();
pop_back();
emplace(const_iterator __position, _Args&&... __args); // 在__position前插入对象
insert(const_iterator __position, const value_type& __x);
erase(iterator __position); // 清除__position上的元素
erase(iterator __first, iterator __last);// 清楚这一个范围内的元素。[first,last)
clear(); //清除所有元素
splice(); //
sort();// list不能使用STL提供的sort(),只能使用自己的sort
```



#### 继承关系

![stl_list](asset/stl_list.png)



### 存疑部分？

- allocter?





