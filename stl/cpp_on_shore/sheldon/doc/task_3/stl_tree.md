# stl_tree源码阅读

## 平衡二叉搜索树概述

二叉搜索树是一种可以在 `O(logn)` 的时间复杂度下搜索到元素的数据结构，但是当二叉搜索树严重不平衡时，二叉搜索树就退化成了有序链表，为了避免这个现象，就需要平衡二叉搜索树。

根据平衡条件的不同，平衡二叉搜索树包括AVL-tree，RB-tree，AA-tree等结构。AVL-tree的平衡条件是要求任何节点的左右子树高度相差最多为1。RB-tree的平衡条件有所不同，而且是STL实现关联性容器的基础。RB-tree的平衡性比AVL-tree弱，但是搜寻平均效率几乎与AVL-tree相等。

## RB-tree

所谓 RB-tree(红黑树)，不仅是一个二叉搜索树，而且必须满足以下规则:

1. 每个节点不是红色就是黑色
2. 根节点为黑色
3. 如果节点为红，其子节点必须为黑
4. 任一节点至NULL (树尾端) 的任何路径，所含之黑节点数必须相同

STL中，树型结构的关联式容器主要有四种：`map`, `set`, `multimap`, `multiset`。这些容器内部元素都是按照key来进行排序的。

### RB-tree节点设计

#### struct _Rb_tree_node_base

```cpp
struct _Rb_tree_node_base
{
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef const _Rb_tree_node_base* _Const_Base_ptr;

    _Rb_tree_color	_M_color;
    _Base_ptr		_M_parent; // 指向父节点
    _Base_ptr		_M_left;// 指向左孩子
    _Base_ptr		_M_right;// 右孩子
    _S_maximum(); //找到最大值
    _S_minimum(); // 找到最小值
};
```

#### struct _Rb_tree_header

header的作用：方便构造和初始化tree_header，并且有成员变量记录节点数量

```cpp
 struct _Rb_tree_header
  {
    _Rb_tree_node_base	_M_header;
    size_t		_M_node_count; // 记录tree的节点数量

    _Rb_tree_header() _GLIBCXX_NOEXCEPT // 普通构造
    {
      ...
    }

#if __cplusplus >= 201103L
    _Rb_tree_header(_Rb_tree_header&& __x) noexcept // 移动构造函数， __x是右值引用
    {
      ...
      _M_move_data(__x); //移动操作
      ...
    }
#endif

    void
    _M_move_data(_Rb_tree_header& __from) // 移动
    {
      _M_header._M_color = __from._M_header._M_color;
      _M_header._M_parent = __from._M_header._M_parent;
      _M_header._M_left = __from._M_header._M_left;
      _M_header._M_right = __from._M_header._M_right;
      _M_header._M_parent->_M_parent = &_M_header;
      _M_node_count = __from._M_node_count;

      __from._M_reset();
    }

    void
    _M_reset()  // 初始化
    {
      _M_header._M_parent = 0;
      _M_header._M_left = &_M_header;
      _M_header._M_right = &_M_header;
      _M_node_count = 0;
    }
  };
```

#### struct _Rb_tree_node : public _Rb_tree_node_base

这是 RB_tree的节点，继承自 `_Rb_tree_node_base `

```cpp
template<typename _Val>
    struct _Rb_tree_node : public _Rb_tree_node_base
    {
      typedef _Rb_tree_node<_Val>* _Link_type; // 这里感觉没啥用
      _Val _M_value_field; // 节点值
    };
```

### rb_tree的迭代器

#### struct _Rb_tree_iterator

注意区分**前置++和后置++**

```cpp
template<typename _Tp>
    struct _Rb_tree_iterator
    {
      typedef _Tp  value_type;
      typedef _Tp& reference;
      typedef _Tp* pointer;

      typedef _Rb_tree_iterator<_Tp>		_Self;
      typedef _Rb_tree_node_base::_Base_ptr	_Base_ptr;
      typedef _Rb_tree_node<_Tp>*		_Link_type;

      ...

      reference
      operator*() const _GLIBCXX_NOEXCEPT
      { return *static_cast<_Link_type>(_M_node)->_M_valptr(); }

      pointer
      operator->() const _GLIBCXX_NOEXCEPT
      { return static_cast<_Link_type> (_M_node)->_M_valptr(); }

      _Self&
      operator++() _GLIBCXX_NOEXCEPT // 前置++，返回的是引用
      {
	_M_node = _Rb_tree_increment(_M_node); // 该函数在src/c++98/tree.cc中，是指向比当前_M_node大的第一个node，具体逻辑在函数 local_Rb_tree_increment 中
	return *this;
      }

      _Self
      operator++(int) _GLIBCXX_NOEXCEPT  // 后置++，返回的是自增前的对象
      {
	_Self __tmp = *this;
	_M_node = _Rb_tree_increment(_M_node);
	return __tmp;
      }

      _Self&
      operator--() _GLIBCXX_NOEXCEPT // 前置--，返回的是引用
      {
	_M_node = _Rb_tree_decrement(_M_node);// 该函数在src/c++98/tree.cc中，是指向比当前_M_node小的第一个node，具体逻辑在函数 local_Rb_tree_decrement 中
	return *this;
      }

      _Self
      operator--(int) _GLIBCXX_NOEXCEPT  // 后置--，返回的是自减前的对象
      {
	_Self __tmp = *this;
	_M_node = _Rb_tree_decrement(_M_node);
	return __tmp;
      }

      friend bool
      operator==(const _Self& __x, const _Self& __y) _GLIBCXX_NOEXCEPT
      { return __x._M_node == __y._M_node; }

      friend bool
      operator!=(const _Self& __x, const _Self& __y) _GLIBCXX_NOEXCEPT
      { return __x._M_node != __y._M_node; }

      _Base_ptr _M_node;
  };
```



