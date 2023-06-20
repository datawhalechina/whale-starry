# Task01---STL_List源码学习笔记

## 1. 节点结构设计

在C++ STL标准模板库中，**<stl_list.h>** 文件定义了一个双向链表容器，该容器允许在任意位置进行元素的插入与删除修改，并且不会对其他元素位置产生影响。

在 **\_\_detail** 命名空间中，为减少代码重复，定义了一个通用的List节点结构 **\_List_node_base** ：

> ```c++
> struct _List_node_base
> {
> 	_List_node_base* _M_next;
> 	_List_node_base* _M_prev;
> 	
>     static void swap(_List_node_base& __x, _List_node_base& __y) _GLIBCXX_USE_NOEXCEPT;
> 
>     void _M_transfer(_List_node_base* const __first, _List_node_base* const __last) _GLIBCXX_USE_NOEXCEPT;
> 
>     void _M_reverse() _GLIBCXX_USE_NOEXCEPT;
> 
>     void _M_hook(_List_node_base* const __position) _GLIBCXX_USE_NOEXCEPT;
> 
>     void _M_unhook() _GLIBCXX_USE_NOEXCEPT;
> };
> ```

该结构体中

- ***\_M_next*** 与 ***\_M_prev*** 分别对双向链表中指向前后节点的指针进行了定义；
- ***swap(\_\_x, \_\_y)*** 则作为一个静态成员函数负责内容交换，以实现节点内链接关系的同时更新；
- ***\_M_transfer(\_List_node_base\* const \_\_first, \_List_node_base\* const \_\_last)*** 负责实现一段节点的位置移动，***’\_\_first‘*** 决定起始位置，***’\_\_last‘*** 决定结束位置
- ***\_M_reverse()*** 用于反转调整节点间连接关系，实现节点前后关系的倒置
- ***\_M_hook(\_List_node_base\* const \_\_position)*** 通过接受参数 ***'\_\_position'*** 将当前节点插入到参数给定的位置之前
- ***\_M_unhook()*** 断开前后连接关系，实现当前节点的移除

在完成对通用结构体的定义后，源码中还定义了一个结构体模板 ***\_List_node***：

> ```c++
> 
> template<typename _Tp>
>     struct _List_node : public __detail::_List_node_base
>     {
>       ///< User's data.
>       _Tp _M_data;
>     };
> 
> ```

该结构体模板继承自前面定义的通用结构体 ***\_\_detail::\_List_node_base*** ，用于表示双向链表中的节点，并实现基类的重用和拓展。此外该结构体模板还定义了一个存储用户自定义数据的成员变量 ***'\_M_data'*** ，该变量可以根据节点的具体需求来确定数据类型。

总体而言，STL中链表节点在实现时被拆分为了 **指针域** 与 **数据域** 两个结构体：

> ```c++
> // 指针域
> struct _List_node_base{
>     _List_node_base* _M_next;
> 	_List_node_base* _M_prev;
> };
> 
> // 数据域
> template<typename _Tp>
>     struct _List_node : public __detail::_List_node_base
>     {
>       ///< User's data.
>       _Tp _M_data;
>     };
> ```

- ***'\_List_node_base'*** 结构体作为指针域，仅包含指向前一个节点和后一个节点的指针，用于构建双向链表。由于不包含具体值，可用于任何类型的链表节点
- ***'\_List_node'*** 结构体中通过添加 ***'\_Tp'*** 类型的变量 ***'\_M_data'*** ，因此可作为数据域存储具体值，并且由于该结构体继承自 ***\_List_node_base*** ，故可以在保存具体类型的值的同时，还能通过 ***\_List_node_base*** 的指针进行链表节点操作。  

通过这种方式，链表在实现时更加灵活，***\_List_node_base*** 构建的链表仅需节点无需储值，因此有效节省了内存空间，并且其是作为容器的内部实现细节而隐藏的。***\_List_node*** 则实现了需要储值的链表，还能在需要是通过将 ***\_List_node_base\**** 转换为 ***\_List_node\**** 实现对节点值的访问和操作，与 ***\_List_node_base***不同，***\_List_node*** 是容器对外暴露的，用户可以基于此直接操作具体的节点值，而无需关心底层链表结构。



## 2. 迭代器设计

#### 结构体类型

定义完节点结构后，STL中还定义了List迭代器 ***\_List_iterator*** 的结构，以此实现对链表 ***\_List*** 中的元素进行迭代访问。

> ```c++
> template<typename _Tp>
>     struct _List_iterator
>     {
>         typedef _List_iterator<_Tp>                _Self;
>         typedef _List_node<_Tp>                    _Node;
>         ...
>     };
> ```

