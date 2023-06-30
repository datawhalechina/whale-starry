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

## 红黑树插入操作
红黑树的插入和二叉排序树插入相同，遵循左左右，右右左，左右左，右左右的规律。之所以要进行旋转操作，是为了保持红黑树的特性以及避免树过深。

### 规律解释：
首先定义c为当前节点，p为父节点，g为祖父节点，u为叔叔节点。
* 左左右：c为p左孩子，p为g左孩子，则进行右旋操作
* 右右左：c为p右孩子，p为g右孩子，则进行左旋操作
* 左右左：p为g左孩子，c为p右孩子，则进行左旋操作
* 右左右：p为g右孩子，c为p左孩子，则进行右旋操作

### `_Rb_tree`插入流程
以其中的一个`insert`流程分析。
```c++
template<typename _Key, typename _Val, typename _KeyOfValue, typename _Compare, typename _Alloc>
template<typename _Arg>
pair<typename _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::iterator, bool>
 _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::_M_insert_unique(_Arg&& __v)
{
	typedef pair<iterator, bool> _Res;
	// 判断是否已存在
	pair<_Base_ptr, _Base_ptr> __res = _M_get_insert_unique_pos(_KeyOfValue()(__v));

	if (__res.second)
	{
		_Alloc_node __an(*this);
		// 插入元素
		return _Res(_M_insert_(__res.first, __res.second, _GLIBCXX_FORWARD(_Arg, __v), __an), true);
	}

	// 插入失败，外部可以查看pair的second成员检查成功与否
	return _Res(iterator(__res.first), false);
}

template<typename _Key, typename _Val, typename _KeyOfValue, typename _Compare, typename _Alloc>
pair<typename _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::_Base_ptr,
	typename _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::_Base_ptr>
		_Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::_M_get_insert_unique_pos(const key_type& __k)
{
	typedef pair<_Base_ptr, _Base_ptr> _Res;
	_Link_type __x = _M_begin();
	_Base_ptr __y = _M_end();
	bool __comp = true;
	while (__x != 0) {
		__y = __x;
		__comp = _M_impl._M_key_compare(__k, _S_key(__x)); // return (__k < _S_key(__x))
		__x = __comp ? _S_left(__x) : _S_right(__x);
	}
	iterator __j = iterator(__y);
	
	if (__comp) {
		if (__j == begin())
		return _Res(__x, __y);
		else
		--__j;
	}
	if (_M_impl._M_key_compare(_S_key(__j._M_node), __k))
		return _Res(__x, __y);
	return _Res(__j._M_node, 0);
}

template<typename _Key, typename _Val, typename _KeyOfValue, typename _Compare, typename _Alloc>
template<typename _Arg, typename _NodeGen>
typename _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::iterator _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::_M_insert_(_Base_ptr __x, _Base_ptr __p, _Arg&& __v, _NodeGen& __node_gen)
{
	bool __insert_left = (__x != 0 || __p == _M_end() || _M_impl._M_key_compare(_KeyOfValue()(__v), _S_key(__p)));

	_Link_type __z = __node_gen(_GLIBCXX_FORWARD(_Arg, __v));

	_Rb_tree_insert_and_rebalance(__insert_left, __z, __p, this->_M_impl._M_header); // 插入元素并根据红黑树特性进行修改。
	++_M_impl._M_node_count;
	return iterator(__z);
}
```

## `_Rb_tree`删除流程
当我们尝试删除一个节点时，我们需要确保删除操作后，红黑树依然满足上述性质。以下是红黑树删除节点的算法步骤：
1. 如果待删除节点有两个非叶子子节点，找到它的后继节点（即大于待删除节点的最小节点），然后将待删除节点的值替换为后继节点的值，并将问题转化为删除后继节点。此时，后继节点最多有一个非叶子子节点。
2. 如果待删除节点是红色，直接删除即可。删除后红黑树依然满足性质。
3. 如果待删除节点是黑色，且有一个红色子节点，可以将子节点染黑，然后删除待删除节点。此操作保持红黑树性质。
4. 如果待删除节点是黑色，且有一个黑色子节点，此时删除操作会破坏红黑树的性质。需要进行调整。主要分以下四种情况处理：
	> * 情况1：兄弟节点是红色，将兄弟节点染黑，父节点染红，进行旋转。
	> * 情况2：兄弟节点是黑色，且兄弟节点的两个子节点都是黑色，将兄弟节点染红，然后将父节点视为待处理节点，继续调整。
	> * 情况3：兄弟节点是黑色，且兄弟节点的近侧子节点是红色，远侧子节点是黑色，将兄弟节点染红，近侧子节点染黑，然后围绕兄弟节点进行旋转，转化为情况4。
	> * 情况4：兄弟节点是黑色，且兄弟节点的远侧子节点是红色，将兄弟节点染成父节点的颜色，父节点染黑，远侧子节点染黑，然后围绕父节点进行旋转，调整结束。

部分源码：
```c++
iterator erase(const_iterator __position)
{
	__glibcxx_assert(__position != end());
	const_iterator __result = __position;
	++__result;
	_M_erase_aux(__position);
	return __result._M_const_cast();
}

template<typename _Key, typename _Val, typename _KeyOfValue,
	typename _Compare, typename _Alloc>
void
_Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
_M_erase_aux(const_iterator __position)
{
	_Link_type __y =
		static_cast<_Link_type>(_Rb_tree_rebalance_for_erase(const_cast<_Base_ptr>(__position._M_node), this->_M_impl._M_header)); // 删除节点并根据特性保持树的相对平衡
	_M_drop_node(__y);
	--_M_impl._M_node_count;
}
```