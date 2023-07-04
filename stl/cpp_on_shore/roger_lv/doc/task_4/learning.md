## 刷题开发-map学习笔记

**源码地址为:** https://github.com/gcc-mirror/gcc/blob/releases/gcc-9/libstdc%2B%2B-v3/include/bits/stl_map.h

- **基本认识**
  
    可以看到map需要传4个模板参数，当然后两个可以不用传，自带默认参数。key表示键的类型，Tp表示值的类型即data，在map中是以键值对形式存储数据的。compare表示如何比较key的大小，alloc分配器可以看到默认key和data是以pair类型存储的。同时可以看到map实际上底层是调用了_Rb_tree因此来实现了key的大小排序，同时把compare和alloc传给了_Rb_tree。因此map的底层实现是红黑树。

    ```cpp
    template <typename _Key, typename _Tp, typename _Compare = std::less<_Key>,
	    typename _Alloc = std::allocator<std::pair<const _Key, _Tp> > >
    class map{
        public:
        typedef _Key					key_type;
        typedef _Tp					mapped_type;
        typedef std::pair<const _Key, _Tp>		value_type;
        typedef _Compare					key_compare;
        typedef _Alloc					allocator_type;
    }

    private:
      /// This turns a red-black tree into a [multi]map.
      typedef typename __gnu_cxx::__alloc_traits<_Alloc>::template rebind<value_type>::other _Pair_alloc_type;

      typedef _Rb_tree<key_type, value_type, _Select1st<value_type>,
		       key_compare, _Pair_alloc_type> _Rep_type;
    ```

    **需要注意的是map无论是无参构造还是拷贝构造，参数都是带const的，意味着我们不能修改map中对象的值！**
    ```cpp
    map(const _Compare& __comp,
	  const allocator_type& __a = allocator_type())
      : _M_t(__comp, _Pair_alloc_type(__a)) { }

      /**
       *  @brief  %Map copy constructor.
       *
       *  Whether the allocator is copied depends on the allocator traits.
       */
    map(const map& __x)
    ```

- **基本迭代器定义**

    注意map中之所以能调用first和second取key或者data的数据，实际上是因为pair这个模板类中定义了_T1 first和_T2 second这两个变量，表示前一个与后一个的值

    - **iterator begin():** Returns a read/write iterator that points to the first pair in the map
    - **iterator end():** Returns a read/write iterator that points one past the last pair in the map
    - **reverse_iterator rbegin():** Returns a read/write reverse iterator that points to the last pair in the map
    - **reverse_iterator rend():** Returns a read/write reverse iterator that points to one before the first pair in the map

- **基本函数接口**

    - **bool empty():** Returns true if the map is empty
    - **size_type size():** Returns the size of the map
    - **size_type max_size():** Returns the maximum size of the map
    - **mapped_type& operator[](const key_type& __k):** 重载[]，元素取值，根据key读取data，If the key does not exist, a pair with that key is created using default values, which is then returned
    - **mapped_type& at(const key_type& __k):** 功能和上述[]一致，唯一区别是加了越界检测
    - **std::pair<iterator, bool> emplace(_Args&&... __args):** Attempts to build and insert a std::pair into the map. 底层调用_M_emplace_unique函数，说明当key与输入参数一致则不允许插入
    - **node_type extract(const_iterator __pos):** extract a node，传入一个迭代器指针
    - **iterator insert(const_iterator __hint, node_type&& __nh):** Re-insert an extracted node
    - **void merge(map<_Key, _Tp, _C2, _Alloc>& __source):** 合并两个map，会自动去重
    - **std::pair<iterator, bool> insert(const value_type& __x)：** Attempts to insert a std::pair into the map. 也支持插入一串pairs
    - **pair<iterator, bool> insert_or_assign(const key_type& __k, _Obj&& __obj):** This function attempts to insert a (key, value) pair into the map. A map relies on unique keys and thus a pair is only inserted if its first element (the key) is not already present in the map. If the %pair was already in the map, the .second of the pair is assigned from __obj.
    - **iterator erase(const_iterator __position):** Erases an element from a map. 显然是直接把一个pair删掉
    - **void swap(map& __x):** Swaps data with another map.
    - **void clear():** Erases all elements from a map.
    - **iterator find(const key_type& __x):** This function takes a key and tries to locate the element with which the key matches. If successful the function returns an iterator pointing to the sought after pair
    - **size_type count(const key_type& __x):** Finds the number of elements with given key.在map中只会返回0或者1

- **基础运算符重载**

    ```cpp
        /**
     *  @brief  Map equality comparison.
    *  @param  __x  A %map.
    *  @param  __y  A %map of the same type as @a x.
    *  @return  True iff the size and elements of the maps are equal.
    *
    *  This is an equivalence relation.  It is linear in the size of the
    *  maps.  Maps are considered equivalent if their sizes are equal,
    *  and if corresponding elements compare equal.
    */
    template<typename _Key, typename _Tp, typename _Compare, typename _Alloc>
        inline bool
        operator==(const map<_Key, _Tp, _Compare, _Alloc>& __x,
            const map<_Key, _Tp, _Compare, _Alloc>& __y)
        { return __x._M_t == __y._M_t; }

    /**
     *  @brief  Map ordering relation.
    *  @param  __x  A %map.
    *  @param  __y  A %map of the same type as @a x.
    *  @return  True iff @a x is lexicographically less than @a y.
    *
    *  This is a total ordering relation.  It is linear in the size of the
    *  maps.  The elements must be comparable with @c <.
    *
    *  See std::lexicographical_compare() for how the determination is made.
    */
    template<typename _Key, typename _Tp, typename _Compare, typename _Alloc>
        inline bool
        operator<(const map<_Key, _Tp, _Compare, _Alloc>& __x,
            const map<_Key, _Tp, _Compare, _Alloc>& __y)
        { return __x._M_t < __y._M_t; }

    /// Based on operator==
    template<typename _Key, typename _Tp, typename _Compare, typename _Alloc>
        inline bool
        operator!=(const map<_Key, _Tp, _Compare, _Alloc>& __x,
            const map<_Key, _Tp, _Compare, _Alloc>& __y)
        { return !(__x == __y); }

    /// Based on operator<
    template<typename _Key, typename _Tp, typename _Compare, typename _Alloc>
        inline bool
        operator>(const map<_Key, _Tp, _Compare, _Alloc>& __x,
            const map<_Key, _Tp, _Compare, _Alloc>& __y)
        { return __y < __x; }

    /// Based on operator<
    template<typename _Key, typename _Tp, typename _Compare, typename _Alloc>
        inline bool
        operator<=(const map<_Key, _Tp, _Compare, _Alloc>& __x,
            const map<_Key, _Tp, _Compare, _Alloc>& __y)
        { return !(__y < __x); }

    /// Based on operator<
    template<typename _Key, typename _Tp, typename _Compare, typename _Alloc>
        inline bool
        operator>=(const map<_Key, _Tp, _Compare, _Alloc>& __x,
            const map<_Key, _Tp, _Compare, _Alloc>& __y)
        { return !(__x < __y); }
    ```