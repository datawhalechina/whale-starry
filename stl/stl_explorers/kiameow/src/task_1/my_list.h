#ifndef MYLIST_H_
#define MYLIST_H_

#include <iostream>

template <typename Tp>
struct MyListNode {
  MyListNode* prev;
  MyListNode* next;
  Tp value;
  int pos;
};

template <typename Tp>
struct MyListIterator {
  typedef MyListIterator<Tp> Self;

  MyListNode<Tp>* it;
  int pos;
  MyListIterator() : it() {}

  explicit MyListIterator(MyListNode<Tp>* _xnode) : it(_xnode), pos(_xnode->pos) {}

  Self& operator+=(int x) {
    for (int i = 0; i < x; ++i) {
      it = it->next;
      ++pos;
    }
    return *this;
  }

  Self& operator-=(int x) {
    for (int i = 0; i < x; ++i) {
      it = it->prev;
      --pos;
    }
    return *this;
  }

  Tp operator*() { return it->value; }

  friend bool operator<(const Self __x, const Self __y) { return __x.pos < __y.pos; }

  friend bool operator>(const Self __x, const Self __y) { return __x.pos > __y.pos; }

  friend bool operator<=(const Self __x, const Self __y) { return __x.pos <= __y.pos; }

  friend bool operator>=(const Self __x, const Self __y) { return __x.pos >= __y.pos; }

  friend bool operator==(const Self __x, const Self __y) { return __x.it == __y.it; }

  friend bool operator!=(const Self __x, const Self __y) { return __x.it != __y.it; }
};
#endif
