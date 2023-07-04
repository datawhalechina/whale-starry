# This is to learn stl-map.
1. 与红黑树(rb-tree)一样, map也是一种关联性容器(associative container), 它的底层是使用红黑树实现的, multimap同理
2. map的特性是, 所有元素都会根据**元素的键值**自动被排序. map中的所有元素都是pair, 含有两项,  *key*和*value*. 其中, pair的第一元素被认为是键值*key*, 第二元素被认为是实值*value*. 注意！ map不允许两个元素有**相同**的键值: 即不能出现 ```{key: value1, key: value2}```（用python-style为例）
3. 因此, 去定义map，首先要先定义pair, 在stl_map中, 没有pair的定义, pair的定义单独在stl_pair.h内部， pair的定义如下:
```
template<class _T1, class _T2>
    struct pair
    {
      typedef _T1 first_type;    /// @c first_type is the first bound type
      typedef _T2 second_type;   /// @c second_type is the second bound type

      _T1 first;                 /// @c first is a copy of the first object
      _T2 second;                /// @c second is a copy of the second object

      // _GLIBCXX_RESOLVE_LIB_DEFECTS
      // 265.  std::pair::pair() effects overly restrictive
      /** The default constructor creates @c first and @c second using their
       *  respective default constructors.  */
      _GLIBCXX_CONSTEXPR pair()
      : first(), second() { }

      /** Two objects may be passed to a @c pair constructor to be copied.  */
      _GLIBCXX_CONSTEXPR pair(const _T1& __a, const _T2& __b)
      : first(__a), second(__b) { }

      ...
      // 之后定义了一些pair的运算符重载, 如
      };
```

