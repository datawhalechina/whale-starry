# Task2 的学习笔记
## 概述
std::vector是一个动态数组，它能保证元素在内存中的连续存储，同时允许快速访问和修改。
## 部分接口说明
### shrink_to_fit
在插入元素时，若vector的空间不够，vector会重新申请一段更大的空间，将老数据拷贝到新空间中，同时将插入元素，在某些场景下会造成vector尾部有空间浪费，vector提供了`shrink_to_fit()`函数将这部分空间释放，最终调用如下：
```c++
template<typename _Alloc> _GLIBCXX20_CONSTEXPR bool vector<bool, _Alloc>::_M_shrink_to_fit()
{
    if (capacity() - size() < int(_S_word_bit))
        return false;
    __try
    {
        if (size_type __n = size())
            _M_reallocate(__n);
        else
        {
            this->_M_deallocate();
            this->_M_impl._M_reset();
        }
        return true;
    }
    __catch(...)
    { return false; }
}
```
### reserve
`void reserve(size_t n)`接口的作用是为vector分配一定的内存空间，以避免频繁的内存分配和释放，从而提高程序的效率。但是，reserve()函数并不会改变vector的大小，也不会初始化新分配的内存。因此，如果在调用reserve()函数后直接访问vector的元素，可能会访问到未初始化的内存，导致不可预期的行为。实现如下：
```c++
void reserve(size_type __n)
{
    if (__n > max_size())
        __throw_length_error(__N("vector::reserve"));
    if (capacity() < __n)
        _M_reallocate(__n);
}
```
### resize
`resize()`函数的作用是改变vector的大小，如果新的大小比原来的大小大，则会分配更多的内存空间，并对新元素进行初始化。如果新的大小比原来的大小小，则会删除多余的元素，并释放多余的内存空间。需要注意的是，resize()函数会改变vector的大小，因此可能会涉及到元素的复制和销毁，比较耗时。
```c++
void resize(size_type __new_size, bool __x = bool())
{
    if (__new_size < size())
        _M_erase_at_end(begin() + difference_type(__new_size));
    else
        insert(end(), __new_size - size(), __x);
}
```
## 总结
`vector`是一个很灵活的基础容器，提供了比数组更灵活的使用方式，在使用时如果提前可以知道需要插入的元素数，可以提前使用`reserve`或者`resize`申请空间，避免在插入元素时发生多次空间的申请和元素的复制。

需要注意的是，如果用`resize`之后使用`emplace_back`时会导致数据插入在最后位置，下面是示例：
```c++
int main()
{
    std::vector<int> vv;
    vv.resize(5);
    vv.emplace_back(123);
    for (const auto& item : vv) {
        std::cout << item << std::endl;
    }
    return 0;
}
```
打印的结果：
```shell
0
0
0
0
0
123
```