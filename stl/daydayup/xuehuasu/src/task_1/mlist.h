#include <iostream>

// 节点基类，定义了节点的指针
struct List_node_base {
  List_node_base* next;
  List_node_base* prev;
  List_node_base() {}
  List_node_base(List_node_base* x) {
    next = x->next;
    prev = x->prev;
  }

  // 在pos的位置插入this
  void m_hook(List_node_base* position) {
    this->next = position;
    this->prev = position->prev;
    position->prev->next = this;
    position->prev = this;
  }

  // 删除this
  void m_uhook() {
    this->next->prev = this->prev;
    this->prev->next = this->next;
  }
  // 将连续的[begin,end)插入在this之前
  void m_transfer(List_node_base* begin, List_node_base* end) {
    List_node_base* this_prev = this->prev;

    this->prev = end->prev;
    end->prev->next = this;
    end->prev = begin->prev;

    this_prev->next = begin;
    begin->prev->next = end;
    begin->prev = this_prev;
  }
};

// 节点类，定义数据部分
template <typename _Tp>
struct List_node : public List_node_base {
  _Tp data;
  List_node() {}
  List_node(_Tp x) { data = x; }
  _Tp* val() { return &data; }
  _Tp const* val() const { return &data; }
};

// 定义迭代器
template <typename _Tp>
struct List_iterator {
  typedef List_iterator<_Tp> Self;
  typedef _Tp value_type;
  typedef _Tp* pointer;
  typedef _Tp& reference;
  typedef List_node<_Tp> node;
  typedef List_node_base basenode;

  basenode* cur;  // 当前指向指针

  List_iterator() { cur = 0; }

  List_iterator(node* pnode) { cur = static_cast<basenode*>(pnode); }
  List_iterator(basenode* pnode) { cur = pnode; }

  reference operator*() const { return *static_cast<node*>(cur)->val(); }
  pointer operator->() const { return static_cast<node*>(cur)->val(); }

  // ++i
  Self& operator++() {
    cur = cur->next;
    return *this;
  }
  // i++
  Self operator++(int) {
    Self tmp = *this;
    cur = cur->next;
    return tmp;
  }

  // --i
  Self& operator--() {
    cur = cur->prev;
    return *this;
  }
  // i--
  Self operator--(int) {
    Self tmp = *this;
    cur = cur->prev;
    return tmp;
  }

  bool operator==(const List_iterator& _Right) const { return cur == _Right.cur; }

  bool operator!=(const List_iterator& _Right) const { return cur != _Right.cur; }
};

template <typename _Tp>
struct List {
  typedef List<_Tp> list;
  typedef List_iterator<_Tp> iterator;
  typedef List_node<_Tp> node;
  typedef List_node_base basenode;

  // 空构造
  List() { init(); }

  // 从范围构造
  List(const iterator& beg, const iterator& end) {
    init();
    for (iterator it = beg; it != end; it++) {
      this->push_back(*it);
    }
  }

  ~List() {
    while (!empty()) {
      // std::cout << "delele: " << *(begin()) << ' ';
      erase(begin());
    }
    delete (this->head);
    // std::cout << "~list end" << std::endl;
  }

  void push_back(const _Tp& value) { insert(end(), value); }

  void push_front(const _Tp& value) { insert(begin(), value); }

  void pop_front() { erase(begin()); }

  void pop_back() {
    iterator tmp = end();
    erase(--tmp);
  }

  iterator insert(const iterator& position, const _Tp& x) {
    node* tmp = new node(x);
    tmp->m_hook(position.cur);

    return iterator(tmp);
  }

  // 在pos位置之前插入一串
  void insert(iterator position, const iterator& begin, const iterator& end) {
    if (begin == end) return;
    list list_tmp(begin, end);
    position.cur->m_transfer(list_tmp.begin().cur, list_tmp.end().cur);
  }

  iterator erase(iterator position) {
    basenode* next_node = position.cur->next;
    position.cur->m_uhook();
    delete position.cur;
    position.cur = 0;
    return iterator(next_node);
  }

  iterator begin() { return iterator(head->next); }

  iterator end() { return iterator(head); }

  bool empty() { return begin() == end(); }

  void init() {
    head = new List_node_base();
    head->next = head;
    head->prev = head;
  }

  List_node_base* head;
};