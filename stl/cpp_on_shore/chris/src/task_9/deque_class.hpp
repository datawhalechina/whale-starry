/*
 Since deque structure is to hard for me to design entirely by myself,
 therefore I refer to the stl design method and utilize it
 in my deque structure.
*/
#ifndef DequeIterator_
#define DequeIterator_
#include <algorithm>
#include <iostream>
#define BUFFER_SIZE 10

namespace MyDeque {
template <typename T>
class DequeIterator {
 public:
  typedef DequeIterator iterator;
  typedef const DequeIterator const_iterator;
  typedef std::random_access_iterator_tag iterator_category;
  // iterator tag.
  typedef T value_type;
  typedef T* pointer;
  typedef T& reference;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef T** map_pointer;
  typedef DequeIterator self;

  T* cur;            // pointer, pointing to the current element.
  T* first;          // pointer, pointing to the first element in current buffer.
  T* last;           // pointer, pointing to the last element in current buffer.
  map_pointer node;  // pointer, pointing to the buffer.

  static size_t buffer_size() { return BUFFER_SIZE; }

  DequeIterator(T* x, map_pointer y)
      : cur(x), first(*y), last(*y + buffer_size()), node(y) {}
  DequeIterator() : cur(0), first(0), last(0), node(0) {}
  DequeIterator(const iterator& x)
      : cur(x.cur), first(x.first), last(x.last), node(x.node) {}

  reference operator*() const { return *cur; }
  reference operator->() const { return cur; }
  bool operator==(const self& x) const { return cur == x.cur; }
  bool operator!=(const self& x) const { return !(*this == x); }
  bool operator<(const self& x) const {
    return (node == x.node) ? (cur < x.cur) : (node < x.node);
  }
  void set_node(map_pointer new_node) {
    node = new_node;
    first = *new_node;
    last = first + buffer_size();
  }

  difference_type operator-(const self& x) const {
    return difference_type(buffer_size()) * (node - x.node - 1) + (cur - first) +
           (x.last - x.cur);
  }

  self& operator++() {
    ++cur;
    if (cur == last) {
      set_node(node + 1);
      cur = first;
    }
    return *this;
  }

  self operator++(int) {
    self temp = *this;
    ++*this;
    return temp;
  }

  self& operator--() {
    if (cur == first) {
      set_node(node - 1);
      cur = last;
    }
    --cur;
    return *this;
  }

  self operator--(int) {
    self temp = *this;
    --*this;
    return temp;
  }

  self& operator+=(difference_type n) {
    difference_type offset = n + (cur - first);
    if (offset >= 0 && offset < difference_type(buffer_size())) {
      cur += n;
      // The destination is in the same buffer.
    } else {
      // The destination is in the different buffer.
      difference_type node_offset =
          offset > 0 ? offset / difference_type(buffer_size())
                     : -difference_type((-offset - 1) / buffer_size()) - 1;
      // transfer to the right buffer node
      set_node(node + node_offset);
      // transfer to the right node
      cur = first + (offset - node_offset * difference_type(buffer_size()));
    }
    return *this;
  }
  self& operator-=(difference_type n) { return *this += -n; }
  self operator+(difference_type n) const {
    self tmp = *this;
    return tmp += n;
  }
  self operator-(difference_type n) const {
    self tmp = *this;
    return tmp -= n;
  }

  reference operator[](difference_type n) const { return *(*this + n); }
};
#endif /* DequeIterator */
#ifndef Deque
#define Deque
#include <algorithm>
#include <new>
#define MAP_SIZE 20
// we set the size of map is 20
template <typename T>
class deque {
 public:
  typedef T value_type;
  typedef value_type* pointer;
  typedef value_type& reference;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef std::allocator<value_type> d_allocator;
  typedef std::allocator<pointer> m_allocator;
  typedef DequeIterator<T> iterator;
  typedef pointer* map_pointer;
  m_allocator map_allocator;
  d_allocator data_allocator;

 protected:
  static size_type buffer_size() { return size_t(BUFFER_SIZE); }
  static size_type initial_map_size() { return 8; }
  size_type max_size() const { return size_type(-1); }

  pointer allocate_node() { return data_allocator.allocate(buffer_size()); }

  void deallocate_node(pointer n) { data_allocator.deallocate(n, buffer_size()); }

  void create_map_and_nodes(size_type num_elements) {
    size_type num_nodes = num_elements / BUFFER_SIZE + 1;
    map_size = std::max(initial_map_size(), num_nodes + 2);
    //  begin and last will be allocate more memory
    //  to save time for inserting elements in deque.
    map = map_allocator.allocate(map_size);
    //  set start and finish node in the middle of map.
    map_pointer nstart = map + (map_size - num_nodes) / 2;
    map_pointer nfinish = nstart + num_nodes - 1;
    for (map_pointer cur = nstart; cur <= nfinish; cur++) {
      *cur = allocate_node();
    }
    start.set_node(nstart);
    finish.set_node(nfinish);
    start.cur = start.first;
    finish.cur = finish.first + num_elements % BUFFER_SIZE;
  }

