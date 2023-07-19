## 哈希表-HashTable学习笔记

- **首先我们需要了解什么是哈希表？**

    - 哈希表就是在关键字和存储位置之间建立对应关系，使得元素的查找可以以O(1)的效率进行， 其中关键字和存储位置之间是通过散列函数建立关系

    - 常见的散列函数：

        线性定址法：直接取关键字的某个线性函数作为存储地址，散列函数为：hash(key)=a×key+b

        除留余数法：将关键字对某一小于散列表长度的数p取余的结果作为存储地址，散列函数为：hash=key % p

        平方取中法：对关键字取平方，然后将得到结果的中间几位作为存储地址

        折叠法：将关键字分割为几部分，然后将这几部分的叠加和作为存储地址

    - 实际在创建哈希表的时候会出现不同关键字映射到同一个地址上，称为哈希冲突：

        线性探测法：当发生冲突时，就顺序查看下一个存储位置，如果位置为空闲状态，就将该关键字存储在该位置上，如果还是发生冲突，就依次往后查看，当查看到存储空间的末尾时还是找不到空位置，就返回从头开始查看

        拉链法：将具有相同存储地址的关键字链成一单链表， m个存储地址就设m个单链表，然后用一个数组将m个单链表的表头指针存储起来，形成一个动态的结构

- **关于哈希表的基本概念**

    - Each _Hashtable data structure has:

        ```c++
          _Bucket[]       _M_buckets    
          // 桶的概念就相当于hashtable存放元素的空间，可以理解为一个数组，每一个位置上实际上是存储一个链表，遵循散列表计算公式，
          // 当_M_element_count>_M_bucket_count时，则扩充bucket的容量并重新计算元素存放的地址
          _Hash_node_base _M_before_begin
          size_type       _M_bucket_count
          size_type       _M_element_count
        ```

    - with _Bucket being _Hash_node* and _Hash_node containing:

        ```c++
          _Hash_node*   _M_next
          Tp            _M_value
          size_t        _M_hash_code if cache_hash_code is true
        ```

    - 基本模板定义：

        ```c++
            template<typename _Key, typename _Value, typename _Alloc,
            typename _ExtractKey, typename _Equal,
            typename _H1, typename _H2, typename _Hash,
            typename _RehashPolicy, typename _Traits>
            class _Hashtable
            : 　public __detail::_Hashtable_base<_Key, _Value, _ExtractKey, _Equal,
                public __detail::_Map_base<_Key, _Value, _Alloc, _ExtractKey, _Equal,  _H1, _H2, _Hash, _RehashPolicy, _Traits>,
                public __detail::_Insert<_Key, _Value, _Alloc, _ExtractKey, _Equal, _H1, _H2, _Hash, _RehashPolicy, _Traits>,
                public __detail::_Rehash_base<_Key, _Value, _Alloc, _ExtractKey, _Equal,_H1, _H2, _Hash, _RehashPolicy, _Traits>,
                public __detail::_Equality<_Key, _Value, _Alloc, _ExtractKey, _Equal,_H1, _H2, _Hash, _RehashPolicy, _Traits>,
                private __detail::_Hashtable_alloc<typename __alloctr_rebind<_Alloc,__detail::_Hash_node<_Value,_Traits::__hash_cached::value> >::__type>
            {};
            // H1,H2,Hash实际上都是仿函数，用来规定hash函数的公式，ExtractKey函数规定怎么从key中取值
            // Equal函数规定如何判断值大小，RehashPolicy定义发生哈希冲突的法则，通常是把bucket扩大两倍重新计算hash
        ```
    
    - hashtable中链表的节点结构

        ```c++
        template<typename _Value>
        struct _Hash_node_value_base : _Hash_node_base
        {
          typedef _Value value_type;

          __gnu_cxx::__aligned_buffer<_Value> _M_storage;

          _Value*
          _M_valptr() noexcept
          { return _M_storage._M_ptr(); }

          const _Value*
          _M_valptr() const noexcept
          { return _M_storage._M_ptr(); }

          _Value&
          _M_v() noexcept
          { return *_M_valptr(); }

          const _Value&
          _M_v() const noexcept
          { return *_M_valptr(); }
        };
        ```
    
    - 关于迭代器

        ```c++
            template<typename _Value, bool __constant_iterators, bool __cache>
            struct _Node_iterator
            : public _Node_iterator_base<_Value, __cache>
            {
            private:
              using __base_type = _Node_iterator_base<_Value, __cache>;
              using __node_type = typename __base_type::__node_type;

            public:
              typedef _Value				value_type;
              typedef std::ptrdiff_t		difference_type;
              typedef std::forward_iterator_tag		iterator_category;

              using pointer = typename std::conditional<__constant_iterators, const _Value*, _Value*>::type;

              using reference = typename std::conditional<__constant_iterators, const _Value&, _Value&>::type;

              _Node_iterator() noexcept : __base_type(0) { }

              explicit _Node_iterator(__node_type* __p) noexcept
              : __base_type(__p) { }

              reference operator*() const noexcept
              { return this->_M_cur->_M_v(); }

              pointer operator->() const noexcept
              { return this->_M_cur->_M_valptr(); }

              _Node_iterator& operator++() noexcept
              {
                  this->_M_incr();
                  return *this;
              }

              _Node_iterator operator++(int) noexcept
              {
                  _Node_iterator __tmp(*this);
                  this->_M_incr();
                  return __tmp;
              }
            };
        ```

- **HashTable的一些重要函数**

    - begin函数

        ```c++
        // 调用_M_begin: 可以把_M_before_begin想象成一个head节点，第一个节点就是下一个节点。
        iterator begin() noexcept
        { return iterator(_M_begin()); }

        __node_type* _M_begin() const
        { return static_cast<__node_type*>(_M_before_begin._M_nxt); }
        ```
    
    - end函数

        ```c++
        iterator end() noexcept
        { return iterator(nullptr); }
        ```

    - size与empty

        ```c++
        size_type size() const noexcept
        { return _M_element_count; }

        bool empty() const noexcept
        { return size() == 0; }
        ```
    
    - 返回桶数量

        ```c++
        size_type bucket_count() const noexcept
        { return _M_bucket_count; }
        ```

    - 计算加载因子

        ```c++
        float load_factor() const noexcept
        {
          return static_cast<float>(size()) / static_cast<float>(bucket_count());
        }
        ```

    - 桶的index计算

        ```c++
        // 实际上底层源码就是用的除数留余法
        size_type bucket(const key_type& __k) const
        { return _M_bucket_index(__k, this->_M_hash_code(__k)); }
        ```