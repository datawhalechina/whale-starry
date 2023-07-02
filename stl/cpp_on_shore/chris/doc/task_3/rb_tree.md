# This is to learn RB-Tree.

1. 红黑树类是为使用STL中关联式容器(associative container)而设计的，其中底层用到红黑树的包括set, multiset, map, multimap.

2. 什么是关联式容器？ 类似于关联式数据库：每个元素都有一个**键值(key)**和一个对应的**实值（value）**, 当元素被插入到关联式容器时，容器内部结构便按照其**键值大小**，以某种特定的规则蒋这个元素放置于适当位置。关联式容器没有所谓的**头尾**。

3. 红黑树设计的理念：

   ```
   1. the header cell is maintained with links not only to the root
      but also to the "leftmost node" of the tree, to enable "constant
      time begin()"", and to the "rightmost node" of the tree, to enable
      "linear time performance" when used with the generic set algorithms
   2. when a node being deleted has two children, its successor node
      is relinked into its place, rather than copied, so that the only
      iterators invalidated are those referring to the deleted node.
   ```

4. 复习树中的基础知识：

   ```
   二叉搜索树(binary search tree) 可以提供对数时间的元素插入和访问。其节点放置规则是:任何节点的键值一定"大于"其"左子树"中的"每一个节点"的键值，并"小于"其"右子树"中"每一个节点"的键值。
   平衡二叉搜索树(balanced binary search tree) "平衡"的大致含义是: 没有一个节点过深
   AVL树(Adelson-Velskii-Landis tree) 是加上了"额外平衡条件"的二叉搜索树。其平衡的建立是为了确保整棵树的深度为O(logN)，AVL树的要求，任何节点的左右子树高度相差最多为1
   红黑树(Red Black tree) 在二叉搜索树的基础上，还必须满足以下规则
   1. 每个节点不是黑色就是红色  stl中的定义为
   enum _Rb_tree_color { _S_red = false, _S_black = true};
   2. 根节点为黑色
   3. 如果某节点为红色，那么其子节点必须为黑色
   4. 任一节点至NULL(树尾端)的任何路径，所含之黑节点数必须相同
   ```

   

5. 红黑树的定义如下

   可以看到找最大或者最小值很简单，因为红黑树必须满足二叉搜索树这个前提

   ```
    struct _Rb_tree_node_base
     {
       typedef _Rb_tree_node_base* _Base_ptr;
       typedef const _Rb_tree_node_base* _Const_Base_ptr;
   
       _Rb_tree_color	_M_color; // _S_red, _S_black
       _Base_ptr		_M_parent; // 父节点
       _Base_ptr		_M_left;
       _Base_ptr		_M_right;
       ...
       static _Base_ptr
       _S_minimum(_Base_ptr __x) _GLIBCXX_NOEXCEPT
       {
         // 二叉搜索树 往左走到头就是最小值
         while (__x->_M_left != 0) __x = __x->_M_left;
         return __x;
       }
       
       static _Base_ptr
       _S_maximum(_Base_ptr __x) _GLIBCXX_NOEXCEPT
       {
         // 二叉搜索树 往右走到头就是最大值
         while (__x->_M_right != 0) __x = __x->_M_right;
         return __x;
       }
       ...
   };
   ```

   ```
   // 定义真实节点(含值和节点的指针)
   template<typename _Val>
       struct _Rb_tree_node : public _Rb_tree_node_base
       {
         typedef _Rb_tree_node<_Val>* _Link_type;
         
         _Val _M_value_field;
         _Val* _M_valptr()
         { return std::__addressof(_M_value_field); } // 寻址指针
         ...
        };
   ```

   

6. RB-Tree的迭代器很重要，因为通过迭代器我们可以对RB-Tree的节点进行遍历访问。创建迭代器需要考虑迭代器的类别、前进、后退、解引用、成员访问等操作如何设计，源码如下

   ```
    template<typename _Tp>
       struct _Rb_tree_iterator
       {
         typedef _Tp  value_type;
         typedef _Tp& reference;
         typedef _Tp* pointer;
   
         typedef bidirectional_iterator_tag iterator_category;
         typedef ptrdiff_t			 difference_type;
   
         typedef _Rb_tree_iterator<_Tp>		_Self;
         typedef _Rb_tree_node_base::_Base_ptr	_Base_ptr;
         typedef _Rb_tree_node<_Tp>*		_Link_type;
         ...
         // 解引用获取值
         reference
         operator*() const _GLIBCXX_NOEXCEPT
         { return *static_cast<_Link_type>(_M_node)->_M_valptr(); }
         
         // 获取当前值指向的地址
         pointer
         operator->() const _GLIBCXX_NOEXCEPT
         { return static_cast<_Link_type> (_M_node)->_M_valptr(); }
         
         // 前进
         _Self&
         operator++() _GLIBCXX_NOEXCEPT
         {
   		_M_node = _Rb_tree_increment(_M_node);
   		return *this;
         }
         
         // 
   ```

