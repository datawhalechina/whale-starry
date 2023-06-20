#include <cstdint>

#include "tiny_alloc.h"
#include "tiny_iterator.h"

namespace __tiny_stl {
using size_type = uint64_t;

struct ListNodeBase {
  ListNodeBase* next_ = nullptr;
  ListNodeBase* prev_ = nullptr;

  // TODO: member function like swap、hook、unhook...
  // 在指定位置position前插入，this为要插入的节点
  void hook(ListNodeBase* const position) noexcept {
    this->next_ = position;
    this->prev_ = position->prev_;
    this->prev_->next_ = this;
    position->prev_ = this;
  }

  // 将当前节点从链表中删除
  void unhook() noexcept {
    this->prev_->next_ = this->next_;
    this->next_->prev_ = this->prev_;
  }
};

template <typename ValueType>
struct ListNode : public ListNodeBase {
  ValueType data_;  // user's data
};

// ⭐ ListBase中的Alloc是不确定的，但是List中会指定默认参数
// 且List中的Alloc就是为了父类而定义的
// ❗其中Alloc是一个模板类型参数
template <typename ValueType, typename Alloc>
class ListBase {
 protected:
  // TODO: consider alloctor
  using node_alloc_t = typename Alloc::rebind<ListNode<ValueType>>::other;

  struct ListImpl : public node_alloc_t {
    ListNodeBase node_;

    // construct function
    ListImpl() : node_() {}
    ListImpl(const node_alloc_t& a) noexcept : node_alloc_t(a), node_() {}
  };

  ListImpl impl_;

  using node_t = ListNode<ValueType>;
  node_t* get_node() {
    // 调用impl_的父类的方法，申请空间 <==> malloc调用
    return impl_.node_alloc_t::allocate();
  }

  // 调用impl_的父类的方法，申请空间 <==> free调用
  void put_node(node_t* p) { impl_.node_alloc_t::deallocate(p); }

 public:
  // 普通构造函数
  ListBase() : impl_() { init(); };
  ListBase(const node_alloc_t& a) noexcept : impl_(a) { init(); }

  // TODO: move constructor, need to implement functions to get allocte
  // to simple the code, we do not implement that

  ~ListBase() noexcept { clear(); }

  // TODO: 析构函数
  void clear() noexcept {}

  void init() {
    this->impl_.node_.next_ = &this->impl_.node_;
    this->impl_.node_.prev_ = &this->impl_.node_;
  }
};

// list iterator
template <typename ValueType>
struct ListIterator {
  using self_t = ListIterator<ValueType>;
  using node_t = ListNode<ValueType>;

  // Iterator trait
  using difference_type = uint64_t;
  using iterator_category = bidirectional_iterator_tag;
  using value_type = ValueType;
  using pointer = ValueType*;
  using reference = ValueType&;

  // construct function
  ListIterator() noexcept : node_() {}
  explicit ListIterator(ListNodeBase* n) noexcept : node_(n) {}

  reference operator*() const noexcept { return static_cast<node_t*>(node_)->data_; }

  pointer operator->() const noexcept { return &(static_cast<node_t*>(node_)->data_); }

  self_t& operator++() noexcept {
    node_ = node_->next_;
    return *this;
  }

  self_t operator++(int) noexcept {
    self_t temp = *this;
    node_ = node_->next_;
    return temp;
  }

  self_t& operator--() noexcept {
    node_ = node_->prev_;
    return *this;
  }

  self_t operator--(int) noexcept {
    self_t temp = *this;
    node_ = node_->prev_;
    return temp;
  }

  bool operator==(const self_t& rhs) const noexcept { return node_ == rhs.node_; }

  bool operator!=(const self_t& rhs) const noexcept { return node_ != rhs.node_; }

  ListNodeBase* node_;
};

/**
 * List 的简单实现
 */
template <typename ValueType, typename Alloc = allocator<ValueType>>
class List : protected ListBase<ValueType, Alloc> {
  using base_t = ListBase<ValueType, Alloc>;
  using node_t = ListNode<ValueType>;
  using reference = node_t&;

  using allocator_type = Alloc;
  using base_t::node_alloc_t;

  // ❗ 目前ValueType只适用于 C++ 内置基本数据类型(无需调用析构)
  node_t* create_node(const ValueType& x) {
    // 调用父类的方法申请一块内存
    node_t* p = this->get_node();

    // 构造对象
    // TODO: 因该调用ValuetType的构造函数 -> 调用中间层：全局的construct()函数<待实现>
    // 因此目前只能使用内置类型
    // 其实此处对于自定义数据结构调用的是拷贝构造/拷贝赋值函数，但是由于无法调用析构函数，因此整体功能受限
    p->data_ = x;

    return p;
  }

  // TODO: 因该调用ValuetType的析构函数 -> 调用中间层：全局的deconstruct()函数<待实现>
  void destory_node(node_t* p) noexcept { base_t::put_node(p); }

 public:
  /* TODO: construct/copy/destroy
   */
  List() : base_t() {}
  explicit List(const allocator_type& a) : base_t(node_alloc_t(a)) {}

  // List copy constructor.
  // List(const List& rhs){this->impl_}

  // List move constructor: need to implement move constructor in ListBase

  // copy assign
  // List& operator=(const List& rhs) noexcept {}

  // move assign
  // List& operator=(List&& rhs) noexcept {}

  /* iterators function
   */
  using iterator = ListIterator<ValueType>;
  iterator begin() noexcept {
    // 获取dummy node的next,并构造成迭代器
    return iterator(this->impl_.node_.next_);
  }
  iterator end() noexcept {
    // 获取dummy node,并构造成迭代器
    return iterator(&(this->impl_.node_));
  }

  // TODO: reverse_iterator

  /* capacity
   */
  bool empty() const noexcept { return this->impl_.node_.next_ == &this->impl_.node_; }
  size_type size() const noexcept {
    size_type distance = 0;
    iterator iter = begin();
    while (iter != end()) {
      ++distance;
      ++iter;
    }

    return distance;
  }

  /* element access
   */
  reference front() noexcept { return *begin(); }
  reference back() noexcept {
    iterator temp = end();
    --temp;
    return *temp;
  }

  /* TODO: modifiers
   */

  void push_front(const ValueType& x) { this->insert(begin(), x); }

  void pop_front() noexcept { this->erase(begin()); }

  void push_back(const ValueType& x) { this->insert(end(), x); }

  void pop_back() noexcept { this->erase(iterator(this->impl_.node_.prev_)); }

  void insert(iterator position, const ValueType& x) {
    node_t* temp = create_node(x);
    temp->hook(position.node_);
  }

  iterator erase(iterator position) noexcept {
    // 返回删除节点的下一个节点
    iterator ret = iterator(position.node_->next_);

    node_t* temp = static_cast<node_t*>(position.node_);
    // 从链表断开
    temp->unhook();
    // 释放空间
    destory_node(temp);

    return ret;
  }

  void swap(List& other) noexcept;

  void clear() noexcept {
    base_t::clear();
    base_t::init();
  }

  /* TODO: list operations
   */
  void splice(iterator pos, List& other);
  void splice(iterator pos, List& other, iterator first, iterator last);

  void reverse() noexcept;
  void unique();

  template <typename Compare>
  void sort(Compare cmp);
};
}  // namespace __tiny_stl