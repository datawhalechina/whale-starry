## STL: vector

### vector概述

vector的底层类似数组，拥有一段连续的内存空间，但是vector是动态空间，可以通过随着新元素的加入，内部可以自动扩充空间。自动扩容的具体细节会在下面指出。

vector的时间复杂度：

- 插入：o(n)
- 删除：o(n)
- 改：o(1)
- 查：o(1)

### vector源码阅读

#### Vector的类

##### struct _Vector_base

list的基类，该类的构造函数和析构函数会进行内存的分配与释放，但不会进行初始化。

```cpp
_Vector_base(const _Node_alloc_type& __a); // 分配 a 个node大小的空间；
~_Vector_base(); // 销毁，内部会调用_M_deallocate释放内存
struct _Vector_impl_data
{
	pointer _M_start; // 指向目前使用到的空间的起始
	pointer _M_finish;// 指向目前使用到的空间的末尾
	pointer _M_end_of_storage; // 指向目前可用的空间的末尾
}
```



##### class vector

构造函数

1. `vector(const allocator_type& __a)` 创建一个没有元素的vector
2. `vector(size_type __n, const allocator_type& __a = allocator_type())` 创建n个元素的vector
3. `vector(size_type __n, const value_type& __value, const allocator_type& __a = allocator_type())` 创建 n 个相同的 值为 __value 的元素的vector
4. `vector(const vector& __x)` 拷贝构造
5. ==移动构造？？？==

析构函数

1. `~vector()` 会删除使用的内存空间

常见接口

```cpp
assign(size_type __n, const value_type& __val); // 将vector设置为 n 个 val，会完全改变原来的vector
assign(_InputIterator __first, _InputIterator __last);
begin();
end();
=====================================
size(); // 已经使用的空间大小
max_size(); // 可以使用的空间大小
resize(size_type __new_size); // 如果resize变大，默认补 0, 也可以指定
empty(); //判断是否为空
reserve(size_type __n); // 提前预分配内存
=====================================
front(); 
back(); 
push_back();
emplace_back(); // C++11之后默认调用这个
pop_back();
emplace(const_iterator __position, _Args&&... __args);
insert(const_iterator __position, const value_type& __x);
erase(const_iterator __position);
erase(const_iterator __first, const_iterator __last); //注意区间左闭右开
```



#### 扩容机制

vector的动态分配内存是他的亮点，所以测试了一下内部的扩容是如何做到的。

从结果来看，如果插入的元素数量多余剩余的可用的空间数量，那么会首先把capacity翻倍提高（例如下面结果中，7-->14）。如果扩充两倍还不够，则会继续扩大翻倍提高。

```cpp
// 测试扩容机制
  std::vector<int> vec_4;
  std::cout << "capacity of vec_4: " << vec_4.capacity() << std::endl;
  PrintVector(vec_4);
  vec_4.push_back(1);
  vec_4.push_back(1);
  std::cout << "capacity of vec_4: " << vec_4.capacity() << std::endl;
  PrintVector(vec_4);
  vec_4.insert(vec_4.end(), 5, 2);
  std::cout << "capacity of vec_4: " << vec_4.capacity() << std::endl;
  PrintVector(vec_4);
  vec_4.insert(vec_4.end(), 1, 2);
  std::cout << "capacity of vec_4: " << vec_4.capacity() << std::endl;
  PrintVector(vec_4);  // 会以两倍的容量进行扩容
  for (int i = 0; i < 10; i++) {
    vec_4.insert(vec_4.end(), 1, i);
    std::cout << "capacity of vec_4: " << vec_4.capacity() << std::endl;
  }

====OUTPUT=====
capacity of vec_4: 0

capacity of vec_4: 2
1 1
capacity of vec_4: 7
1 1 2 2 2 2 2
capacity of vec_4: 14
1 1 2 2 2 2 2 2
capacity of vec_4: 14
capacity of vec_4: 14
capacity of vec_4: 14
capacity of vec_4: 14
capacity of vec_4: 14
capacity of vec_4: 14
capacity of vec_4: 28
capacity of vec_4: 28
capacity of vec_4: 28
capacity of vec_4: 28
```



### 存疑部分？

==移动构造？？？==

