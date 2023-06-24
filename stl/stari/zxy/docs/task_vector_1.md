<!--
 * @Author: XavierZXY zxy_xavier@163.com
 * @Date: 2023-06-24 08:44:43
 * @LastEditors: XavierZXY zxy_xavier@163.com
 * @Description: stl-vector源码学习
 * 
 * Copyright (c) 2023 by zxy_xavier@163.com, All Rights Reserved. 
-->
# stl-vector学习
> [stl源码](https://github.com/gcc-mirror/gcc/blob/releases/gcc-9/libstdc%2B%2B-v3/include/bits/stl_vector.h)
> [stl源码分析](https://github.com/FunctionDou/STL)
## 基本结构
- `_Vector_base`: 一个模板结构体，用于分配内存和管理 vector 的容量和大小等信息；
- `_Vector_impl`: _Vector_base 的子类，用于存储 vector 的元素。


### `_Vector_base`
- 主要成员变量
    - _Tp_alloc_type: 分配器类型，用于分配和释放内存。
    - _M_start: 指向 vector 存储空间的起始位置。
    - _M_finish: 指向 vector 中最后一个元素的下一个位置。
    - _M_end_of_storage: 指向 vector 存储空间的末尾位置。
- 主要成员函数
    - _Vector_base(): 构造函数，用于初始化 _Tp_alloc_type 和 _M_start 等成员变量。
    - _Vector_base(size_type __n): 构造函数，用于分配大小为 __n 的存储空间。
    - _Vector_base(size_type __n, const allocator_type& __a): 构造函数，用于分配大小为 __n 的存储空间，并使用分配器 __a 进行内存分配。
    - ~_Vector_base(): 析构函数，用于释放 _M_start 指向的存储空间。
    - capacity() const: 返回 vector 的容量大小。
    - size() const: 返回 vector 中元素的个数。
    - empty() const: 判断 vector 是否为空。
    - reserve(size_type __n): 用于预留至少能容纳 __n 个元素的存储空间。
    - swap(_Vector_base& __x): 用于交换当前 vector 和另一个 vector 的存储空间。

###  `_Vector_impl`
- 主要成员函数
    - _Vector_impl(): 构造函数，用于初始化 _Tp_alloc_type 和 _M_start 等成员变量。
    - _Vector_impl(const allocator_type& __a): 构造函数，用于初始化 _Tp_alloc_type 和 _M_start 等成员变量，并使用分配器 __a 进行内存分配。
    - ~_Vector_impl(): 析构函数，用于释放 _M_start 指向的存储空间。
    - begin(): 返回指向 vector 中第一个元素的迭代器。
    - end(): 返回指向 vector 中最后一个元素的下一个位置的迭代器。
    - rbegin(): 返回指向 vector 中最后一个元素的反向迭代器。
    - rend(): 返回指向 vector 中第一个元素的前一个位置的反向迭代器。
    - front(): 返回 vector 中第一个元素的引用。
    - back(): 返回 vector 中最后一个元素的引用。
    - operator[](size_type __n): 返回 vector 中第 __n 个元素的引用。
    - at(size_type __n): 返回 vector 中第 __n 个元素的引用，并进行越界检查。
    - data(): 返回指向 vector 存储空间的指针。
    - push_back(const value_type& __x): 将元素 __x 添加到 vector 的末尾。
    - pop_back(): 删除 vector 中最后一个元素。
    - erase(iterator __position): 删除迭代器 __position 指向的元素。
    - erase(iterator __first, iterator __last): 删除迭代器 __first 和 __last 之间的元素。
    - clear(): 删除 vector 中所有元素。