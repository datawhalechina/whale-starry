- **关于迭代器iterator的补充**

    iterator主要是用来遍历容器中的数据节点，即不用在外部知道容器内部实现原理，就能遍历数据。**迭代器说白了就是一个带有模板性质的指针**，没什么特别的，我们可以定义成类或者结构体

    所以iterator的主要功能有：支持++，遍历元素; 支持*，取元素程序; 支持->，指针操作; 支持==和!=操作，比较iterator是否到了结尾。

    实现起来也很简单，参照不同容器下自己定义的iterator即可，std::vector\<int\>::iterator
    ```c++
    // 比如自己写一个iterator类，但注意这里必须要传一个容器节点Node而不是直接传一个容器类
    template <typename T>
	class ListIter
	{
	public:
		using value_type = T;
		using reference = T & ;
		using const_referenct = const T&;
		using pointer = T * ;
		using const_pointor = const T*;
		using size_type = size_t;
		using difference_type = ptrdiff_t;

		ListIter(pointer p = nullptr) : Iter(p) {}

		bool operator==(const ListIter& rhs) const noexcept
		{
			return Iter == rhs.Iter;
		}
		bool operator!=(const ListIter& rhs) const noexcept
		{
			return Iter != rhs.Iter;
		}
		ListIter& operator++()
		{
			Iter = Iter->next();
			return *this;
		}
		ListIter& operator++(int)
		{
			value_type tmp = *this;
			++&*this;
			return tmp;
		}
		reference operator*()
		{
			return *Iter;
		}
		pointer operator->()
		{
			return Iter;
		}
	private:
		pointer Iter;
	};
    ```