7.  前置++和后置++的区别(也包括了前置--和后置--)，此处的++和--都是重载为在红黑树中的移动具体代码实现如下：

   ```
    	  _Self&
         operator++() _GLIBCXX_NOEXCEPT
         {
   		_M_node = _Rb_tree_increment(_M_node);
   		return *this;
         }
   
    	  _Self
         operator++(int) _GLIBCXX_NOEXCEPT
         {
   		_Self __tmp = *this;
   		_M_node = _Rb_tree_increment(_M_node);
   		return __tmp;
         }
   
     	  _Self&
         operator--() _GLIBCXX_NOEXCEPT
         {
   		_M_node = _Rb_tree_decrement(_M_node);
   		return *this;
         }
   
     	  _Self
         operator--(int) _GLIBCXX_NOEXCEPT
         {
   		_Self __tmp = *this;
   		_M_node = _Rb_tree_decrement(_M_node);
   		return __tmp;
         }
   ```

   可以看到，对于需要在当前表达式改变值的运算符重载函数，都是使用了其引用类型，这也符合了effective C++的要求，对于值需要改变的表达式，则需要使用其引用

8. 对于```_Rb_tree_decrement```,``` _Rb_tree_increment```, 两个函数的实现，需要追溯到c++98协议中的stl_tree的实现，大概的实现过程如下：

   ```
   struct _rb_tree_base_iterator{
   	typedef __rb_tree_node_base::base_ptr base_ptr;
   	typedef bidirectional_iterator_tag iterator_category;
   	typedef ptrdiff_t difference_type;
   	
   	base_ptr node;
   	...
   };
   
   void increment() {
   	if (node->right != 0){ // 如果有右子节点。状况(1)
   		node = node->right; // 就往右走
   		while (node->left != 0){ // 然后就一直往左走到底
   			node = node->left;
   		}
   	}
   	else { // 没有右子节点。状况(2)
   		base_ptr y = node->parent; // 找出父节点
   		while (node == y->right) { // 如果现在的节点本身是右子节点
   			node = y; 
   			y = y->parent; // 往上走，直到"不是右子节点为止"
   		}
   		// 如果此时的右子节点不等于此时的父节点
   		// 则状况(3) 此时的父节点及为我们想要的
   		// 否则此时的node及为解答
   		if (node->right != y) node = y;
   	}
   }
   
   
   void decrement(){
   	if (node->color == _rb_tree_red && // 如果是红节点，且
   		node->parent->parent == node)  // 父节点的父节点等于自己,
   		node = node->right;            // 右子节点即为解答
   	// 以上情况发生于node为header时(或者为node为end()时)
   	// 注意, header之右子节点即rightmost，指向整棵树的max节点
   	else if (node->left != 0) {        // 若有左子节点。
   		base_ptr y = node->left;       // 令y指向左子节点
   		while (y->right != 0)          // 当y有右子节点时
   			y = y->right;			   // 一直往右子节点走到底
   		node = y; 					   // 即为我们需要找的节点
   	}
   	else {                             // 既非根节点，也无左子节点
   		base_ptr y = node->parent;     //  找出父节点
   		while (node == y->left) {      // 当现行节点身为左子节点
   			node = y;                  // 一直交替往上走, 直到现行节点
   			y = y->parent;             // 不为左子节点
   		}
   		node = y;                      // 此时之父节点即为所需要的节点
   	}
   }
   ```

9.  RB-tree 的数据结构，即RB-Tree真正的定义

   由于gcc的树结构太过庞大，这里只用一些示例代码来进行说明

   ```
   template <class Key, class Value, class KeyOfValue, class Compare, class Alloc = alloc>
   class rb_tree {
   	...
   	// 分配内存空间
   	link_type get_node() { return rb_tree_node_allocator::allocate(); }
   	// 释放内存空间
   	void put_node(link_type p) { r_tree_node_allocator::deallocate(p);}
   	// 创造新节点，需要分配内存空间
   	link_type create_node(const value_type& x) {
   		link_type tmp = get_node();
   		__STL_TRY {
   			construct(&tmp->value_field, x);
   		}
   		__STL_UNWIND(put_node(tmp));
   		return tmp;
   	}
   	// 复制一个节点, 包括值和颜色
   	link_type clone_node(link_type x) {
   		link_type tmp = create_node(x->value_field);
   		tmp->color = x->color;
   		tmp->left = 0;
   		tmp->right = 0;
   		return tmp;
   	}
   	void destroy_node(link_type p) {
   		destroy(&p->value_field); // 析构内容
   		put_node(p); // 释放内存
   	}
   };
   ```

