# stl_hashtable源码阅读

## hashtable概述

STL中的`hashtable`是一个用于实现哈希表的数据结构，`unordered_map` 和 `unordered_set` 就是基于hashtable来实现的。哈希表的基本思想是利用`key`快速查找到对应的`value`。

时间复杂度：

查找：`O(1)`

删除：`O(1)`

插入：`O(1)`

删除：`O(1)`

hashtable的适用场景就是需要快速查找和插入的情况。以统计某个数组中的某个数组出现次数为例，那最简单的实现快速查找和插入的方法就是将所有可能出现的数字都枚举一遍，`所有可能出现的数字`以32位机器为例，就需要保存`2^32`个索引，这是不可接受的，因此这是哈希表需要解决的第一个问题：如何减小索引的大小，将一个元素映射到一个“大小可以接受的索引范围内”。第二个问题则是对于其他非整数类型的key，如何映射到索引范围？

## hash_function

解决上述两个问题的方法就是hash_function。那既然“没有给每一个元素”都提供了索引，那么就会有“哈希冲突”的风险。

解决哈希冲突的方法有：

1. 线性探测：如果对应索引的位置被占了，则会往下继续寻找，直到找到空位置。缺点是效率低。
2. 二次探测：思路和1相似，但是就是往下寻找的步长是符合 $i^2$ 的二次函数。
3. 开链：每一个索引位置都维护了一个链表，用来存放hash_function映射为相同索引的元素。



## hashtable中的开链结构

hashtable中维护了 桶（buckets） 和 节点（nodes）。

buckets是用vector实现的；每个bucket的各自的链表是自己维护的单向链表。



## hashtable如何根据key计算到桶的索引？

源码在

```cpp
// hashtable.h
size_type
bucket(const key_type& __k) const
{ return _M_bucket_index(__k, this->_M_hash_code(__k)); }
// 先计算this->_M_hash_code(__k)

// hashtable_policy.h
__hash_code
_M_hash_code(const _Key& __k) const
{
  static_assert(__is_invocable<const _H1&, const _Key&>{},
                  "hash function must be invocable with an argument of key type");
  return _M_h1()(__k);
}
// _M_h1 在unordered_map中默认是 std::hash<key>
// 接着寻找_M_bucket_index

// hashtable.h
size_type
_M_bucket_index(const key_type& __k, __hash_code __c) const
{ return __hash_code_base::_M_bucket_index(__k, __c, _M_bucket_count); }
// hashtable_policy.h
std::size_t
_M_bucket_index(const _Key&, __hash_code __c, std::size_t __n) const
{ return _M_h2()(__c, __n); }
// _M_h2 是unordered_map中默认是 _Mod_range_hashing，就是取余操作
```

计算流程是：

1. 首先计算key的哈希值`Hashcode`，
2. 然后将`Hashcode`与桶的数量取余，
3. 得到最终的桶的索引



## hashtable如何实现根据key查找节点？

```cpp
// hashtable.h
template<typename _Key, typename _Value,
		typename _Alloc, typename _ExtractKey, typename _Equal,
		typename _H1, typename _H2, typename _Hash, typename _RehashPolicy,
		typename _Traits>
auto
_Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal,
_H1, _H2, _Hash, _RehashPolicy, _Traits>::
find(const key_type& __k)
    -> iterator
{
    __hash_code __code = this->_M_hash_code(__k);
    std::size_t __n = _M_bucket_index(__k, __code);
    __node_type* __p = _M_find_node(__n, __k, __code);
    return __p ? iterator(__p) : end();
}

// hashtable.h
__node_type*
_M_find_node(size_type __bkt, const key_type& __key, __hash_code __c) const
{
    __node_base* __before_n = _M_find_before_node(__bkt, __key, __c);
    if (__before_n)
        return static_cast<__node_type*>(__before_n->_M_nxt);
    return nullptr;
}
// Find the node whose key compares equal to k in the bucket n.
// Return nullptr if no node is found.
template<typename _Key, typename _Value,
typename _Alloc, typename _ExtractKey, typename _Equal,
typename _H1, typename _H2, typename _Hash, typename _RehashPolicy,
typename _Traits>
auto
_Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal,
_H1, _H2, _Hash, _RehashPolicy, _Traits>::
_M_find_before_node(size_type __n, const key_type& __k,
                    __hash_code __code) const
    -> __node_base*
{
    __node_base* __prev_p = _M_buckets[__n];
    if (!__prev_p)
        return nullptr;

    for (__node_type* __p = static_cast<__node_type*>(__prev_p->_M_nxt);;
         __p = __p->_M_next())
    {
        if (this->_M_equals(__k, __code, __p))
            return __prev_p;

        if (!__p->_M_nxt || _M_bucket_index(__p->_M_next()) != __n)
            break;
        __prev_p = __p;
    }
    return nullptr;
}
```

步骤：

1. 首先计算key的哈希值
2. 根据哈希值找到桶的索引
3. 遍历桶内的链表，找到`_M_equals(__k, __code, __p)`

==疑问==：放在同一个桶里的都是key的哈希值是一样的，既然索引得到的hashcode是一样的，在遍历结点的时候，如何判断这个结点是key对应的结点的呢？讲道理应该需要在`_Hash_node`中保存原始的key用来判断，但是`_Hash_node`中并没有原始的key。



## hashtable如何实现插入？

`hashtable`实现插入操作时，会经过以下步骤：

1. 计算哈希值：首先，使用哈希函数计算要插入元素的关键字的哈希值。哈希函数将关键字转换为一个整数，该整数将确定元素应该插入到哈希表的哪个桶中。
2. 查找桶：根据计算得到的哈希值，找到对应的桶。
3. 冲突处理：如果插入的哈希值与已有元素的哈希值冲突（即多个元素映射到同一个桶），那么采用开链法（链地址法）来解决冲突。将新的元素插入到对应桶的链表的头部。这样，链表中的元素按照插入的顺序进行排列。
4. 负载因子检查：在插入元素后，会检查负载因子，即当前哈希表中元素的数量与桶的数量之间的比值。如果负载因子超过最大负载因子（通常为0.5或0.75），就会触发rehash操作，以重新调整桶的数量，从而减少哈希冲突并保持哈希表的高效性能。



## 什么时候rehash？

STL中的`hashtable`在插入新元素时会检查负载因子（load factor），并根据负载因子来判断是否需要进行rehash操作。负载因子表示当前哈希表中元素的数量与桶的数量之间的比值。

`hashtable`在插入新元素时会进行以下判断：

1. 如果插入新元素后，负载因子小于等于最大负载因子（通常为0.5或0.75），则不会进行rehash操作，直接将元素插入到相应的桶中。因为负载因子比较低，哈希表仍然有足够的空间来容纳新元素。
2. 如果插入新元素后，负载因子超过最大负载因子，即当前哈希表中的元素数量接近或超过了桶的数量，这时就会触发rehash操作。rehash操作会重新调整桶的数量，通常会将桶的数量扩大一倍，并将已有元素重新分配到新的桶中。这样做的目的是为了减少哈希冲突，保持哈希表的高效性能。