4. 注意: 我们不可以通过map的迭代器修改元素的**key**. map元素的键值关系到了map元素的排序规则(因为底层是红黑树), 任意改变元素**key**会严重破坏map组织, 但是如果想要修正元素的**value**, 是可以的, 因为map元素的**value**并不会影响map元素的排列规则.
因此, map的迭代器**iterator**不是const的, 也不是mutable
5. map和list相同的特征是: 当我们对它的元素做新增(**insert**)或者删除(**delete**)操作时, 该值之前所有的迭代器在操作完成仍然有效
6. map的底层机制是红黑树(rb-tree), 因为rb-tree是一种平衡二叉搜索树, 可以进行自动排序和调整, 同时,红黑树也包含了几乎所有的map操作
7. map的源码实现如下:
```
  template <typename _Key, typename _Tp, typename _Compare = std::less<_Key>,
            typename _Alloc = std::allocator<std::pair<const _Key, _Tp> > >
    class map
    {
    public:
      typedef _Key                                          key_type;
      typedef _Tp                                           mapped_type;
      typedef std::pair<const _Key, _Tp>                    value_type;
      typedef _Compare                                      key_compare;
      typedef _Alloc                                        allocator_type;

    private:
      // concept requirements
      typedef typename _Alloc::value_type                   _Alloc_value_type;
      __glibcxx_class_requires(_Tp, _SGIAssignableConcept)
      __glibcxx_class_requires4(_Compare, bool, _Key, _Key,
				_BinaryFunctionConcept)
      __glibcxx_class_requires2(value_type, _Alloc_value_type, _SameTypeConcept)

    public:
    // 定义了一个类作为成员变量, 是为了更直观的调用"元素比较函数"
      class value_compare
      : public std::binary_function<value_type, value_type, bool>
      {
	friend class map<_Key, _Tp, _Compare, _Alloc>;
      protected:
	_Compare comp;

	value_compare(_Compare __c)
	: comp(__c) { }

      public:
	bool operator()(const value_type& __x, const value_type& __y) const
	{ return comp(__x.first, __y.first); }
      }; // 这里是 class value_compare的收尾

    private:
      /// This turns a red-black tree into a [multi]map. 
      typedef typename __gnu_cxx::__alloc_traits<_Alloc>::template
	rebind<value_type>::other _Pair_alloc_type;

    // 可以很直观的看到 map的底层实现都是红黑树
      typedef _Rb_tree<key_type, value_type, _Select1st<value_type>,
		       key_compare, _Pair_alloc_type> _Rep_type;

      /// The actual tree structure.
      _Rep_type _M_t;

      typedef __gnu_cxx::__alloc_traits<_Pair_alloc_type> _Alloc_traits;

    public:
      // many of these are specified differently in ISO, but the following are
      // "functionally equivalent"
      // allocator是老朋友, 用来分配内存空间的
      typedef typename _Alloc_traits::pointer            pointer;
      typedef typename _Alloc_traits::const_pointer      const_pointer;
      typedef typename _Alloc_traits::reference          reference;
      typedef typename _Alloc_traits::const_reference    const_reference;
      // 可以看到
      typedef typename _Rep_type::iterator               iterator;
      typedef typename _Rep_type::const_iterator         const_iterator;
      typedef typename _Rep_type::size_type              size_type;
      typedef typename _Rep_type::difference_type        difference_type;
      typedef typename _Rep_type::reverse_iterator       reverse_iterator;
      typedef typename _Rep_type::const_reverse_iterator const_reverse_iterator;
```
8. map内部的一些操作:
```
map()
      : _M_t() { }

      /**
       *  @brief  Creates a %map with no elements.
       *  @param  __comp  A comparison object.
       *  @param  __a  An allocator object.
       */
       // 创建一个空的没有元素的map
      explicit
      map(const _Compare& __comp,
	  const allocator_type& __a = allocator_type())
      : _M_t(__comp, _Pair_alloc_type(__a)) { }

      /**
       *  @brief  %Map copy constructor.
       *  @param  __x  A %map of identical element and allocator types.
       *
       *  The newly-created %map uses a copy of the allocation object
       *  used by @a __x.
       */
       // map的拷贝构造函数
      map(const map& __x)
      : _M_t(__x._M_t) { }

#if __cplusplus >= 201103L
      /**
       *  @brief  %Map move constructor.
       *  @param  __x  A %map of identical element and allocator types.
       *
       *  The newly-created %map contains the exact contents of @a __x.
       *  The contents of @a __x are a valid, but unspecified %map.
       */
       // map的移动构造函数
      map(map&& __x)
      noexcept(is_nothrow_copy_constructible<_Compare>::value)
      : _M_t(std::move(__x._M_t)) { }

      /**
       *  @brief  Builds a %map from an initializer_list.
       *  @param  __l  An initializer_list.
       *  @param  __comp  A comparison object.
       *  @param  __a  An allocator object.
       *
       *  Create a %map consisting of copies of the elements in the
       *  initializer_list @a __l.
       *  This is linear in N if the range is already sorted, and NlogN
       *  otherwise (where N is @a __l.size()).
       */
      // 从initializer_list中创建一个map
      map(initializer_list<value_type> __l,
	  const _Compare& __comp = _Compare(),
	  const allocator_type& __a = allocator_type())
      : _M_t(__comp, _Pair_alloc_type(__a))
      { _M_t._M_insert_unique(__l.begin(), __l.end()); }

      /// Allocator-extended default constructor.
      explicit
      map(const allocator_type& __a)
      : _M_t(_Compare(), _Pair_alloc_type(__a)) { }

      /// Allocator-extended copy constructor.
      map(const map& __m, const allocator_type& __a)
      : _M_t(__m._M_t, _Pair_alloc_type(__a)) { }

      /// Allocator-extended move constructor.
      map(map&& __m, const allocator_type& __a)
      noexcept(is_nothrow_copy_constructible<_Compare>::value
	       && _Alloc_traits::_S_always_equal())
      : _M_t(std::move(__m._M_t), _Pair_alloc_type(__a)) { }

      /// Allocator-extended initialier-list constructor.
      map(initializer_list<value_type> __l, const allocator_type& __a)
      : _M_t(_Compare(), _Pair_alloc_type(__a))
      { _M_t._M_insert_unique(__l.begin(), __l.end()); }

      /// Allocator-extended range constructor.
      template<typename _InputIterator>
        map(_InputIterator __first, _InputIterator __last,
	    const allocator_type& __a)
	: _M_t(_Compare(), _Pair_alloc_type(__a))
        { _M_t._M_insert_unique(__first, __last); }
#endif

      /**
       *  @brief  Builds a %map from a range.
       *  @param  __first  An input iterator.
       *  @param  __last  An input iterator.
       *
       *  Create a %map consisting of copies of the elements from
       *  [__first,__last).  This is linear in N if the range is
       *  already sorted, and NlogN otherwise (where N is
       *  distance(__first,__last)).
       */
      template<typename _InputIterator>
        map(_InputIterator __first, _InputIterator __last)
	: _M_t()
        { _M_t._M_insert_unique(__first, __last); }
```