  void reallocate_map(size_type nodes_to_add, bool add_at_front) {
    size_type old_num_nodes = finish.node - start.node + 1;
    size_type new_num_nodes = old_num_nodes + nodes_to_add;
    map_pointer new_nstart;
    if (map_size > 2 * new_num_nodes) {
      //  to balance map size.
      //  just to avoid map size being off balance.
      new_nstart =
          map + (map_size - new_num_nodes) / 2 + (add_at_front ? nodes_to_add : 0);
      if (new_nstart < start.node)
        std::copy(start.node, finish.node + 1, new_nstart);
      else
        std::copy_backward(start.node, finish.node + 1, new_nstart + old_num_nodes);
    } else {
      size_type new_map_size = map_size + std::max(map_size, nodes_to_add) + 2;
      //  allocate new memory for new map.
      map_pointer new_map = map_allocator.allocate(new_map_size);
      new_nstart = new_map + (new_map_size - new_num_nodes) / 2 +
                   (add_at_front ? nodes_to_add : 0);
      //  copy original map.
      std::copy(start.node, finish.node + 1, new_nstart);
      //  reallocate original map.
      map_allocator.deallocate(map, map_size);
      //  set new map and corresponding size.
      map = new_map;
      map_size = new_map_size;
    }
    //  reset start and finish node.
    start.set_node(new_nstart);
    finish.set_node(new_nstart + old_num_nodes - 1);
  }

 protected:
  iterator start;
  iterator finish;
  map_pointer map;
  size_type map_size;

 public:
  deque() : start(), finish(), map(0) { create_map_and_nodes(0); }

  ~deque() {
    for (map_pointer node = start.node; node <= finish.node; ++node) {
      //  free memory of buffer.
      delete *node;
    }
    destroy_map_and_nodes();
  }

  void destroy_map_and_nodes() { map_allocator.deallocate(map, map_size); }

  iterator begin() { return start; }
  iterator end() { return finish; }
  reference front() { return *start; }
  reference back() {
    iterator temp = finish;
    --temp;
    return *temp;
  }

  reference operator[](size_type n) { return start[difference_type(n)]; }
  size_type size() const { return finish - start; }
  bool empty() const { return finish == start; }

  void push_back(const value_type& t) {
    //  check if there is the enough memory to insert element
    if (finish.cur != finish.last - 1) {
      data_allocator.construct(finish.cur, t);
      //  directly constructor.
      ++finish.cur;
      //  adjust the finish map iterator.
    } else {
      //  the special push back version.
      push_back_aux(t);
    }
  }

  void push_back_aux(const value_type& t) {
    value_type t_copy = t;
    reserve_map_at_back();
    *(finish.node + 1) = allocate_node();
    //  allocate new node buffer.
    map_allocator.construct(finish.cur, t_copy);
    //  constructe.
    finish.set_node(finish.node + 1);
    //  change finish iterator to point to new node.
    finish.cur = finish.first;
    //  set finish state.
  }

  void reserve_map_at_back(size_type nodes_to_add = 1) {
    if (nodes_to_add > map_size - (finish.node - map) - 1)
      //  if the size of back deque
      //  reallocate map at back.
      reallocate_map(nodes_to_add, false);
  }

  void push_front(const value_type& t) {
    if (start.cur != start.first) {
      //  check if there is enough memory to allocate new element.
      data_allocator.construct(start.cur - 1, t);
      --start.cur;
    } else
      //  if there isn't enough memory to insert new element
      //  call the special push front version.
      push_front_aux(t);
  }

  void push_front_aux(const value_type& t) {
    value_type t_copy = t;
    reserve_map_at_front();
    *(start.node - 1) = allocate_node();
    start.set_node(start.node - 1);
    start.cur = start.last - 1;
    data_allocator.construct(start.cur, t_copy);
  }
  void reserve_map_at_front(size_type nodes_to_add = 1) {
    if (nodes_to_add > start.node - map) reallocate_map(nodes_to_add, true);
  }

  void pop_back() {
    if (!empty()) {
      if (finish.cur != finish.first) {
        data_allocator.destroy(finish.cur);
        --finish.cur;
      } else
        pop_back_aux();
    }
  }

  void pop_back_aux() {
    deallocate_node(finish.first);  // reallocate tha last buffer.
    finish.set_node(finish.node - 1);
    finish.cur = finish.last - 1;
    data_allocator.destroy(finish.cur);  // deconstructor element
  }

