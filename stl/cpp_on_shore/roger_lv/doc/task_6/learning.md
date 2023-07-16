## 迭代器-iterator

- iterator主要是用来遍历容器中的数据节点，即不用在外部知道容器内部实现原理，就能遍历数据。**迭代器说白了就是一个带有模板性质的指针**，没什么特别的，我们可以定义成类或者结构体

- 所以iterator的主要功能有：支持++，遍历元素; 支持*，取元素程序; 支持->，指针操作; 支持==和!=操作，比较iterator是否到了结尾。

- 迭代器类或者说结构体的基本定义：

    ```c++
    template<typename _Category, typename _Tp, typename _Distance = ptrdiff_t,
           typename _Pointer = _Tp*, typename _Reference = _Tp&>
    struct iterator
    {
      /// One of the @link iterator_tags tag types@endlink.
      typedef _Category  iterator_category;
      /// The type "pointed to" by the iterator.
      typedef _Tp        value_type;
      /// Distance between iterators is represented as this type.
      typedef _Distance  difference_type;
      /// This type represents a pointer-to-value_type.
      typedef _Pointer   pointer;
      /// This type represents a reference-to-value_type.
      typedef _Reference reference;
    };
    ```

- 实现起来也很简单，参照不同容器下自己定义的iterator即可，`std::vector<int>::iterator`

    ```c++
    // 比如自己写一个iterator类，但注意这里必须要传一个容器节点Node而不是直接传一个容器类
    template <typename T>
	class ListIter
	{
	public:
	  using value_type = T;
	  using reference = T & ;
	  using const_referenct = const T&;
	  using pointer = T * ;
	  using const_pointor = const T*;
	  using size_type = size_t;
	  using difference_type = ptrdiff_t;

	  ListIter(pointer p = nullptr) : Iter(p) {}

	  bool operator==(const ListIter& rhs) const noexcept
	  {
		return Iter == rhs.Iter;
	  }
	  bool operator!=(const ListIter& rhs) const noexcept
	  {
		return Iter != rhs.Iter;
      }
	  ListIter& operator++()
	  {
		Iter = Iter->next();
		return *this;
	  }
	  ListIter& operator++(int)
	  {
		value_type tmp = *this;
		++&*this;
		return tmp;
	  }
	  reference operator*()
	  {
		return *Iter;
	  }
	  pointer operator->()
	  {
		return Iter;
	  }
	private:
	  pointer Iter;
	};
    ```
