#include <assert.h>
#include <iostream>
template <class _Ty>
class list {
 public:
  struct _Node;
  typedef _Node* _Nodeptr;
  struct _Node {
    _Ty _Value;
    _Nodeptr _Next;
    _Nodeptr _Prev;
  };

  struct _Acc {
    typedef _Nodeptr& _Nodepref;
    typedef _Ty& _Vref;
    static _Nodepref _Next(_Nodeptr _P) {
      return ((_Nodepref)(*_P)._Next);  // _p->_next
    }
    static _Nodepref _Prev(_Nodeptr _P) {
      return ((_Nodepref)(*_P)._Prev);  // _p->_pref
    }
    static _Vref _Value(_Nodeptr _P) {
      return ((_Vref)(*_P)._Value);  // _p->_value
    }
  };
  // 萃取
  typedef size_t size_type;
  typedef _Ty value_type;
  typedef _Ty* pointer_type;
  typedef _Ty& reference_type;
  typedef const _Ty* const_pointer_type;
  typedef const _Ty& const_reference_type;
  typedef int difference_type;

  class iterator;
  class const_iterator;

  friend class const_iterator;
  // 常迭代器
  class const_iterator {
    // 比较是否相同，如何++，如何--，如何取值
   public:
    const_iterator() {}
    const_iterator(_Nodeptr _P) : _Ptr(_P) {}
    const_iterator(const_iterator& _X) : _Ptr(_X._Ptr) {}
    // 重写*
    const_reference_type operator*() const {  // return _Acc::_Value(_Ptr);
      return _Ptr->_Value;
    }
    // 重写++
    // 前++
    const_iterator& operator++() {
      _Ptr = _Acc::_Next(_Ptr);
      return *this;
    }
    // 后++
    const_iterator operator++(int) {
      const_iterator _Tmp = *this;
      ++(*this);
      return _Tmp;
    }
    // 重写--
    // 前--
    const_iterator& operator--() {
      _Ptr = _Acc::_Prev(_Ptr);
      return *this;
    }
    // 后--
    const_iterator operator--(int) {
      const_iterator _Tmp = *this;
      --(*this);
      return _Tmp;
    }
    // 比较是否相同与不同
    bool operator==(const const_iterator& _X) const { return _Ptr == _X._Ptr; }
    bool operator!=(const const_iterator& _X) const {
      return !(*this == _X);  // 直接使用构造过的==
    }
    _Nodeptr _Mynode() const { return _Ptr; }

   protected:
    _Nodeptr _Ptr;
  };
  friend class iterator;
  // 迭代器
  class iterator : public const_iterator {
   public:
    iterator() {}
    iterator(_Nodeptr _P) : const_iterator(_P) {}
    // iterator(const_iterator& _X) : const_iterator(_X), _Ptr(_X._Mynode()) {}

    // 重写*
    reference_type operator*() const {
      return _Ptr->_Value;
    }  // return _Acc::_Value(_Ptr); }
    // 重写++
    // 前++
    iterator& operator++() {
      _Ptr = _Acc::_Next(_Ptr);
      return (*this);
    }
    // 后++
    iterator operator++(int) {
      iterator _Tmp = *this;
      ++(*this);
      return _Tmp;
    }
    // 重写--
    // 前--
    iterator& operator--() {
      _Ptr = _Acc::_Prev(_Ptr);
      return *this;
    }
    // 后--
    iterator operator--(int) {
      iterator _Tmp = *this;
      --(*this);
      return _Tmp;
    }
    // 比较是否相同与不同
    bool operator==(const iterator& _X) const { return _Ptr == _X._Ptr; }
    bool operator!=(const iterator& _X) const {
      return !(*this == _X);  // 直接使用构造过的==
    }

   protected:
    _Nodeptr _Ptr;
  };

 public:
  explicit list() : _Head(_Buynode()), _Size() {}  // 对私有属性的赋值尽量写在外面
  explicit list(size_type _N, const _Ty& _V = _Ty()) : _Head(_Buynode()), _Size(0) {
    insert(begin(), _N, _V);
  }
  explicit list(const _Ty* _F, const _Ty* _L) : _Head(_Buynode()), _Size(0) {
    insert(begin(), _F, _L);
  }

  // 返回大小
  size_type size() const { return _Size; }

  // 判空
  bool empty() const { return size() == 0; }

  // 返回头
  const_reference_type front() const { return (*begin()); }
  reference_type front() { return *begin(); }

