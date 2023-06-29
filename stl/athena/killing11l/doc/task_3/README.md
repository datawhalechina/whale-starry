# Task3 的学习笔记
## 普通红黑树介绍
红黑树是一种接近平衡的二叉查找树，时间复杂度是`O(logN)`，在极端情况下，所有的节点可能位于根节点的左侧或右侧，那么二叉排序树就退化为一个链表。

红黑树的特性如下：

1. 红黑树节点是红色或者黑色
2. 根节点是黑色
3. 最后的外部空节点是黑色
4. 红色节点的子节点和父节点是黑色
5. 从根节点到叶子节点的的所有路径上不能有2个连续的红色节点
6. 从任一节点到叶子节点的所有路径都包含相同数目的黑色节点

## STL红黑树
`STL`的红黑树是作为关系型容器如`set`、`map`、`multiset`和`multimap`的底层实现来使用，同时也具备普通红黑树的所有特征，不同的地方在于`STL`的红黑树多了一个红色的头节点，这个头节点指向`root`节点也指向最左和最右节点。

## 红黑树迭代器
> 以下内容引用自[Light-City](https://github.com/Light-City/CPlusPlusThings/blob/master/src_analysis/stl/rb_tree.md)，感谢大佬分享。

红黑树迭代器里面有一个红黑树基类成员，然后通过该成员进行迭代器的相关操作。 同时，我们可以知道该迭代器属于`bidirectional_iterator_tag`。

里面也包含了萃取机相关需要的`typedef`。

```c++
template<typename _Tp>
struct _Rb_tree_iterator
{
    typedef _Tp  value_type;
    typedef _Tp& reference;
    typedef _Tp* pointer;

    typedef bidirectional_iterator_tag iterator_category;
    typedef ptrdiff_t                  difference_type;

    typedef _Rb_tree_iterator<_Tp>        _Self;
    typedef _Rb_tree_node_base::_Base_ptr _Base_ptr;
    typedef _Rb_tree_node<_Tp>*           _Link_type;

    _Base_ptr _M_node;
};   
```

获取数据
```c++
reference
operator*() const _GLIBCXX_NOEXCEPT
{ return *static_cast<_Link_type>(_M_node)->_M_valptr(); }

pointer
operator->() const _GLIBCXX_NOEXCEPT
{ return static_cast<_Link_type> (_M_node)->_M_valptr(); }
```

重载++操作符
```c++
_Self&
operator++() _GLIBCXX_NOEXCEPT
{
    _M_node = _Rb_tree_increment(_M_node);
    return *this;
}
```

而`_Rb_tree_increment`底层是`local_Rb_tree_increment`，如下实现：
```c++
static _Rb_tree_node_base *
local_Rb_tree_increment( _Rb_tree_node_base* __x ) throw ()
{
	if ( __x->_M_right != 0 )       /* 存在右子树,那么下一个节点为右子树的最小节点 */
	{
		__x = __x->_M_right;
		while ( __x->_M_left != 0 )
			__x = __x->_M_left;
	}else  {                        
        /* 不存在右子树,那么分为两种情况：自底往上搜索,当前节点为父节点的左孩子的时候,父节点就是后继节点； */
/* 第二种情况:_x为header节点了,那么_x就是最后的后继节点. 简言之_x为最小节点且往上回溯,一直为父节点的右孩子,直到_x变为父节点,_y为其右孩子 */
		_Rb_tree_node_base *__y = __x->_M_parent;
		while ( __x == __y->_M_right )
		{
			__x	= __y;
			__y	= __y->_M_parent;
		}
		if ( __x->_M_right != __y )
			__x = __y;
	}
	return (__x);
}
```

重载--操作符：
```c++
_Self&
operator--() _GLIBCXX_NOEXCEPT
{
	_M_node = _Rb_tree_decrement(_M_node);
    return *this;
}
```

同理，而`_Rb_tree_decrement`底层是`local_Rb_tree_decrement`，如下实现：
```c++
static _Rb_tree_node_base *
local_Rb_tree_decrement( _Rb_tree_node_base * __x )
throw ()
{
/* header节点 */
	if ( __x->_M_color ==
	     _S_red
	     && __x
	     ->_M_parent->_M_parent == __x )
		__x = __x->_M_right;
	else if ( __x->_M_left != 0 ) /* 左节点不为空,返回左子树中最大的节点 */
	{
		_Rb_tree_node_base *__y = __x->_M_left;
		while ( __y->_M_right != 0 )
			__y = __y->_M_right;
		__x = __y;
	}else  {
/* 自底向上找到当前节点为其父节点的右孩子,那么父节点就是前驱节点 */
		_Rb_tree_node_base *__y = __x->_M_parent;
		while ( __x == __y->_M_left )
		{
			__x	= __y;
			__y	= __y->_M_parent;
		}
		__x = __y;
	}
	return
		(__x);
}
```
## 红黑树插入操作
红黑树的插入和二叉排序树插入相同，遵循左左右，右右左，左右左，右左右的规律。之所以要进行旋转操作，是为了保持红黑树的特性以及避免树过深。

### 规律解释：
首先定义c为当前节点，p为父节点，g为祖父节点，u为叔叔节点。
* 左左右：c为p左孩子，p为g左孩子，则进行右旋操作
* 右右左：c为p右孩子，p为g右孩子，则进行左旋操作
* 左右左：p为g左孩子，c为p右孩子，则进行左旋操作
* 右左右：p为g右孩子，c为p左孩子，则进行右旋操作