### rb_tree的查找 find

rb_tree 的 find是里用二分法来进行key的查找。

```cpp
  template<typename _Key, typename _Val, typename _KeyOfValue,
	   typename _Compare, typename _Alloc>
    typename _Rb_tree<_Key, _Val, _KeyOfValue,
		      _Compare, _Alloc>::iterator
    _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
    find(const _Key& __k)
    {
      iterator __j = _M_lower_bound(_M_begin(), _M_end(), __k);
      return (__j == end()
	      || _M_impl._M_key_compare(__k,
					_S_key(__j._M_node))) ? end() : __j;
    }
```

### rb_tree的插入

平衡树的插入操作主要包括两个左旋和右旋操作。STL中的代码及注释如下。

要注意的是，在**rb_tree中除了要维护二叉树中基本的左右孩子指针，还要维护父节点的指针**。

```cpp
  static void
  local_Rb_tree_rotate_right(_Rb_tree_node_base* const __x,
			     _Rb_tree_node_base*& __root)
  // 右 旋 __x: __x的左孩子和__x的一起往右旋转
  {
    _Rb_tree_node_base* const __y = __x->_M_left; // 保存__x的左孩子为__y，右旋即为把 __y 和 __x 向右旋转

    __x->_M_left = __y->_M_right; // __x的左孩子指向__y的右孩子（由二叉搜索树的性质即可明白其中的含义）
    if (__y->_M_right != 0) // 如果__y的右孩子不是空
      __y->_M_right->_M_parent = __x; // 那么把__y的右孩子的父节点设为 __x
    __y->_M_parent = __x->_M_parent; // 更新__y的父节点

    // 下面这一段就是更新 __x的父节点的孩子的指向
    if (__x == __root) // 如果 __x 是根节点
      __root = __y;
    else if (__x == __x->_M_parent->_M_right) // 如果 __x 是其父节点的右孩子
      __x->_M_parent->_M_right = __y; 
    else // 如果 __x 是其父节点的左孩子
      __x->_M_parent->_M_left = __y;
    __y->_M_right = __x; // __y的右孩子指向__x，完成右旋
    __x->_M_parent = __y;// 更新 __x 的父节点
  }
```

```cpp
  static void
  local_Rb_tree_rotate_left(_Rb_tree_node_base* const __x,
		             _Rb_tree_node_base*& __root)
  // 左 旋 __x：__x的右孩子和__x的一起往左旋转
  {
    _Rb_tree_node_base* const __y = __x->_M_right; // 保存 __x的右孩子

    __x->_M_right = __y->_M_left;// __x的右孩子指向__y的左孩子
    if (__y->_M_left !=0) // 如果__y的左孩子 不为空
      __y->_M_left->_M_parent = __x; // 更新__y的左孩子的父节点为__x
    __y->_M_parent = __x->_M_parent; // 更新__y的父节点
	// 下面这一段就是更新 __x的父节点的孩子的指向
    if (__x == __root)
      __root = __y;
    else if (__x == __x->_M_parent->_M_left)
      __x->_M_parent->_M_left = __y;
    else
      __x->_M_parent->_M_right = __y;
    
    __y->_M_left = __x;// __y的左孩子指向__x，完成左旋
    __x->_M_parent = __y;// 更新 __x 的父节点
  }
```

rb-tree中的插入操作包括两种，`insert_unique()`和`insert_equal()`。

`insert_unique()`表示插入的 key 在树中是独一无二的。

`insert_equal()`表示插入的节点键值在整棵树中是可以重复的。

源码在`stl_tree.h`和`tree.cc`中并没有找到。

## 存疑

rb_tree的`insert_unique()`和`insert_equal()`源码？