10.  RB-Tree的构造和内存管理：一种是以现有的RB-Tree复制一个新的RB-Tree，另一种是产生一个空树```rb_tree<int, int, identity<int>, less<int> > itree;``` 此处制定了节点的键值、实值、大小比较标准，然后调用RB-Tree的默认构造函数

    ```rb_tree(const Compare& comp = Compare()): node_count(0), key_compare(comp) { init(); } ```

    其中，init函数的实现如下：

    ```
    void init() {
    	header = get_node(); // 生成一个节点空间，并令header指向它
    	color(header) = __rb_tree_red; // 令header为红色，用于区分header和root
    	
    	root() = 0;
    	leftmost() = header; // 令header的左子节点为自己
    	rightmost() = header; // 令header的右子节点为自己
    }
    ```

    对于树状结构的各种操作，最需要注意的就是边界情况的发生，也就是走到根节点时要有特殊的处理，为了简化处理，STL特别为根节点再设计一个父节点，名为header

11. 当插入新节点时，既需要按照RB-Tree的规则来调整，还需要检查header的正确性，使其父节点指向根节点，左子节点指向最小节点，右节点指向最大节点
RB-Tree的插入操作
**insert_unique** 表示插入节点的键值(Key)在整棵树中必须是唯一的，因此如果树中有相同的键值，插入操作就不会执行
**insert_equal**则表示插入节点的键值可以重复出现在这棵树中

```
// insert_equal
// 返回值是一个RB-Tree的iterator, 指向一个新节点
template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_equal(const Value& v){
   link_type y = header;
   link_type x = root(); // 从根节点开始
   while(x!=0) {
      y = x;
      // 遇见“大”则往左, 遇见小于或者等于往右
      x = key_compare(KeyOfValue()(v), key(x)) ? left(x): right(x);
   }
   // x是新值插入节点, y为插入点之父节点, v为新值
   return __insert(x, y, v);
}


// insert_unique
template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
pair<typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator, bool>
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_unique(const Value& v){
   link_type y = header;
   link_type x = root();
   bool comp = true;
   while(x != 0){
      y = x;
      comp = key_compare(KeyOfValue()(v), key(x)); //键值小于目前节点之键值
      x = comp?left(x): right(x); // 遇"大"则往左，遇小于或等于则往右
   }
   // 离开while循环之后，y所指即插入点之父节点
   iterator j = iterator(y); // 令迭代器j指向插入点之父节点y
   if(comp) //如果离开while循环时comp为真(则表示遇大，将插入左侧)
      if(j==begin())
         // x为插入点, y为插入点之父节点, v为新值
         return pair<iterator, bool>(__insert(x, y, v), true);
      else --j; // 否则调整j
   if(key_compare(key(j.node), KeyOfValue()(v)))
      //小于新值(表示遇小，将插入右侧)
      return pair<iterator, bool>(__insert(xx, y, v), true);
      // x为新值插入点, y为插入点之父节点, v为新值

   // 到此步骤，则表示新值一定与树中的键值重复，则不插入新值
   return pair<iterator, bool>(j, false);
}


//真正的插入执行程序 __insert()
template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator;
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::
__insert(base_ptr x_, base_ptr y_, const Value& v){
   // x_为新值插入点, 参数y_为插入点的父节点, 参数v为新值
   link_type x = (link_type) x_;
   link_type y = (link_type) y_;
   link_type z;

   // key_compare是键值大小比较的准则
   if(y == header || x!= 0 || key_compare(KeyOfValue()(v), key(y))){
      z = create_node(v); // 产生一个新节点
      left(y) = z; // 使得y为header时, leftmost() = z
      if(y == header) {
         root() = z;
         rightmost() = z;
      }
      else if (y == leftmost()) // 若y为最左节点
         leftmost() = z;
   }
   else {
      z = create_node(v); // 产生一个新节点
      right(y) = z; // 令新节点成为插入点之父节点y的右子节点
      if(y == rightmost())
         rightmost() = z; //维护rightmost(), 使他永远指向最右节点
   }
   parent(z) = y; // 设定新节点的父节点
   left(z) = 0; //设定新节点的左子节点
   right(z) = 0; // 设定新节点的右子节点
                 // 新节点的颜色将在__rb_tree_rebalance()设定
   __rb_tree_rebalance(z, header->parent); // 参数1为新增节点，参数2为root
   ++node_count; // 节点数累加
   return iterator(z); // 返回一个迭代器
}
```

