# stl_map源码阅读

## map概述

在STL-map中，所有的元素都会根据元素的key进行自动排序，map的每一个元素都是`pair<key, value>`。注意map中不允许两个元素拥有相同的key。map的底层实现依旧是红黑树。

时间复杂度：

- 插入 `O(logn)`
- 删除 `O(logn)`
- 修改 `O(logn)`
- 查询 `O(logn)`



### map类设计

stl_map中每一个元素都是`std::pair<const _Key, _Tp>`，默认的排序规则是`std::less<_Key>`，即根据_Key值从小到大排列。

```cpp
  template <typename _Key, typename _Tp, typename _Compare = std::less<_Key>,
	    typename _Alloc = std::allocator<std::pair<const _Key, _Tp> > >
    class map
    {
    public:
      typedef _Key					key_type;
      typedef _Tp					mapped_type;
      typedef std::pair<const _Key, _Tp>		value_type;
      typedef _Compare					key_compare;
      typedef _Alloc					allocator_type;
      ...
    }

```



#### 构造函数

```cpp
/*默认构造*/
map() : _M_t() { } // 


/*指定比较器的构造*/
explicit
map(const _Compare& __comp,
    const allocator_type& __a = allocator_type())
: _M_t(__comp, _Pair_alloc_type(__a)) { } 

/*拷贝构造*/
map(const map& __x): _M_t(__x._M_t) { }

/*移动构造，减少不必要的拷贝，提升性能*/
map(map&&) = default;

/*批量初始化*/
template<typename _InputIterator>
map(_InputIterator __first, _InputIterator __last);
```

#### 迭代器相关

```cpp
begin(); //首
end();   //尾
rbegin();//尾
rend();  //首
...

```

#### 常用接口

```cpp
empty();  	 // 是否为空
size();   	 // 大小
max_size();  // 最大容量
mapped_type& operator[](const key_type& __k); // 运算符重载 map[key]，实现思路是二分，作用是查找，也可以插入
at(); // 安全的获取 map.at(key)，会判断是否超出范围
std::pair<iterator, bool> insert(const value_type& __x) //插入，map.insert(std::pair<key, value>(KEY, VALUE));
insert(iterator __position, const value_type& __x); //指定位置插入以提高性能，不常用
erase(iterator __position); // 删除指定位置
erase(const key_type& __x); // 删除Key对应的元素 
erase(iterator __first, iterator __last); // 批量删除
swap(map& __x); // 与另一个map交换数据
iterator find(const key_type& __x); // 查找Key对应的元素 
size_type count(const key_type& __x); //统计 Key对应的元素 的个数，map中只为1或0
iterator lower_bound(const key_type& __x); //找到比__x对应的key小的
iterator upper_bound(const key_type& __x); //找到比__x对应的key大的


```



## 填坑：关于完美转发

STL中运用了大量的**移动构造**和**完美转发**，减少不必要的拷贝，提升性能。在这里对`std::forward`和`std::move`进行一些实验，加深理解。下面这些代码里涉及左值右值、引用折叠、完美转发。

```cpp
#include <iostream>

template <typename T>
void print(T& t) {
  std::cout << "lvalue" << std::endl;
}
template <typename T>
void print(T&& t) {
  std::cout << "rvalue" << std::endl;
}

template <typename T>
void TestForward(T&& v) {
  print(v);
  print(std::forward<T>(v));
  print(std::move(v));
}

int main() {
  TestForward(1);
  /*
   print(v);
   //lvalue. 1 是右值，传入TestForward后 v是右值引用，v是左值
   print(std::forward<T>(v));
   //rvalue. 1 是右值，传入TestForward后v是右值引用，forward转发后是右值引用
   print(std::move(v));
   //rvalue. 1 是右值，传入TestForward后v是右值引用，std::move后返回右值引用
  */
  int x = 1;
  TestForward(x);
  /*
   print(v);
   //lvalue v是左值，传入TestForward后 v是左值引用，v是左值
   print(std::forward<T>(v));
   //lvalue v被forward转发后依然是左值引用
   print(std::move(v));
   //rvalue v被move后转化为右值引用
  */
  TestForward(std::forward<int>(x));
  /*
   print(v);
   //lvalue std::forward<int>(x)是右值，v是右值引用
   print(std::forward<T>(v));
   //rvalue v被forward转发后依然是右值引用
   print(std::move(v));
   //rvalue v被move后依然是右值引用
  */
  return 0;
}
```



