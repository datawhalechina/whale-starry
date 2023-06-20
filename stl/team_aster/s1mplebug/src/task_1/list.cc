#if 0
// node struct with two pointers.
struct _List_node_base {
  _List_node_base* _M_next;
  _List_node_base* _M_prev;
};

// node struct with data value.
template <typename _Tp>
struct _List_node : public _List_node_base {
  _Tp _M_data;
};

// list iterator
template <typename _Tp>
struct _List_iterator {
  typedef _List_iterator<_Tp> _Self;
  typedef _List_node<_Tp> _Node;

  typedef _Tp value_type;
  typedef _Tp* pointer;
  typedef _Tp& reference;

  _List_node_base* _M_node;

  // two construct function.
  _List_iterator() : _M_node() {}
  explicit _List_iterator(_List_node_base* __x) : _M_node(__x) {}

  // return iterator self.
  _Self _M_const_cast() const { return *this; }

  // operator functions.
  reference operator*() const { return static_cast<_Node>(_M_node)->_M_data; }
  // pointer operator->() const { return }
  _Self& operator++() {
    _M_node = _M_node->_M_next;
    return *this;
  }
  _Self operator++(int) {
    _Self __tmp = *this;
    _M_node = _M_node->_M_next;
    return __tmp;
  }
  _Self& operator--() {
    _M_node = _M_node->_M_prev;
    return *this;
  }
  _Self operator--(int) {
    _Self __tmp = *this;
    _M_node = _M_node->_M_prev;
    return __tmp;
  }
  bool operator==(const _Self& __x) const { return _M_node == __x._M_node; }
  bool operator!=(const _Self& __x) const { return _M_node != __x._M_node; }
};
#endif

/**
 * 定义容器空间配置器
 **/
// template<typename T>
// struct Allocator
// {
//     T *allocate(size_t size)   //只负责内存的开辟 不进行对象的构造
//     {
//         return (T *)malloc(sizeof(T) * size);
//     }
//     void deallocate(void *p)    //只负责内存的释放 不尽兴对象的析构
//     {
//         free(p);
//     }
//     // void construct(T *p, const T &val)  //负责对象构造
//     // {
//     //     new (p) T(val);
//     // }
//     // void construct(T *p, T &&val)  //负责对象构造
//     // {
//     //     new (p) T(std::move(val));
//     // }
//     template<typename Ty>
//     void construct(T *p, Ty &&val)
//     {
//         new (p) T(std::forward<Ty>(val));
//     }
//     void destroy(T *p)
//     {
//         p->~T();
//     }
// };

/* list */
#if 0
template<typename _Tp, typename _Alloc>
  class _List_base {
    protected:
      // 存储的实例实际上并非「allocator_type」类型，相反，我们将类型rebind在了Allocator<List_node<Tp>>上
      // List_node<Tp>与Tp大小不同（两个指针那么大），因为List_node<Tp>正在被绑定着，所以可能Tp伤的特化未被使用
      typedef typename _Alloc::template rebind<_List_node<_Tp>>::other _Node_alloc_type;
      typedef typename _Alloc::template rebind<_Tp>::other _Tp_alloc_type;

      // 这里 _Node_alloc_type 和 _Tp_alloc_type还没搞懂 类型萃取 ?
      struct _List_impl : public _Node_alloc_type {
        _List_node_base _M_node; // 这是哨兵

        // construct function
        _List_impl() : _M_node() {}
        _List_impl(const _Node_alloc_type &__a) : Node_alloc_type(__a), _M_node() {}
      };

      _List_impl _M_impl; // 这个接口应该是用来分配内存的
      // _M_get_node function申请堆空间（配置一个节点并传回） _M_put_node function释放堆空间（释放一个节点）
      _List_node<_Tp> *_M_get_node() { return _M_impl.Node_alloc_type::allocate(1); }
      void _M_put_node(_List_node<_Tp> *__p) { _M_impl._Node_alloc_type::deallocate(__p, 1); }
    

    public:
      typedef _Alloc allocator_type; // 分配器直接定义为allocator_type类型

      _Node_alloc_type &_M_get_Node_allocator() {
        return *static_cast<_Node_alloc_type *>(&_M_impl);
      }
      _Tp_alloc_type _M_get_Tp_allocator() const {
        return _Tp_alloc_type(_M_get_Node_allocator());
      }
      allocator_type get_allocator() const {
        return allocator_type(_M_get_Node_allocator());
      }


      // 初始化单个节点，让这个节点的prev和next指针都指向自己，每个节点是一个_List_base
      void _M_init() {
        this->_M_impl._M_node._M_next = &this->_M_impl._M_node;
        this->_M_impl._M_node._M_prev = &this->_M_impl._M_node;
      }

      // 还有四个分配函数 没搞懂
      _Node_alloc_type &_M_get_Node_allocator() { return *static_cast<_Node_alloc_type *>(&_M_impl); }
      const _Node_alloc_type &_M_get_Node_allocator() const { return *static_cast<_Node_alloc_type *>(&_M_impl); }
      _Tp_alloc_type _M_get_Tp_allocator() const { return _Tp_alloc_type(_M_get_Node_allocator()); }
      allocator_type get_allocator() const { return allocator_type(_M_get_Node_allocator()); }

      // construct function
      _List_base() : _M_impl() { _M_init(); }
      _List_base(const _Node_alloc_type &__a) : _M_impl(__a) { _M_init(); }

      void _M_clear();
      ~_List_base() { _M_clear(); }
  };

template<typename _Tp, typename _Alloc = std::allocator<_Tp>>
  class list : protected _List_base<_Tp, _Alloc> {
    typedef typename _Alloc::value_type _Alloc_value_type;
    typedef _List_base<_Tp, _Alloc> _Base;
    typedef typename _Base::Node_alloc_type _Node_alloc_type;
    typedef typename _Base::_Tp_alloc_type _Tp_alloc_type;

  public:
    typedef _Tp value_type;
    typedef typename _Tp_alloc_type::pointer pointer;
    typedef typename _Tp_alloc_type::reference reference;
    typedef _List_iterator<_Tp> iterator;
    typedef size_t size_type;
    typedef _Alloc allocator_type;

  protected:
    typedef _List_node<_Tp> _Node;
    using _Base::_M_impl;
    using _Base::_M_put_node;
    using _Base::_M_get_node;
    using _Base::_M_get_Node_allocator;
    using _Base::_M_get_Tp_allocator;

    // 返回末尾空指针的next 作为当前的首节点 因此是个环形链表
    iterator begin() {
        return iterator(this->_M_impl._M_node._M_next);
    }
    iterator end() {
        return iterator(&this->_M_impl._M_node);
    }
    // next指向自己的时候 说明只有一个空哨节点 所以为空
    bool empty() const {
      return this->_M_impl._M_node._M_next == & this->_M_impl._M_node;
    }
    size_type size() const {
        return std::distance(begin(), end());
    }
  };
#endif

#include <ctime>
#include <iostream>
#include <list>

// 以下为12次测试结果，单位秒
// 0.9068 0.8604 0.8007 0.8015 0.7883 1.2490 1.1146 0.9116 0.8705 0.8698 0.8731 0.8779
void test_list_insert(std::list<int>& l) {
  clock_t stime = clock();
  for (int i = 0; i < (int)1e7; ++i) l.push_back(i);
  std::cout << (double)(clock() - stime) / CLOCKS_PER_SEC << std::endl;
}

int main() {
  std::list<int> l;
  test_list_insert(l);
  return 0;
}