12. RB-Tree的调整操作    

    在红黑树中，任何插入操作在节点插入完毕之后，都需要按照红黑树规则进行调整，使用```__rb_tree_rebalance()```函数来进行调整

    ```
    // 全局函数
    // 领树形重新平衡(改变颜色以及旋转机型)
    // 参数一为新增节点 参数二为root
    __rb_tree_rebalance(__rb_tree_node_base* x, __rb_tree_node_base*& root){
     x->color = __rb_tree_red; // 新节点一定是red节点
     while (x!=root && x->parent->color == __rb_tree_red) {
     	if (y && y->color == __rb_tree_red) { // 令y为伯父节点
     		x->parent->color == __rb_tree_black; // 伯父节点存在，且为红色
     		x->color = __rb_tree_black; // 把父节点改成黑色
     		x->parent->parent->color = __rb_tree_red; // 把祖父节点改成红色
     		x = x->parent->parent;
    	 }
     	else { // 无伯父节点或者伯父节点为黑色
     		if(x == x->parent->right) { // 如果新节点为父节点之右子节点
     			x = x->parent;
     			__rb_tree_rotate_left(x, root); // 第一参数为左旋点
     		}
     		x->parent->color = __rb_tree_black; // 改变颜色
     		x->parent->parent->color = __rb_tree_red;
     		__rb_tree_rotate_right(x->parent->parent, root); // 将第一参数改为右节点
     	}
     }
     else { // 父节点为祖父节点之右子节点
     	__rb_tree_node_base* y = x->parent->parent->left; // 令y为伯父节点
     	if (y && y->color == __rb_tree_red) { // 有伯父节点，且为红色
     		x->parent->color = __rb_tree_black; // 更改伯父节点为黑色
     		x->parent->parent->color = __rb_tree_red; // 更改祖父节点颜色为红色
     		x = x->parent->parent; // 继续往上层检查
     	}
     	else { // 无伯父节点或者伯父节点为黑色
     		if(x == x->parent->left) { // 如果新节点为父节点之左子节点
     			x = x->parent;
     			__rb_tree_rotate_right(x, root); // 第一参数为右节点
     		}
     		x->parent->color = __rb_tree_black; // 改变颜色
     		x->parent->parent->color = __rb_tree_red;
     		__rb_tree_rotate_left(x->parent->parent, root); // 第一参数为左旋点
     	
     	}
     } // while结束
     root->color = __rb_tree_black; // 根节点永远为黑
    }
    ```

13.  12中的左旋右旋操作，这一条是具体的实现

    ```
    // 全局函数
    // 新节点必为红节点, 如果插入处之父节点亦为红节点, 则违反红黑树规则
    // 做树形旋转(及颜色改变)
    inline void
    __rb_tree_rotate_left(__rb_tree_node_base* x, __rb_tree_node_base*& root){
    	// x为旋转点
    	__rb_tree_node_base* y = x->right; // 令y为旋转点的右子节点
    	x->right = y->left;
    	if (y->left != 0) y->left->parent = x; // 设置父节点
    	y->parent = x->parent;
    	// 令y完全顶替x的地位(必须将x对其父节点的关系完全接收过来)
    	if (x == root) root = y;
    	else if (x == x->parent->left) // x为其父节点的左子节点
    		x->parent->left = y;
    	else // x为其父节点的右子节点
    		x->parent->left = y;
    	y->left = x;
    	x->parent = y;
    }
    
    inline void
    __rb_tree_rotate_right(__rb_tree_node_base* x, __rb_tree_node_base*& root){
    	// x为旋转点
    	__rb_tree_node_base* y = x->left; // y为旋转点的左子节点
    	x->left = y->right;
    	if (y->right != 0)
    		y->right->parent = x; // 设置父节点
    	y->parent = x->parent;
    	
    	// 令y完全顶替x的地位
    	if (x == root) root = y;
    	else if (x == x->parent->right) // x为其父节点的右子节点
    		x->parent->right = y;
    	else 
    		x->parent->left = y;
    	x->right = x;
    	x->parent = y;
    
    ```

14.  二叉搜索树中的元素查找

    ```
    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::find(const Key& k) {
    	link_type y = header; // Last node which is not less than k.
    	link_type x = root(); // Current node
    	
    	while (x!=0)
    		// 以下, key_compare是节点键值大小比较准则
    		if (!key_compare(key(x), k))
    			{y = x, x = left(x);}
    		else x = right(x);
    	
    	iterator j = iterator(y);
    	return (j==end() || key_compare(k, key(j.node))) ? end() : j;
    }
    ```

    