该结构体中包含了 ***'\_Self'*** 与 ***'\_Node'*** 两种类型，其中 ***'\_Self'*** 是对结构体 ***\_List_iterator*** 的一个自引用，以此实现在成员函数中对迭代器自身的返回；***'\_Node'*** 则是类型 ***\_List_node*** 的指代，既可以通过***'\_Node'*** 指向链表节点，也可通过将 ***'\_Node\*'*** 转化为 ***\_List_node<_Tp>\**** 来访问节点中的成员 ***'\_M_data'*** 存储的具体值。



#### 迭代器类型

在完成对结构体类型的定义后，为了使得 ***\_List_iterator*** 符合迭代器要求，并且可以在遍历链表时正确操作和访问节点值，该结构中其中还定义了其他类型：

> ```c++
>  		typedef ptrdiff_t                          difference_type;
>       	typedef std::bidirectional_iterator_tag    iterator_category;
>       	typedef _Tp                                value_type;
>       	typedef _Tp*                               pointer;
>       	typedef _Tp&                               reference;
> 
> 		...
> ```

- ***‘difference_type’*** 用于表示两个迭代器之间的距离的类型，因此在这里被定义为指针间的差值类型 ***'ptrdiff_t'***，由于链表属于非连续存储结构，迭代器无法直接使用指针运算计算迭代器间距离，引入***'difference_type'*** 定义类型后，可以使得链表迭代器与指针进行类似的操作，以此保证在不同容器间的一致性和通用性。
- ***'iterator_category'*** 作为迭代器的分类类型，表示迭代器的遍历方向与所支持的操作，在这里被定义为双向迭代器 ***'std::bidirectional_iterator_tag'*** 以此实现对前向和后向遍历的支持
- ***'value_type'*** 定义了迭代器指向的值的类型，这里被定义为模板参数 ***\_Tp*** 的类型
- ***'pointer'*** 定义了迭代器所指向值的指针的类型，这里被定义为指向 ***\_Tp*** 类型对象的指针 ***\_Tp\**** 
- ***'reference'*** 则定义了迭代器所指向值的引用类型，因此这里被定义为指向 ***\_Tp*** 类型对象的引用 ***\_Tp&*** 



#### 构造函数与成员函数

完成对类型的定义后，该迭代器结构中还定义了构造函数与成员函数，用于创建对象以及执行对象操作：

> ```C++
>  		_List_iterator() _GLIBCXX_NOEXCEPT
>       	: _M_node() { }
> 
>       	explicit
>       	_List_iterator(__detail::_List_node_base* __x) _GLIBCXX_NOEXCEPT
>       	: _M_node(__x) { }
> 
>      	_Self
>       	_M_const_cast() const _GLIBCXX_NOEXCEPT
>       	{
>          	return *this; 
>       	}
> 
>       	// Must downcast from _List_node_base to _List_node to get to _M_data.
>       	reference
>       	operator*() const _GLIBCXX_NOEXCEPT
>       	{ 
>           	return static_cast<_Node*>(_M_node)->_M_data; 
>       	}
> 
>       	pointer
>       	operator->() const _GLIBCXX_NOEXCEPT
>       	{ 
>           	return std::__addressof(static_cast<_Node*>(_M_node)->_M_data); 
>       	}
> 
>       	_Self&
>       	operator++() _GLIBCXX_NOEXCEPT
>       	{
>           	_M_node = _M_node->_M_next;
>           	return *this;
>       	}
> 
>       	_Self
>       	operator++(int) _GLIBCXX_NOEXCEPT
>       	{
>           	_Self __tmp = *this;
>           	_M_node = _M_node->_M_next;
>           	return __tmp;
>       	}
> 
>       	_Self&
>       	operator--() _GLIBCXX_NOEXCEPT
>       	{
>          	_M_node = _M_node->_M_prev;
>           	return *this;
>       	}
> 
>       	_Self
>       	operator--(int) _GLIBCXX_NOEXCEPT
>       	{
>           	_Self __tmp = *this;
>           	_M_node = _M_node->_M_prev;
>           	return __tmp;
>       	}
> 
>       	bool
>       	operator==(const _Self& __x) const _GLIBCXX_NOEXCEPT
>       	{
>           	return _M_node == __x._M_node; 
>       	}
> 
>       	bool
>       	operator!=(const _Self& __x) const _GLIBCXX_NOEXCEPT
>       	{ 
>           	return _M_node != __x._M_node; 
>       	}
> 
>       	// The only member points to the %list element.
>       	__detail::_List_node_base* _M_node;
> ```

