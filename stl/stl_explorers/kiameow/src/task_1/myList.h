#ifndef MYLIST_H
#define MYLIST_H
#include <iostream>

template <typename Tp>
struct myList_node {
  myList_node* prev;
  myList_node* next;
  Tp value;
  int pos;
};

template <typename Tp>
struct myList_iterator {
  typedef myList_iterator<Tp> _Self;

  myList_node<Tp>* it;
  int pos;
  myList_iterator() : it() {}

  explicit myList_iterator(myList_node<Tp>* _xnode) : it(_xnode), pos(_xnode->pos) {}

  _Self& operator+=(int x) {
    for (int i = 0; i < x; ++i) {
      it = it->next;
      ++pos;
    }
    return *this;
  }

  _Self& operator-=(int x) {
    for (int i = 0; i < x; ++i) {
      it = it->prev;
      --pos;
    }
    return *this;
  }

  Tp operator*() { return it->value; }

  friend bool operator<(const _Self __x, const _Self __y) { return __x.pos < __y.pos; }

  friend bool operator>(const _Self __x, const _Self __y) { return __x.pos > __y.pos; }

  friend bool operator<=(const _Self __x, const _Self __y) { return __x.pos <= __y.pos; }

  friend bool operator>=(const _Self __x, const _Self __y) { return __x.pos >= __y.pos; }

  friend bool operator==(const _Self __x, const _Self __y) { return __x.it == __y.it; }

  friend bool operator!=(const _Self __x, const _Self __y) { return __x.it != __y.it; }
};
#endif
