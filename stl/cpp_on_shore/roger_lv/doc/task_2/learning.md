## 面试必备-vector学习笔记

- **vector在GCC中的介绍**

    A standard container which offers fixed time access to individual elements in any order.
    In some terminology a vector can be described as a dynamic C-style array, it offers fast and efficient access to individual elements in any order and saves the user from worrying about memory and size allocation.
    ```Cpp
    template<typename _Tp, typename _Alloc = std::allocator<_Tp> > class vector
    ```
    由于vector和list的成员类型基本一致，在此不重复记录</br>
    不同于list是一个双向链表，vector的存储空间是连续的，只是元素可以任意读写</br>
    向量（Vector）是一个封装了动态大小数组的顺序容器（Sequence Container）。跟任意其它类型容器一样，它能够存放各种类型的对象。可以简单的认为，向量是一个能够存放任意类型的动态数组。不同于list，vector只有尾插没有头插操作。

- **基本成员函数介绍**

    - **vector(size_type __n):** Creates a vector with default constructed elements. 默认值为0，底层调用的是_M_default_initialize函数

    - **vector(size_type __n, const value_type& __value):** Creates a vector with copies of an exemplar element. 底层调用的是_M_fill_initialize函数

    - **vector(const vector& __x):** 拷贝构造，All the elements of __x are copied, but any unused capacity in __x  will not be copied

    - **void assign(size_type __n, const value_type& __val):** Assigns a given value to a vector. 指定传入n个val给vector

    - **iterator begin():** Returns a read/write iterator that points to the first element in the vector.

    - **iterator end():** Returns a read/write iterator that points one past the last element in the vector.

    - **reverse_iterator rbegin():** Returns a read/write reverse iterator that points to the last element in the vector.  rbegin() == --end()

    - **reverse_iterator rend():** Returns a read/write reverse iterator that points to one before the first element in the vector

    - **size_type size():** Returns the number of elements in the vector. 底层就是直接vector中定义的尾指针-头指针得到元素的个数，转型成size_type输出

    - **void resize(size_type __new_size):** Resizes the vector to the specified number of elements. If the number is smaller than the vector's current size the vector is truncated, otherwise default constructed elements are appended. 此外还可以指定多出的值填什么

    - **void shrink_to_fit():** A non-binding request to reduce capacity() to size(). 把capacity和size对齐，即缩小capacity到size. 底层调用的是_S_do_it这个函数

    - **size_type capacity():** Returns the total number of elements that the vector can hold before needing to allocate more memory. 即容器初始化时赋予的大小

    - **bool empty():** Returns true if the vector is empty.

    - **void reserve(size_type __n):** Attempt to preallocate enough memory for specified number of elements. 具体实现在vector.tcc中，比如reserve(10)则capacity就是10. 这样的好处是在于我们提前知道处理的数据数量级大概是多少，就可以提前开辟，节约后续处理事件

    - **reference operator[](size_type __n):** Subscript access to the data contained in the vector. Read/write reference to data. 很显然我们平时可以通过[]修改值是因为这里返回的是引用，根据源码剖析，显然这里的查找时间复杂度为O(1)。如果索引超出数组大小则返回默认值0

    - **reference at(size_type __n):** Provides access to the data contained in the vector. 使用at随机读取/修改vector中的值和`[]`的唯一区别就是at要求不能越界，加了一步`__n<capcity`的检查，底层实际上调用的还是`[]`

    - **reference front():** Returns a read/write reference to the data at the first element of the vector.  内部实现直接返回*begin()

    - **reference back():** Returns a read/write reference to the data at the last element of the vector.  内部实现直接返回*(end()-1)

    - **_Tp\* data():** Returns a pointer such that [data(), data() + size()) is a valid range.  For a non-empty vector, data() == &front(). 返回vector的首地址

    - **void push_back(const value_type& __x):** Add data to the end of the vector. 典型的栈操作，时间复杂度O(1). 如果加入新元素后大小超过了原先分配的capacity则要先分配额外空间。底层调用_M_realloc_insert函数。实际上自C++11之后push_back也是调用emplace_back函数的
        ```C++
        void push_back(const value_type& __x)
        {
            if (this->_M_impl._M_finish != this->_M_impl._M_end_of_storage)
            {
                _GLIBCXX_ASAN_ANNOTATE_GROW(1);
                _Alloc_traits::construct(this->_M_impl, this->_M_impl._M_finish,
                            __x);
                ++this->_M_impl._M_finish;
                _GLIBCXX_ASAN_ANNOTATE_GREW(1);
            }
            else
                _M_realloc_insert(end(), __x);
        }
        ```

    - **void emplace_back(_Args&&... __args):** 实现的功能和上述push_back一致。push和emplace的区别就在于：push首先是执行模板类的构造函数创建了一个临时的对象, 然后再通过拷贝构造函数把这个临时对象的成员变量值复制到vector对应的空间里. 而emplace则是直接在vector已有的空间上, 直接调用了模板类的构造函数
        ```C++
            vector<_Tp, _Alloc>::emplace_back(_Args&&... __args)
            {
                if (this->_M_impl._M_finish != this->_M_impl._M_end_of_storage)
                {
                    _GLIBCXX_ASAN_ANNOTATE_GROW(1);
                    _Alloc_traits::construct(this->_M_impl, this->_M_impl._M_finish,
                                std::forward<_Args>(__args)...);
                    ++this->_M_impl._M_finish;
                    _GLIBCXX_ASAN_ANNOTATE_GREW(1);
                }
                else
                    _M_realloc_insert(end(), std::forward<_Args>(__args)...);
            }
        ```

    - **void pop_back():** Removes last element.

    - **iterator emplace(const_iterator __position):** Inserts an object in vector before specified iterator. 采用这种插入方式代价是很高的，如果实在要频繁插入建议是使用list

    - **iterator insert(const_iterator __position, const value_type& __x):** Inserts given value into vector before specified iterator. Insert甚至可以指定插入几个__x值；插入一个区间

    - **iterator erase(const_iterator __position):** Remove element at given position. 注意传入的是迭代器，一个细节是erase之后会改变vector的size却不会改变原始的capacity. 底层实际调用地是针对迭代器的_M_erase函数，同样也可以指定一个区间删除

    - **void swap(vector& __x):** Swaps data with another vector. Note that the global std::swap() function is specialized such that std::swap(v1,v2) will feed to this function.

    - **void clear():** Erases all the elements.  Note that this function only erases the elements

- **知识补充：关于std::forword()**

    可以将一个参数的左右值特性原封不动地转发给其他函数，它可以保证参数的左右值特性在传参时不改变！
    ```C++
    #include <iostream>
    #include <utility>

    void func(int& x) {
        std::cout << "lvalue reference: " << x << std::endl;
    }

    void func(int&& x) {
        std::cout << "rvalue reference: " << x << std::endl;
    }

    template<typename T>
    void wrapper(T&& arg) {
        func(std::forward<T>(arg));
    }

    int main() {
        int x = 42;
        wrapper(x);  // lvalue reference: 42
        wrapper(1);  // rvalue reference: 1
        return 0;
    }
    ```

- **个人代码测试结果如下:**

    ![img_test](https://rogerlv51.blob.core.windows.net/images/vector_test.png)



