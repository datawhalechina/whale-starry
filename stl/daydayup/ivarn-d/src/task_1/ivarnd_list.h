#ifndef _IVARND_LIST_H_
#define _IVARND_LIST_H_

#include <assert.h>

#include <algorithm>

namespace ivarnd {
template <class T>
struct __list_node {
  __list_node<T>* _next;
  __list_node<T>* _prev;
  T _data;
  __list_node(const T& x = T()) : _next(nullptr), _prev(nullptr), _data(x) {}
};
template <class T, class Ref, class Ptr>
struct __list_iterator {
  typedef __list_node<T> node;
  typedef __list_iterator<T, Ref, Ptr> self;
  node* _node;
  __list_iterator(node* n) : _node(n) {}
  Ref operator*() { return _node->_data; }
  Ptr operator->() { return &_node->_data; }
  self& operator++() {
    _node = _node->_next;
    return *this;
  }
  self operator++(int) {
    self tmp(*this);
    _node = _node->_next;
    return tmp;
  }
  self& operator--() {
    _node = _node->_prev;
    return *this;
  }
  self operator--(int) {
    self tmp(*this);
    _node = _node->_prev;
    return tmp;
  }
  bool operator!=(const self& s) { return _node != s._node; }
  bool operator==(const self& s) { return _node == s._node; }
};
template <class T>
class list {
  typedef __list_node<T> node;

 public:
  typedef __list_iterator<T, T&, T*> iterator;
  typedef __list_iterator<T, const T&, const T*> const_iterator;
  const_iterator begin() const { return const_iterator(_head->_next); }
  const_iterator end() const { return const_iterator(_head); }
  iterator begin() { return iterator(_head->_next); }
  iterator end() { return iterator(_head); }
  void empty_initialize() {
    _head = new node;
    _head->_next = _head;
    _head->_prev = _head;
  }
  list() { empty_initialize(); }
  template <class Iterator>
  list(Iterator first, Iterator last) {
    empty_initialize();

    while (first != last) {
      push_back(*first);
      ++first;
    }
  }
  void swap(list<T>& l) { std::swap(_head, l._head); }
  list(const list<T>& l) {
    empty_initialize();
    list<T> tmp(l.begin(), l.end());
    swap(tmp);
  }
  list<T>& operator=(list<T> l) {
    swap(l);
    return *this;
  }
  ~list() {
    clear();
    delete _head;
    _head = nullptr;
  }
  void clear() {
    iterator it = begin();
    while (it != end()) {
      erase(it++);
    }
  }
  void push_back(const T& x) { insert(end(), x); }
  void push_front(const T& x) { insert(begin(), x); }
  void insert(iterator pos, const T& x) {
    node* cur = pos._node;
    node* prev = cur->_prev;

    node* newnode = new node(x);

    prev->_next = newnode;
    newnode->_prev = prev;
    newnode->_next = cur;
    cur->_prev = newnode;
  }
  iterator erase(iterator pos) {
    assert(pos != end());
    node* prev = pos._node->_prev;
    node* next = pos._node->_next;

    prev->_next = next;
    next->_prev = prev;
    delete pos._node;

    return iterator(next);
  }
  void pop_back() { erase(--end()); }
  void pop_front() { erase(begin()); }
  void print_list(const list<T>& l) {
    for (list<T>::const_iterator it = l.begin(); it != l.end(); ++it) {
      std::cout << *it << " ";
    }
    std::cout << std::endl;
  }

 private:
  node* _head;
};
}  // namespace ivarnd

#endif  // _IVARND_LIST_H_