在上述函数中：

- 默认构造函数 ***\_List_iterator()*** 负责在创建对象未提供参数时自动调用，以此将 ***\_List_node*** 中成员变量 ***\_M_data*** 初始化为空指针；显示构造函数 ***\_List_iterator(\_\_detail::_List_node_base\* \_\_x)*** 则负责接收 ***\_List_node_base*** 指针作为参数，自定义初始化并赋值给成员变量 ***\_M_data*** 。通过这两种函数，可以实现在不同情况下创建 ***_List_iterator*** 对象

- ***\_M_const_cast() const*** 是一个常量转换函数，由于 ***const*** 位于函数尾部，该函数作为常量函数，不能修改内部 ***’\_M_node'*** 成员变量的值。因此该函数直接返回当前迭代器对象***'\*this'*** ，来将当前迭代器对象的副本作为结果返回。通过这种方式来从常量迭代器中获取非常量迭代器对象，实现通过非常量迭代器修改迭代器所指向元素的值。

- 解引用操作符 ***operator\*() const*** 负责将***‘\_M_node’*** 强制转为 ***’\_Node\*‘*** 类型，以此访问成员变量 ***’\_M_data‘*** 获取节点值；成员访问操作符 ***operator->() const*** 则是返回迭代器指向节点值的指针，通过将***‘\_M_node’*** 强制转为 ***’\_Node\*‘*** 类型，使用 ***’std::\_\_addressof‘*** 来获取节点值地址。

- 递增操作符 ***operator++*** 中，前者 ***operator++()*** 在迭代器先向前移动一个节点后，再返回移动后迭代器的引用，返回的是递增后迭代器对象本身，因此是前置递增；后者 ***operator++(int)*** 则是在迭代器向前移动一个节点时，先返回移动前迭代器的副本，以便使用原始值进行计算或存储，即先返回原始值赋给递增后的节点，再将原始值进行递增，因此是后置递增。并且值得注意的是，与常量转换函数相同，该返回类型是 ***'\_Self'*** 即 ***’\_List_iterator<_Tp>‘*** ，返回了一个与当前对象相同类型的迭代器对象（即副本）。

- 递减操作符 ***operator--*** 与递增操作符定义同理。

- 最后两个则是相等比较符 ***'operator==(const _Self& \_\_x) const'*** 与 不等比较符 ***'operator!=(const _Self& \_\_x) const'*** ，前者负责比较当前迭代器与另一个迭代器是否指向相同节点，后者则比较二者是否指向不同节点，最终返回比较结果的布尔值。这两个比较符在下述函数中被重载。

  ```c++
    template<typename _Val>
      inline bool
      operator==(const _List_iterator<_Val>& __x, const _List_const_iterator<_Val>& __y) _GLIBCXX_NOEXCEPT
      { return __x._M_node == __y._M_node; }
  
    template<typename _Val>
      inline bool
      operator!=(const _List_iterator<_Val>& __x, const _List_const_iterator<_Val>& __y) _GLIBCXX_NOEXCEPT
      { return __x._M_node != __y._M_node; }
  ```

  

- ***\_M_node*** ：在整个迭代器中作为唯一成员变量，来以此标识迭代器的当前位置。



## 3. List 定义

在STL中，作为List容器的基类，***'\_List_base'*** 负责head节点的初始化，定义了容器的内部结构与内存分配。

#### 内存分配

在 ***'\_List_base'*** 中，内存分配是通过两个成员函数 ***'\_M_get_node()'*** 与 ***'\_M_put_node()'*** 实现

> ```c++
> _List_impl(): _Node_alloc_type(), _M_node()
> { }
> 
> _List_impl(const _Node_alloc_type& __a) _GLIBCXX_NOEXCEPT: _Node_alloc_type(__a), _M_node()
> { }
> 
> _List_node<_Tp>* _M_get_node()
> {
>     return _M_impl._Node_alloc_type::allocate(1);
> }
> 
> void _M_put_node(_List_node<_Tp>* __p) _GLIBCXX_NOEXCEPT
> {
>     _M_impl._Node_alloc_type::deallocate(__p, 1);
> }
> ```

函数 ***'\_M_get_node()'*** 通过 ***'\_M_impl'*** 调用 ***allocate()*** 函数请求分配内存，***'\_M_put_node()'*** 则使用 ***'\_M_impl'*** 调用 ***deallocate()*** 来释放内存，该函数接受的参数为所释放内存块的指针和释放的数量。 



#### 成员函数