  void pop_front() {
    if (!empty()) {
      if (start.cur != start.last - 1) {
        data_allocator.destroy(start.cur);
        ++start.cur;
      } else

        pop_front_aux();
    }
  }
  void pop_front_aux() {
    data_allocator.destroy(start.cur);
    deallocate_node(start.first);  //  free the first map iterator.
    start.set_node(start.node + 1);
    start.cur = start.first;
  }

  iterator insert(iterator position, const value_type& value = value_type()) {
    if (position == start) {
      push_front(value);
      return start;
    } else if (position == finish) {
      push_front(value);
      return finish;
    } else {
      return insert_aux(position, value);
    }
  }

  iterator insert_aux(iterator pos, const value_type& x) {
    difference_type index = pos - start;  // 插入点之前的元素个数
    value_type x_copy = x;
    if (index < size() / 2)  // 如果插入点之前的元素个数比较少
    {
      push_front(front());      // 在最前端加入与第一元素同值的元素
      iterator front1 = start;  // 以下标示记号，然后进行元素移动
      ++front1;
      iterator front2 = front1;
      ++front2;
      pos = start + index;
      iterator pos1 = pos;
      ++pos1;
      std::copy(front2, pos1, front1);  // 元素移动
    } else                              // 插入点之后的元素个数比较少
    {
      push_back(back());        // 在最尾端加入与最后元素同值的元素
      iterator back1 = finish;  // 以下标示记号，然后进行元素移动
      --back1;
      iterator back2 = back1;
      --back2;
      pos = start + index;
      std::copy_backward(pos, back2, back1);  // 元素移动
    }
    *pos = x_copy;  // 在插入点上设定新值
    return pos;
  }

  iterator erase(iterator pos) {
    iterator next = pos;
    ++next;
    difference_type index = pos - start;
    //  check operate at which side will cost less price.
    if (index < (size() >> 1)) {
      std::copy_backward(start, pos, next);
      pop_front();
    } else {
      std::copy(next, finish, pos);
      pop_back();
    }
    return start + index;
  }

  iterator erase(iterator first, iterator last) {
    if (first == start && last == finish) {
      //  if erasing the whole deque.
      //  just clear.
      clear();
      return finish;
    } else {
      difference_type n = last - first;
      difference_type elems_before = first - start;
      if (elems_before < (size() - n) / 2) {
        std::copy_backward(start, first,
                           last);  // 向后移动前方元素（覆盖清除区间）
        iterator new_start = start + n;  // 标记deque的新起点
        destroy(start, new_start);       // 移动完毕，将冗余的元素析构
        // 以下将冗余的缓冲区释放
        for (map_pointer cur = start.node; cur < new_start.node; ++cur)
          data_allocator.deallocate(*cur, buffer_size());
        start = new_start;  // 设定deque的新起点
      } else                // 如果清除区间后方的元素个数比较少
      {
        std::copy(last, finish, first);  // 向前移动后方元素（覆盖清除区间）
        iterator new_finish = finish - n;  // 标记deque的新尾点
        destroy(new_finish, finish);       // 移动完毕，将冗余的元素析构
        // 以下将冗余的缓冲区释放
        for (map_pointer cur = new_finish.node + 1; cur <= finish.node; ++cur)
          data_allocator.deallocate(*cur, buffer_size());
        finish = new_finish;  // 设定deque的新尾点
      }
      return start + elems_before;
    }
  }

  void clear() {
    // 以下针对头尾以外的每一个缓冲区
    for (map_pointer node = start.node + 1; node < finish.node; ++node) {
      // 将缓冲区内的所有元素析构
      for (int i = 0; i != buffer_size() + 1; i++) {
        data_allocator.destroy(*node + i);
      }
      // 释放缓冲区内存
      data_allocator.deallocate(*node, buffer_size());
    }

    if (start.node != finish.node)  // 至少有头尾两个缓冲区
    {
      for (auto iter = start.cur; iter != start.last; iter++) {
        data_allocator.destroy(iter);
      }
      for (auto iter = finish.first; iter != finish.cur; iter++) {
        data_allocator.destroy(iter);
      }
      // 以下释放尾缓冲区。注意：头缓冲区保留
      data_allocator.deallocate(finish.first, buffer_size());
    } else  // 只有一个缓冲区
      for (auto iter = start.cur; iter != finish.cur; iter++) {
        data_allocator.destroy(iter);  // 将此唯一缓冲区内的所有元素析构
      }
    // 注意：并不释放缓冲区空间，这唯一的缓冲区将保留
    finish = start;  // 调整状态
  }
};
#endif /* Deque_h */
}