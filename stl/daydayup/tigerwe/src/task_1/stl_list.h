#include "stl_def.h"

/** @file stl_list.h
 *
 *  This is an stl_list header file, implement double loop list warppes
 *
 *  Created by yejy on 18-8-18
 *  copyright (c) yejy. all rights reserved
 *
 */

__YAMI_BEGIN

/* stl list  allocate 直接使用默认new/delete */
template <typename T>
class list {
 public:
  // 不包数据实体，只包含指针和相关操作, 可以认为是节省一个指针大小的内存
  struct list_node_base {
    list_node_base* Next;
    list_node_base* Prev;

    list_node_base() : Next(nullptr), Prev(nullptr) {}
  };

  // dataEntry node
  struct list_node : public list_node_base {
    T dataEntry;
  };

  // 迭代器 iterator
  struct list_iterator {
    typedef list_iterator _Self;
    typedef T value_type;
    typedef T* pointer;
    typedef T& reference;

    list_iterator() _T_STD_NOEXCEPT { m_smartPtr = nullptr; }

    explicit list_iterator(list_node_base* pNode) _T_STD_NOEXCEPT { m_smartPtr = pNode; }

    reference operator*() _T_STD_NOEXCEPT {
      return static_cast<list_node*>(m_smartPtr)->dataEntry;
    }

    list_node_base* operator->() _T_STD_NOEXCEPT { return m_smartPtr; }

    _Self operator++(int) _T_STD_NOEXCEPT  // 后 ++
    {
      _Self __tmp = *this;
      m_smartPtr = m_smartPtr->Next;
      return __tmp;
    }

    _Self& operator++() _T_STD_NOEXCEPT  // 前 ++
    {
      m_smartPtr = m_smartPtr->Next;
      return *this;
    }

    _Self operator--(int) _T_STD_NOEXCEPT {
      _Self __tmp = *this;
      m_smartPtr = m_smartPtr->Prev;
      return __tmp;
    }

    _Self& operator--() _T_STD_NOEXCEPT {
      m_smartPtr = m_smartPtr->Prev;
      return *this;
    }

    bool operator==(const list_iterator& _Right) const _T_STD_NOEXCEPT {
      return m_smartPtr == _Right.m_smartPtr;
    }

    bool operator!=(const list_iterator& _Right) const _T_STD_NOEXCEPT {
      return m_smartPtr != _Right.m_smartPtr;
    }

    list_node_base* m_smartPtr;  // 节点指针
  };

 public:
  typedef list_iterator iterator;

 public:
  list()  // 默认构造
  {
    empty_init();
  }

  list(const list<T>& rhs)  // 拷贝构造
  {
    if (this != &rhs) {
      empty_init();  // 初始化

      iterator itrBegin = rhs.begin();
      iterator itrEnd = rhs.end();

      while (itrBegin != itrEnd) {
        list_node* tmp = static_cast<list_node*>(itrBegin.m_smartPtr);

        push_back(tmp->dataEntry);

        ++itrBegin;
      }
    }
  }

  list& operator=(const list<T>& rhs)  // 赋值运算符重载
  {
    if (this != &rhs) {
      // 如果原来链表有值，则先清空
      if (begin() != end()) {
        clear();
      }

      iterator itrBegin = rhs.begin();
      iterator itrEnd = rhs.end();

      while (itrBegin != itrEnd) {
        list_node* tmp = static_cast<list_node*>(itrBegin.m_smartPtr);

        push_back(tmp->dataEntry);

        ++itrBegin;
      }
    }
  }

  ~list() {
    clear();

    if (pHeadNode) {
      delete pHeadNode;
      pHeadNode = nullptr;
    }
  }

  iterator begin() _T_STD_NOEXCEPT { return iterator(pHeadNode->Next); }

  iterator end() _T_STD_NOEXCEPT { return iterator(pHeadNode); }

  void push_back(const T& value) { insert(end(), value); }

  void push_front(const T& value) { insert(begin(), value); }

  void pop_front() { erase(begin()); }

  void pop_back() {
    iterator tmp = end();
    erase(--tmp);
  }

  T& front() { return *begin(); }

  T& back() { return *(--end()); }

  unsigned int remove(const T& value) {
    unsigned int count = 0;

    iterator itrBegin = begin();
    while (itrBegin != end()) {
      if (*itrBegin == value) {
        itrBegin = erase(itrBegin);
        ++count;
      } else {
        ++itrBegin;
      }
    }

    return count;
  }

  iterator erase(iterator position) {
    list_node_base* next_node = position.m_smartPtr->Next;
    list_node_base* prev_node = position.m_smartPtr->Prev;
    prev_node->Next = next_node;
    next_node->Prev = prev_node;

    delete position.m_smartPtr;
    position.m_smartPtr = nullptr;

    if (_size > 0) {
      _size--;
    }

    return iterator(next_node);
  }

  iterator insert(iterator position, const T& x) {
    list_node* tmp = new list_node();
    tmp->dataEntry = x;
    tmp->Next = position.m_smartPtr;
    tmp->Prev = position.m_smartPtr->Prev;
    position.m_smartPtr->Prev->Next = tmp;
    position.m_smartPtr->Prev = tmp;

    ++_size;
    return iterator(tmp);
  }

  void clear() {
    iterator itrBegin = begin();
    while (itrBegin != end()) {
      list_node* tmp = static_cast<list_node*>(itrBegin.m_smartPtr);

      ++itrBegin;

      if (tmp) {
        delete tmp;  // 差点犯了一个错误，delete会对用析构函数，并且释放内存。
                     // 需要析构子类还是父类，一定要传入正确类型
      }
    }

    pHeadNode->Next = pHeadNode;
    pHeadNode->Prev = pHeadNode;
    _size = 0;
  }

  int size() { return _size; }

 private:
  void empty_init() {
    pHeadNode = new list_node_base();
    pHeadNode->Next = pHeadNode;  // 初始化指针指向自己
    pHeadNode->Prev = pHeadNode;

    _size = 0;
  }

 private:
  list_node_base* pHeadNode;  // 链表头

  unsigned int _size;  // 链表个数，提高查找效率，如果想为了节省内存，可以不要，临时查找
};

__YAMI_END