- **rb-tree基本认识**

	![rb_tree](https://rogerlv51.blob.core.windows.net/images/rb_tree.png)
    ![rb_class](https://rogerlv51.blob.core.windows.net/images/rb_tree_class.png)

	rb-tree是map和set的底层数据结构，也就是说map和set是有序的，因为rb-tree本身就是一个平衡二叉搜索树：有序的。而我们经常用的unordered_map和unordered_set底层则采用哈希表实现。

    ```c++
    template<typename _Key, typename _Val, typename _KeyOfValue, typename _Compare, typename _Alloc = allocator<_Val> >
    class _Rb_tree   // 可以看到我们在使用时要求要给4个模板参数
    // key: 键值类型, value: 值类型, keyOfValue: 怎么从value里面取key, compare: 比较器是一个函数比大小（我们可以在自己定义的类里面自定义这个函数）

    std::_Rb_tree<int, int, std::_Identity<int>, std::less<int> > tree;
    ```

	- **基础节点定义**

    <font color='yellow'>由于红黑树的handle-body设计结构的特殊性，实际的接口定义实际上是在_Rb_tree_impl中实现的！！！</font>

	```c++
	struct _Rb_tree_node_base
	{
		typedef _Rb_tree_node_base* _Base_ptr;
		typedef const _Rb_tree_node_base* _Const_Base_ptr;

		_Rb_tree_color	_M_color;    // 两种颜色，不是红就是黑；根节点为黑色，如果节点为红则其子节点必须为黑
		// 任一节点至NULL（树尾端）的任何路径，所含之黑节点数必须相同。
		_Base_ptr		_M_parent;  // 指向父节点
		_Base_ptr		_M_left;    // 指向左孩子
		_Base_ptr		_M_right;   // 指向右孩子
	}
	```

	- **关于迭代器**

	```c++
	template<typename _Tp>
    struct _Rb_tree_iterator
    {
		typedef _Tp  value_type;
		typedef _Tp& reference;
		typedef _Tp* pointer;

		typedef bidirectional_iterator_tag iterator_category;
		typedef ptrdiff_t		  difference_type;

		typedef _Rb_tree_iterator<_Tp>	_Self;
		typedef _Rb_tree_node_base::_Base_ptr _Base_ptr;
		typedef _Rb_tree_node<_Tp>*	   _Link_type;

		_Rb_tree_iterator() _GLIBCXX_NOEXCEPT
		: _M_node() { }

		explicit
		_Rb_tree_iterator(_Base_ptr __x) _GLIBCXX_NOEXCEPT
		: _M_node(__x) { }
		// 迭代器取值
		reference operator*() const _GLIBCXX_NOEXCEPT
		{ return *static_cast<_Link_type>(_M_node)->_M_valptr(); }
		// 指针操作
		pointer operator->() const _GLIBCXX_NOEXCEPT
		{ return static_cast<_Link_type> (_M_node)->_M_valptr(); }
		// 前++操作
		_Self& operator++() _GLIBCXX_NOEXCEPT
		{
		_M_node = _Rb_tree_increment(_M_node);
		return *this;
		}
		// 后++操作
		_Self operator++(int) _GLIBCXX_NOEXCEPT
		{
		_Self __tmp = *this;
		_M_node = _Rb_tree_increment(_M_node);
		return __tmp;
		}
		// 前--操作，这是因为rb-tree是双层节点结构因此也要采用双层迭代器
		_Self& operator--() _GLIBCXX_NOEXCEPT
		{
		_M_node = _Rb_tree_decrement(_M_node);
		return *this;
		}
		// 后--操作
		_Self operator--(int) _GLIBCXX_NOEXCEPT
		{
		_Self __tmp = *this;
		_M_node = _Rb_tree_decrement(_M_node);
		return __tmp;
		}
		// 比较是否相等
		bool operator==(const _Self& __x) const _GLIBCXX_NOEXCEPT
		{ return _M_node == __x._M_node; }

		bool operator!=(const _Self& __x) const _GLIBCXX_NOEXCEPT
		{ return _M_node != __x._M_node; }

		_Base_ptr _M_node;
    };
	```

- **红黑树的插入过程**

    **具体源码见链接：** https://github.com/gcc-mirror/gcc/blob/master/libstdc++-v3/src/c++98/tree.cc

    ```c++
    // 左旋转

    //    _x                      _y
    //  /   \     左旋转         /  \
    // T1   _y   --------->   _x    T3
    //     / \              /   \
    //    T2 T3            T1   T2
    static void local_Rb_tree_rotate_left(_Rb_tree_node_base* const __x, _Rb_tree_node_base*& __root)
    {
        // 保存x的右节点，变为x的父节点
        _Rb_tree_node_base* const __y = __x->_M_right;
        // x的右节点指向y的左节点，即T2变成x的右节点
        __x->_M_right = __y->_M_left;
        if (__y->_M_left !=0)
        __y->_M_left->_M_parent = __x;   //y的左节点的父节点变为x
        __y->_M_parent = __x->_M_parent; //y的父节点变为x的父节点

        if (__x == __root)
        __root = __y;   //如果原来x是根节点，那么现在根节点即变成了y
        else if (__x == __x->_M_parent->_M_left)   //  原_x的父节点的左孩子或右孩子连接新节点_y
        __x->_M_parent->_M_left = __y;
        else
        __x->_M_parent->_M_right = __y;
        __y->_M_left = __x;
        __x->_M_parent = __y;
    }

    // 右旋同理，不再赘述
    //        _x                      _y
    //      /   \     右旋转         /  \
    //     _y    T2 ------------->  T0  _x
    //    /  \                         /  \
    //   T0  T1                       T1  T2

    static void local_Rb_tree_rotate_right(_Rb_tree_node_base* const __x, _Rb_tree_node_base*& __root)
    {
        _Rb_tree_node_base* const __y = __x->_M_left;

        __x->_M_left = __y->_M_right;
        if (__y->_M_right != 0)
        __y->_M_right->_M_parent = __x;
        __y->_M_parent = __x->_M_parent;

        if (__x == __root)  
        __root = __y;
        else if (__x == __x->_M_parent->_M_right)
        __x->_M_parent->_M_right = __y;
        else
        __x->_M_parent->_M_left = __y;
        __y->_M_right = __x;
        __x->_M_parent = __y;
    }
    ```

    **完整的插入和平衡代码注释如下：**

    ```c++
    void _Rb_tree_insert_and_rebalance(const bool __insert_left,
                                _Rb_tree_node_base *__x,
                                _Rb_tree_node_base *__p,
                                _Rb_tree_node_base &__header) throw() 
    {
        _Rb_tree_node_base * &__root = __header._M_parent;

        // Initialize fields in new node to insert.
        __x->_M_parent = __p;
        __x->_M_left = 0;
        __x->_M_right = 0;
        __x->_M_color = _S_red;

        // 处理__header部分
        // Insert.
        // Make new node child of parent and maintain root, leftmost and
        // rightmost nodes.
        // N.B. First node is always inserted left.
        if (__insert_left) {
            __p->_M_left = __x; // also makes leftmost = __x when __p == &__header

            if (__p == &__header) {
                __header._M_parent = __x;
                __header._M_right = __x;
            } else if (__p == __header._M_left)
                __header._M_left = __x; // maintain leftmost pointing to min node
        } else {
            __p->_M_right = __x;

            if (__p == __header._M_right)
                __header._M_right = __x; // maintain rightmost pointing to max node
        }

    // Rebalance.
        while (__x != __root
            && __x->_M_parent->_M_color == _S_red)   // 若新插入节点不是为RB-Tree的根节点，且其父节点color属性也是红色,即违反了性质4.
        {
            _Rb_tree_node_base *const __xpp = __x->_M_parent->_M_parent;        // 祖父节点

            if (__x->_M_parent == __xpp->_M_left)   // 父亲是祖父节点的左孩子
            {
                _Rb_tree_node_base *const __y = __xpp->_M_right;    // 得到叔叔节点
                if (__y && __y->_M_color == _S_red)     // case1: 叔叔节点存在，且为红色
                {
                    /**
                    * 解决办法是：颜色翻转，父亲与叔叔的颜色都变为黑色,祖父节点变为红色,然后当前节点设为祖父，依次网上来判断是否破坏了红黑树性质
                    */
                    __x->_M_parent->_M_color = _S_black;    // 将其父节点改为黑色
                    __y->_M_color = _S_black;               // 将其叔叔节点改为黑色
                    __xpp->_M_color = _S_red;               // 将其祖父节点改为红色
                    __x = __xpp;                            // 修改_x,往上回溯
                } else {        // 无叔叔或者叔叔为黑色
                    if (__x == __x->_M_parent->_M_right) {          // 当前节点为父亲节点的右孩子
                        __x = __x->_M_parent;
                        local_Rb_tree_rotate_left(__x, __root);     // 以父节点进行左旋转
                    }
                    // 旋转之后,节点x变成其父节点的左孩子
                    __x->_M_parent->_M_color = _S_black;            // 将其父亲节点改为黑色
                    __xpp->_M_color = _S_red;                       // 将其祖父节点改为红色
                    local_Rb_tree_rotate_right(__xpp, __root);      // 以祖父节点右旋转
                }
            } else {        // 父亲是祖父节点的右孩子
                _Rb_tree_node_base *const __y = __xpp->_M_left; // 保存叔叔节点
                if (__y && __y->_M_color == _S_red) {       // 叔叔节点存在且为红色
                    __x->_M_parent->_M_color = _S_black;    // 父亲节点改为黑色
                    __y->_M_color = _S_black;               // 祖父节点改为红色
                    __xpp->_M_color = _S_red;
                    __x = __xpp;
                } else {        // 若无叔叔节点或者其叔叔节点为黑色
                    if (__x == __x->_M_parent->_M_left) {   // 当前节点为父亲节点的左孩子
                        __x = __x->_M_parent;
                        local_Rb_tree_rotate_right(__x, __root);    // 以父节点右旋转
                    }
                    __x->_M_parent->_M_color = _S_black;        // 父节点置为黑色
                    __xpp->_M_color = _S_red;                   // 祖父节点置为红色
                    local_Rb_tree_rotate_left(__xpp, __root);   // 左旋转
                }
            }
        }
        //若新插入节点为根节点,则违反性质2
        //只需将其重新赋值为黑色即可
        __root->_M_color = _S_black;
    }
    ```

**由于我们不会单独使用rb-tree这种数据结构，通常是在map和set中使用，所以这里不过多赘述其接口函数**