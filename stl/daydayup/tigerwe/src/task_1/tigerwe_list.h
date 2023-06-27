#ifndef _TIGERWE_LIST_H
#define _TIGERWE_LIST_H

#include <iostream>

// 不包数据实体，只包含指针和相关操作, 可以认为是节省一个指针大小的内存
struct list_node_base {
  list_node_base* Next;
  list_node_base* Prev;

  list_node_base() : Next(nullptr), Prev(nullptr) {}
};

// Data node
template <typename T>
struct list_node : public list_node_base {
  T Data;
};

// 迭代器 iterator
template <typename T>
struct list_iterator {
  typedef list_iterator<T> _Self;

  typedef ptrdiff_t difference_type;
  typedef T value_type;
  typedef T* pointer;
  typedef T& reference;

  list_iterator() noexcept : m_node() {}

  explicit list_iterator(list_node_base* pNode) noexcept : m_node(pNode) {}

  reference operator*() noexcept { return static_cast<list_node<T>*>(m_node)->Data; }

  pointer operator->() noexcept { return m_node; }
  // 返回的是对象
  _Self& operator++() noexcept  // 前 ++
  {
    m_node = m_node->Next;
    return *this;
  }
  // 返回的是引用
  _Self operator++(int) noexcept  // 后 ++
  {
    _Self __tmp = *this;
    m_node = m_node->Next;
    return __tmp;
  }

  // 同理
  _Self operator--(int) noexcept {
    _Self __tmp = *this;
    m_node = m_node->Prev;
    return __tmp;
  }

  _Self& operator--() noexcept {
    m_node = m_node->Prev;
    return *this;
  }

  bool operator==(const list_iterator& _Right) const noexcept {
    return m_node == _Right.m_node;
  }

  bool operator!=(const list_iterator& _Right) const noexcept {
    return m_node != _Right.m_node;
  }

  list_node_base* m_node;  // 节点指针
};

/* stl list  allocate 直接使用默认new/delete */
template <typename T>
class list {
 public:
  typedef list_iterator<T> iterator;

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
        list_node<T>* tmp = static_cast<list_node<T>*>(itrBegin.m_node);

        push_back(tmp->Data);

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
        list_node<T>* tmp = static_cast<list_node<T>*>(itrBegin.m_node);

        push_back(tmp->Data);

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

  iterator begin() noexcept { return iterator(pHeadNode->Next); }

  iterator end() noexcept { return iterator(pHeadNode); }

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
    list_node_base* next_node = position.m_node->Next;
    list_node_base* prev_node = position.m_node->Prev;
    prev_node->Next = next_node;
    next_node->Prev = prev_node;

    delete position.m_node;
    position.m_node = nullptr;

    if (_size > 0) {
      _size--;
    }

    return iterator(next_node);
  }

  iterator insert(iterator position, const T& x) {
    list_node<T>* tmp = new list_node<T>();
    tmp->Data = x;
    tmp->Next = position.m_node;
    tmp->Prev = position.m_node->Prev;
    position.m_node->Prev->Next = tmp;
    position.m_node->Prev = tmp;

    ++_size;
    return iterator(tmp);
  }

  void clear() {
    iterator itrBegin = begin();
    while (itrBegin != end()) {
      list_node<T>* tmp = static_cast<list_node<T>*>(itrBegin.m_node);

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

  unsigned int _size;  // 链表个数
};

#endif