  // 返回尾
  const_reference_type back() const { return (*(--end())); }
  reference_type back() { return *--end(); }

  const_iterator begin() const { return const_iterator(_Acc::_Next(_Head)); }
  iterator begin() { return iterator(_Acc::_Next(_Head)); }
  iterator end() { return iterator(_Head); }
  const_iterator end() const { return const_iterator(_Head); }

  // 尾插
  // void push_back(const_reference_type v) {
  //   // _s->_Value = v;
  //   // _s->_Prev = _Head->_Prev;
  //   // _Head->_Prev->_Next = _s;
  //   // _s->_Next = _Head;
  //   // _Head->_Prev = _s;
  //   _Nodeptr _S = _Buynode(_Head, _Acc::_Prev(_Head));
  //   _S->_Value = v;
  //   _Acc::_Next(_Acc::_Prev(_S)) = _S;
  //   _Acc::_Prev(_Head) = _S;
  //   _Size++;
  // }
  void push_back(const _Ty& _X) { insert(end(), _X); }
  // 头插
  void push_front(const _Ty& _X) { insert(begin(), _X); }
  void pop_back() { erase(--end()); }
  void pop_front() { erase(begin()); }
  void clear() { erase(begin(), end()); }
  void assign(iterator _F, iterator _L) {
    erase(begin(), end());
    insert(_F, _L);
  }

  void show() {
    _Nodeptr _P = _Head->_Next;
    while (_P != _Head) {
      std::cout << _P->_Value << "-->";
      _P = _P->_Next;
    }
    std::cout << "Over" << std::endl;
  }

  void show_iterator() {
    iterator _F = ++begin(), _L = end();
    std::cout << *_F << " == " << *_L << std::endl;
    int cc = 0;
    for (; _F != _L; ++_F) {
      if (cc > 10) {
        break;
      }
      std::cout << *_F << " ";
      cc++;
    }
  }

 public:
  // iterator insert(iterator _P, const _Ty& _X = _Ty()) {
  //   _Nodeptr _Ptr = _P._Mynode();
  //   _Nodeptr _S = _Buynode(_Ptr, _Acc::_Prev(_Ptr));
  //   _S->_Value = _X;
  //   _Acc::_Prev(_Ptr) = _S;
  //   _Acc::_Next(_Acc::_Prev(_S)) = _S;
  //   // _Acc::_Value(_X);
  //   _Size++;
  //   return iterator(_S);
  // }
  iterator insert(iterator _P, const _Ty& _X = _Ty()) {
    _Nodeptr _S = _P._Mynode();
    // _Ptr->_Value = _X;
    _Acc::_Prev(_S) = _Buynode(_S, _Acc::_Prev(_S));
    _S = _Acc::_Prev(_S);
    _Acc::_Next(_Acc::_Prev(_S)) = _S;
    _S->_Value = _X;

    _Size++;
    return iterator(_S);
  }
  // 多样性插入
  // 区间插入
  void insert(iterator _P, size_type _M, const _Ty& _X) {
    for (; 0 < _M; --_M) {
      insert(_P, _X);
    }
  }

  // 范围插入
  typedef const_iterator _It;
  void insert(iterator _P, _It _F, _It _L) {
    for (; _F != _L; ++_F) {
      insert(_P, *_F);
    }
  }

  iterator erase(iterator _P) {
    _Nodeptr _S = (_P++)._Mynode();
    _Acc::_Next(_Acc::_Prev(_S)) = _Acc::_Next(_S);
    _Acc::_Prev(_Acc::_Next(_S)) = _Acc::_Prev(_S);
    ///////////////////////////////////
    free(_S);
    //////////////////////////////////
    --_Size;
    return _P;
  }

  // 区间删除
  iterator erase(iterator _F, iterator _L) {
    while (_F != _L) {
      erase(_F++);
    }
    return _F;
  }

 protected:
  // _Narg后继参数 _Parg前继参数
  _Nodeptr _Buynode(_Nodeptr _Narg = 0, _Nodeptr _Parg = 0) {
    _Nodeptr _S = (_Nodeptr)malloc(sizeof(_Node));
    assert(_S != NULL);
    _Acc::_Next(_S) = _Narg != 0 ? _Narg : _S;
    _Acc::_Prev(_S) = _Parg != 0 ? _Parg : _S;
    return _S;
  }

 private:
  _Nodeptr _Head;
  size_type _Size;
};
