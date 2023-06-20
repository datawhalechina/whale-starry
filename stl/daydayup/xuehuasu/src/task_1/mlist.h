#include <iostream>

struct List_node_base {
  List_node_base* next;
  List_node_base* prev;
  List_node_base() {}
  List_node_base(List_node_base* x) {
    next = x->next;
    prev = x->prev;
  }
};

template <typename _Tp>
struct List_node : public List_node_base {
  _Tp data;
  _Tp* val() { return &data; }
  _Tp const* val() const { return &data; }
};

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
  typedef List_iterator<_Tp> iterator;
  typedef List_node<_Tp> node;
  typedef List_node_base basenode;

  List() { init(); }
  List(const List<_Tp>& t) {
    iterator begin = t.begin();
    iterator end = t.end();
    while (begin != end) {
      node* temp = static_cast<node*>(begin.node);
      this->push_back(temp.data);
      begin++;
    }
  }
  ~List() {
    iterator begin = this->begin();
    iterator end = this->end();
    while (iterator it = begin; it != end;) {
      begin++;
      erase(it);
      it = begin;
    }
    delete (this->head);
  }

  void push_back(const _Tp& value) { insert(end(), value); }
  void push_front(const _Tp& value) { insert(begin(), value); }
  void pop_front() { erase(begin()); }

  void pop_back() {
    iterator tmp = end();
    erase(--tmp);
  }

  iterator insert(iterator position, const _Tp& x) {
    node* tmp = new node();
    tmp->data = x;
    tmp->next = position.cur;
    tmp->prev = position.cur->prev;
    position.cur->prev->next = tmp;
    position.cur->prev = tmp;

    return iterator(tmp);
  }

  iterator erase(iterator position) {
    basenode* next_node = position.cur->next;
    basenode* prev_node = position.cur->prev;
    prev_node->next = next_node;
    next_node->prev = prev_node;

    delete position.cur;
    position.cur = 0;

    return iterator(next_node);
  }

  iterator begin() { return iterator(head->next); }

  iterator end() { return iterator(head); }

  void init() {
    head = new List_node_base();
    head->next = head;
    head->prev = head;
  }

  List_node_base* head;
};