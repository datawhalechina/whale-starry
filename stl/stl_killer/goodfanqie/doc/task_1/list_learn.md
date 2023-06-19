## STL - list 源码阅读笔记

### 结构体list_node的基础实现

在gcc9中的list实现中，最基础的结构其实是`_List_node_base`这个结构体。

+ 每个成员的细节如下：

`_M_next`：表示下一个节点指针。这是一个指向 _List_node_base 类型的指针。
`_M_prev`：表示上一个节点指针。这是一个指向 _List_node_base 类型的指针。
`swap(_List_node_base& __x, _List_node_base& __y)`：静态成员函数。用于交换两个 _List_node_base 对象的成员值。__x 和 __y 对象的类型都必须是 _List_node_base 或者 _List_node。
`_M_transfer(_List_node_base* const __first, _List_node_base* const __last)`：用于将链表中一个区间从 [__first, __last) 移动到当前节点前面。这个函数是无异常抛出保证的函数。
`_M_reverse()`：用于翻转链表的顺序，也就是将链表变成倒序模式。这个函数也是无异常抛出保证的函数。
`_M_hook(_List_node_base* const __position)`：用于将一个节点插到链表的指定位置前面。这个函数也是无异常抛出保证的函数。位置 __position 必须是当前链表中的节点或者是链表头节点 _M_prev。
`_M_unhook()`：用于将当前节点从链表中删除。这个函数也是无异常抛出保证的函数，并且它不会释放节点的内存，只是将链表中相邻节点连接起来。


其次是`_List_node_header `

+ 实现细节如下：
`_M_storage`：通过使用一个 __aligned_membuf 的对象来进行内存分配，用于保存节点存储元素的空间。__aligned_membuf 是一个定义在STL内部的类，它可以分配和管理对齐的内存，这避免了内存泄漏等许多问题。
`_M_valptr()`: 用于返回当前节点存储元素的指针，返回的指针类型为 _Tp*。
`_M_valptr() const`: 与上面的函数相同，用于在被 const 限定的对象中返回一个指向存储元素的指针，返回的指针类型为 _Tp const *，也就是常量指针。
注意，这个结构体是一个内部实现细节，用户在使用链表时并不需要直接操作这个结构体。


以上两个可以说是list实现的基础，往后的搭建都是在这个基础上进行的，但是STL与一般的程序不同，他所需要更多的是稳定性跟可扩展性。不仅需要讲究更多的安全措施还通过c++强大的模板编程使得STL在可扩展性上也变得非常出色。



### List_iterator迭代器的实现

现在来到了STL容器中非常重用的一环：迭代器，这个被称为STL容器的胶水的组件。
下面对代码进行解释：

+ `_List_iterator`是一个模板结构体，用于定义list的迭代器。

+ `_Node`是list节点(_List_node)的类型定义，用于指向list中的节点。

+ 定义了一系列迭代器相关的类型别名，包括difference_type、iterator_category、value_type、pointer和reference。

+ `_List_iterator()`是默认构造函数，初始化一个空的迭代器。

+  `_List_iterator(__detail::_List_node_base* __x)`是显式构造函数，接受一个指向基类节点(_List_node_base)的指针，并将其初始化为迭代器的节点。

+  `_M_const_cast()`是一个私有成员函数，用于进行常量转换，返回一个const版本的迭代器。

+  `operator*()`重载了解引用操作符，返回迭代器指向节点的值的引用。

+  `operator->()`重载了箭头操作符，返回迭代器指向节点的值的指针。

+  `operator++()`和`operator++(int)`重载了前置和后置自增操作符，用于将迭代器向后移动一个位置。

+  `operator--()`和`operator--(int)`重载了前置和后置自减操作符，用于将迭代器向前移动一个位置。

+  `operator==()`和`operator!=()`重载了相等和不等操作符，用于比较两个迭代器是否相等。

+  `_M_node`是迭代器唯一的成员变量，指向当前迭代器所指向的节点。

通过迭代器可以访问list中的元素，并支持前后移动、比较等操作。通过迭代器，可以实现list的遍历和操作。


### List_base 的实现


+ `_List_base`是一个模板类，用于为list提供基础功能。

+ `_Tp_alloc_type`和`_Node_alloc_type`是类型别名，分别表示元素类型的分配器和节点类型的分配器。

+ `_List_impl`是一个嵌套结构体，继承自节点分配器类型，包含一个节点头(_List_node_header)。节点头包含指向链表首尾的指针。

+ `_List_impl()`是默认构造函数，初始化一个空的链表实现。

+ `_List_impl(const _Node_alloc_type& __a)`是构造函数，接受一个节点分配器，用于初始化链表实现。

+ `_M_get_size()`、`_M_set_size()`、`_M_inc_size()`和`_M_dec_size()`是函数，用于获取、设置、增加和减少链表的大小。

+ `_M_distance()`是函数，用于计算两个节点之间的距离（元素个数）。

+ `_M_node_count()`是函数，用于返回链表中节点的个数。

实现了链表基类的一些基本功能，包括链表的大小操作、节点距离计算等。它还定义了链表实现的结构和相关的成员函数，为派生类提供了基础支持。


### List的最终实现


+ `list`是一个模板类，继承自`_List_base<_Tp, _Alloc>`，用于实现list容器。

+ `_Tp`是元素类型，`_Alloc`是分配器类型，默认为`std::allocator<_Tp>`。

+ `static_assert`用于编译时断言，判断`_Tp`是否为非const、非volatile的类型，确保`std::list`必须具有非const、非volatile的`value_type`。

+ `_Base`是基类`_List_base<_Tp, _Alloc>`的类型别名。

+ `_Tp_alloc_type`、`_Tp_alloc_traits`、`_Node_alloc_type`和`_Node_alloc_traits`是类型别名，分别表示元素类型的分配器、元素类型的分配器特性、节点类型的分配器和节点类型的分配器特性。

+ 类中定义了一系列类型别名，包括`value_type`、`pointer`、`const_pointer`、`reference`、`const_reference`、`iterator`、`const_iterator`、`const_reverse_iterator`、`reverse_iterator`、`size_type`、`difference_type`和`allocator_type`。

+ `_Node`是节点类型`_List_node<_Tp>`的类型别名。

+ `使用了`using`关键字，继承了基类`_List_base`中的成员函数和成员变量。

定义了list容器的类型别名，包括迭代器类型、大小类型等，并继承了基类`_List_base`中的成员函数和成员变量。这些类型别名和继承的成员函数和成员变量为list容器提供了基本的功能和支持。


### 总结

因为时间原因跟期末复习，所以一些具体函数的实现没有去深究，但是从目前来看，我还是能被STL源码实现的各种细节吸引，对模板的深刻应用，以及如何在保护底层实现的同时又对外部使用展现一些非常优秀的接口，各种细节的把握以及对性能的深究都足以说明STL的分量。