9. map的其他底层函数都是基于红黑树实现的, 所以很多成员函数都是调用红黑树的结构, 其代码示例如下(源码太过抽象所以简单使用省略后的代码进行说明)

```
// map使用的是底层rb-tree的insert_unique() 而不是insert_equal()
// 因为map 不允许相同键值的存在
// 前面的构造函数加进来方便说明
map() : t(Compare()) {}
// 初始化列表构造
explicit map(const Compare& comp) : t(comp) {}
template<typename _InputIterator>
map(_InputIterator __first, _InputIterator __last,
	const _Compare& __comp, const allocator_type& __a = allocator_type())
	: _M_t(__comp, _Pair_alloc_type(__a))
	// 注意此处使用的是_M_insert_unique();
        { _M_t._M_insert_unique(__first, __last); }
map(const map<_Key, _T, _Compare, _Alloc>& x) : t(x.t) {}
map<Key, T, Compare, Alloc>& operator=(const map<_Key, _T, _Compare, _Alloc>& x)
{t = x.t; return *this;}

// 下面的全部map操作, 底层都是使用rb-tree所提供的操作
// 几个迭代器 完全一致
key_compare key_comp() const { return t.key_comp();}
value_compare value_comp() const { return value_compare(t.key_comp());}
iterator begin() { return t.begin(); }
const_iterator begin() const { return t.begin(); } // const_iterator和普通iterator区别
iterator end() { return t.end(); }
const_iterator end() const { return t.end(); }
reverse_iterator rbegin() { return t.rbegin(); }
const_reverse_iterator rbegin() const { return t.rbegin(); }
reverse_iterator rend() { return t.rend(); }
const_reverse_iterator rend() const { return t.rend(); }
bool empty() const { return t.empty(); }
size_type size() const { return t.size(); }
size_type max_size() const { return t.max_size(); }

```

10. map中的at和[]: 之前在vector中同样有at和[]的成员函数和重载运算符, 但是在vector之中, 因为vector的at增加了索引检查，这使得at的访问操作比起[]来会更加安全, 在map中, at操作和[]操作也有着像vector中一样的区别, 源码可见下文操作

```
// map的[] operator
// 可以看到, []operator并没有做边界异常的检查
// 不需要的复杂定义已经删除
mapped_type&
operator[](key_type&& __k)
{
	iterator __i = lower_bound(__k);
	// __i->first is greater than or equivalent to __k.
	if (__i == end() || key_comp()(__k, (*__i).first))
	  __i = _M_t._M_emplace_hint_unique(__i, std::piecewise_construct,
					std::forward_as_tuple(std::move(__k)),
					std::tuple<>());
	return (*__i).second;
}
// map的at操作, 对两端边界进行了异常检查
// 通过__throw_out_of_range来实现, 提示下标越界
at(const key_type& __k)
{
	iterator __i = lower_bound(__k);
	if (__i == end() || key_comp()(__k, (*__i).first))
	  __throw_out_of_range(__N("map::at"));
	return (*__i).second;
}

```

