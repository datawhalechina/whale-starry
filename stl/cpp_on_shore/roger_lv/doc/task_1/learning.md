## 小试牛刀-list学习笔记

- **STL基本框架**
  ![img](https://rogerlv51.blob.core.windows.net/images/stl_component.png)
  ![img](https://rogerlv51.blob.core.windows.net/images/stl_basic.png)

- **关于STL中Allocator的一些说明**

  通常一个对象的完整生命周期应该是operator::new分配内存空间->调用构造函数->调用析构函数->delete</br>
  而Allocator实际上就是提供一个泛化的内存分配接口，把上述4个功能统一分配到一个类中，方便管理</br>
  一般我们是不用手动采用`vetor<int, allocator<int>> v`这种方式的，而是直接`vector<int> v`，系统会默认使用分配器</br>

  **在GCC中底层实际上是基于一个叫`new_allocator`的类实现的**

  ```C++
    // 底层主要是下面4个函数，分别表示new，delete，构造，析构
    pointer allocate(size_type __n, const void* = static_cast<const void*>(0))
    void deallocate(pointer __p, size_type)
    void construct(_Up* __p, _Args&&... __args)
    void destroy(_Up* __p)
  ```


- **关于list在gcc中的基础介绍：**

    A standard container with linear time access to elements, and fixed time insertion/deletion at any point in the sequence.

    list是protected继承于_List_base的，它们都有两个模板类型分别是_Tp(Type of element)、_Alloc(Allocator type)，实现上均为模板泛型编程

    A list can be represented as `A <---> B <---> C <---> D` is actually circular; a link exists between A and D. The list::iterator pointing to the end of the list. To get to the head of the list, we start at the tail and move forward by one. When this member iterator's next/previous pointers refer to itself, the list is empty.

    - **基本数据结构构成**
    ```C++
    struct _List_node_base {
        _List_node_base* _M_next;
        _List_node_base* _M_prev;
        _Tp data;
    };   可以看到每一个list节点都有两个指针一个指向前一个节点一个指向后一个节点，还有一个data值
    ```

- **基本成员变量定义**

|                        定义                        |          名称          |
| :------------------------------------------------: | :--------------------: |
|                    typedef _Tp                     |       value_type       |
|     typedef typename _Tp_alloc_traits::pointer     |        pointer         |
|  typedef typename _Tp_alloc_traits::const_pointer  |     const_pointer      |
|    typedef typename _Tp_alloc_traits::reference    |       reference        |
| typedef typename _Tp_alloc_traits::const_reference |    const_reference     |
|            typedef _List_iterator<_Tp>             |        iterator        |
|         typedef _List_const_iterator<_Tp>          |     const_iterator     |
|   typedef std::reverse_iterator<const_iterator>    | const_reverse_iterator |
|      typedef std::reverse_iterator<iterator>       |    reverse_iterator    |
|                   typedef size_t                   |       size_type        |
|                 typedef ptrdiff_t                  |    difference_type     |
|                   typedef _Alloc                   |     allocator_type     |

- **基本成员函数定义**
    - **构造函数**
      - list(size_type __n): 传入参数为一个long unsigned int类型，函数中调用_M_default_initialize实际上这里面又是调用emplace_back从末尾插入默认值0
      - list(size_type __n, const value_type& __value)：与第一个constructor的唯一区别是可以指定初始化的值，内部调用_M_fill_initialize函数实际上是通过push_back实现，比较好奇这里为什么又不使用emplace_back尾插了呢？
      - list(const list& __x)：拷贝构造传入一个常量引用，内部实际上也是使用_M_fill_initialize实现的
      - list(initializer_list<value_type> __l)：移动构造传入初始化列表，内部实际上也是使用_M_fill_initialize实现的

    - **其他函数**
      - operator=：有三种形式，这里不一一列举了，将一个list赋值给另一个list，深拷贝   
      - assign：可以传一个size_t的数字和模板类型值，或者传两个迭代器，或者一个初始化列表，目标是修改list所有的值
      - get_allocator：返回内存分配对象

    - **迭代器相关**
      - begin：Returns a read/write iterator that points to the first element in the list
      - end：Returns a read/write iterator that points one past the last element in the list
      - rbegin：Returns a read/write reverse iterator that points to the last element in the list  obviously rbegin = end
      - rend：Returns a read/write reverse iterator that points to one before the first element in the list   obviously rend = begin
      - cbegin：Returns a read-only (constant) iterator that points to the first element in the list
      - cend：Returns a read-only (constant) iterator that points one past the last element in the list

    - **capacity相关**
      - empty：Returns whether the list is empty
      - size：Returns the number of elements in the list
      - max_size：Returns the maximum number of elements the list can hold

    - **基础元素操作相关**
      - resize：Resizes the list to the specified number of elements.
      - front：Returns a read/write reference to the data at the first. 如果带const则不能修改
      - back：Returns a read/write reference to the data at the last element
      - push_front：Add data to the front of the list. 顾名思义头插，底层调用_M_insert函数
      - emplace_front：功能与push_front一致，效率应该更高
      - pop_front：Removes first element. 底层调用_M_erase函数
      - push_back：Add data to the end of the list. 底层也是使用_M_insert函数
      - emplace_back：功能一致
      - pop_back：Removes last element. 底层调用_M_erase函数
      - emplace：在指定迭代器前一个位置修改赋予给定值
      - insert：This function will insert a copy of the given value before the specified location. 还可以插入一个区间范围给list
      - erase：Remove element at given position. 还可以删除list的一个区间范围
      - swap：Swaps data with another list.
      - clear：Removes all elements from the list.
      - splice：Insert contents of another list. 插入给定位置之前
      - remove：Remove all elements equal to value.
      - remove_if：Remove all elements satisfying a predicate.
      - unique：Remove consecutive duplicate elements. 同样也可以指定要删除的值
      - merge：Merge sorted lists.
      - reverse：Reverse the elements in list.
      - sort：Sort the elements. 可以自定义排序函数

    - **操作符重载函数**
      - operator==：List equality comparison.
      - operator<：List ordering relation.

- **一些基础知识补充**
    - **右值引用的作用：** 节省内存空间，提高效率，减少内存拷贝 &&，对于T&&为万能引用既能接收左值也能接收右值
    - **std::move移动语义的作用：** 把一个左值转化成右值，也是为了节省内存空间
    - 我们通常在自己构造类的时候会引入一个移动构造函数，可避免在初始化的时候同时调用构造和拷贝构造，节省空间
      ```Cpp
        DataBlock(DataBlock&& db) noexcept {
          this->data = db.data;
          db.data = nullptr;
          cout << "Move Constructor being invoked!" << endl;
        }
      ```
    - 我们通常会使用一个const将右值绑定到左值引用上`const int& size = 1000`

- **以下是我代码的整体测试输出**

  ![img](https://rogerlv51.blob.core.windows.net/images/list_test.png)