> ```c++
> allocator_type get_allocator() const _GLIBCXX_NOEXCEPT
> {
>     return allocator_type(_M_get_Node_allocator()); 
> }
> 
> void _M_clear() _GLIBCXX_NOEXCEPT;
> 
> void _M_init() _GLIBCXX_NOEXCEPT
> {
>     this->_M_impl._M_node._M_next = &this->_M_impl._M_node;
>     this->_M_impl._M_node._M_prev = &this->_M_impl._M_node;
> }
> 
> ~_List_base() _GLIBCXX_NOEXCEPT
> {
>     _M_clear();
> }
> ```

在上述函数中：

- ***'get_allocator()' const*** 通过调用 ***'\_M_get_Node_allocator()'*** 函数获取List内节点分配器，并将其转换为 ***'allocator_type'*** 类型返回。用户提供调用该函数，可以获取列表的分配器，以将其用于构造List或插入元素等操作。
- ***'\_M_init()'*** 将 ***'\_M_impl'*** 对象中的节点指针 ***'\_M_node'*** 的 ***'\_M_next'*** 与 ***'\_M_prev'*** 指向自身，形成一个空循环链表，以此实现head节点的初始化。
- ***'\_M_clear()'*** 负责清空列表，以此示方所有节点所占用的内存
- ***'~_List_base()'***  负责调用 ***'\_M_clear()'*** 函数，以销毁 ***\_List_base*** 对象。由于 ***'~_List_base()'*** 无需显示调用，通过这种方式调用 ***'\_M_clear()'*** 可以确保在对象销毁时自动进行清理操作，以避免忘记因手动调用  ***'\_M_clear()'*** 导致资源泄露。



## 4. List 实现

在实现部分，***list*** 类模板基于 ***\_List_base<_Tp, _Alloc>*** 进行派生，同时指定分配器类型为 ***'\_Alloc'*** 

> ```c++
> template<typename _Tp, typename _Alloc = std::allocator<_Tp> >
> 	class list : protected _List_base<_Tp, _Alloc>
>     	{...}
> ```



#### 基本操作

> ```c++
> void pop_back() _GLIBCXX_NOEXCEPT  { this->_M_erase(iterator(this->_M_impl._M_node._M_prev)); }
> 
> void push_back(const value_type& __x)  { this->_M_insert(end(), __x); }
> 
> void pop_front() _GLIBCXX_NOEXCEPT  { this->_M_erase(begin()); }
> 
> void push_front(const value_type& __x)  { this->_M_insert(begin(), __x); }
> 
> void _M_insert(iterator __position, const value_type& __x)
> {
>     _Node* __tmp = _M_create_node(__x);
>     __tmp->_M_hook(__position._M_node);
> }
> 
> void _M_erase(iterator __position) _GLIBCXX_NOEXCEPT
> {
>     __position._M_node->_M_unhook();
>     _Node* __n = static_cast<_Node*>(__position._M_node);
>     _M_put_node(__n);
> }
> ```

上述函数共同实现了对 ***'list'*** 容器的基本操作：

- ***'\_M_insert(iterator __position, const value_type& \_\_x)'*** 用于在指定位置插入一个元素。它创建一个新节点，并通过调用 ***'_M_hook()'*** 函数将其插入到 ***'__position'*** 所指向的节点之前
- ***'void _M_erase(iterator __position) '*** 用于从容器中移除指定位置的元素。它先调用 ***'_M_unhook()'*** 函数将节点从链表中移除，然后通过调用 ***'_M_put_node'*** 函数释放节点的内存空间。
- ***'pop_back()'*** 函数用于移除 ***'list'*** 容器中的最后一个元素，通过调用 ***'\_M_erase()'*** 函数将指向最后一个元素的迭代器作为参数传递给 ***’\_M_erase()‘*** 函数
- ***'push_back()'*** 函数用于在 ***'list'*** 容器末尾插入元素，通过调用 ***'\_M_insert()'*** 函数将末尾迭代器作为参数传递给 ***’\_M_insert()‘*** 函数，同时传递插入的元素值 ***'\_\_x'***
- ***'pop_front()'*** 函数用于移除 ***'list'*** 容器中的第一个元素，通过调用 ***'\_M_erase()'*** 函数将指向第一个元素的迭代器作为参数传递给 ***’\_M_erase()‘*** 函数
- ***'push_front()'*** 函数用于在 ***'list'*** 容器头部插入元素，通过调用 ***'\_M_insert()'*** 函数将头部迭代器作为参数传递给 ***’\_M_insert()‘*** 函数，同时传递插入的元素值 ***'\_\_x'***