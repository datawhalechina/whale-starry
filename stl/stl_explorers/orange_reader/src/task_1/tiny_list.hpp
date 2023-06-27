#ifndef TINYSTL_LIST_HPP_
#define TINYSTL_LIST_HPP_

#include "tiny_alloc.h"
#include "tiny_iterator.h"

#include <cstdint>

namespace tiny_stl {
using SizeType = uint64_t;

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

template <typename DataType>
struct ListNode : public ListNodeBase {
  DataType data_;  // user's data
};

// ⭐ ListBase中的Alloc是不确定的，但是List中会指定默认参数
// 且List中的Alloc就是为了父类而定义的
// ❗其中Alloc是一个模板类型参数
template <typename DataType, typename Alloc>
class ListBase {
 protected:
  // TODO: consider alloctor
  using NodeAllocType = typename Alloc::Rebind<ListNode<DataType>>::other;

  struct ListImpl : public NodeAllocType {
    ListNodeBase node_;

    // construct function
    ListImpl() : node_() {}
    ListImpl(const NodeAllocType& a) noexcept : NodeAllocType(a), node_() {}
  };

  ListImpl impl_;

  using NodeType = ListNode<DataType>;
  NodeType* get_node() {
    // 调用impl_的父类的方法，申请空间 <==> malloc调用
    return impl_.NodeAllocType::allocate();
  }

  // 调用impl_的父类的方法，申请空间 <==> free调用
  void put_node(NodeType* p) { impl_.NodeAllocType::deallocate(p); }

 public:
  // 普通构造函数
  ListBase() : impl_() { init(); };
  ListBase(const NodeAllocType& a) noexcept : impl_(a) { init(); }

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
template <typename DataType>
struct ListIterator {
  using SelfType = ListIterator<DataType>;
  using NodeType = ListNode<DataType>;

  // Iterator trait
  using DifferenceType = uint64_t;
  using IteratorCategory = BidirectionalIteratorTag;
  using ValueType = DataType;
  using Pointer = DataType*;
  using Reference = DataType&;

  // construct function
  ListIterator() noexcept : node_() {}
  explicit ListIterator(ListNodeBase* n) noexcept : node_(n) {}

  Reference operator*() const noexcept { return static_cast<NodeType*>(node_)->data_; }

  Pointer operator->() const noexcept { return &(static_cast<NodeType*>(node_)->data_); }

  SelfType& operator++() noexcept {
    node_ = node_->next_;
    return *this;
  }

  SelfType operator++(int) noexcept {
    SelfType temp = *this;
    node_ = node_->next_;
    return temp;
  }

  SelfType& operator--() noexcept {
    node_ = node_->prev_;
    return *this;
  }

  SelfType operator--(int) noexcept {
    SelfType temp = *this;
    node_ = node_->prev_;
    return temp;
  }

  bool operator==(const SelfType& rhs) const noexcept { return node_ == rhs.node_; }

  bool operator!=(const SelfType& rhs) const noexcept { return node_ != rhs.node_; }

  ListNodeBase* node_;
};

/**
 * List 的简单实现
 */
template <typename DataType, typename Alloc = Allocator<DataType>>
class List : protected ListBase<DataType, Alloc> {
  using BaseType = ListBase<DataType, Alloc>;
  using NodeType = ListNode<DataType>;
  using Reference = NodeType&;

  using AllocatorType = Alloc;
  using BaseType::NodeAllocType;

  // ❗ 目前DataType只适用于 C++ 内置基本数据类型(无需调用析构)
  NodeType* create_node(const DataType& x) {
    // 调用父类的方法申请一块内存
    NodeType* p = this->get_node();

    // 构造对象
    // TODO: 因该调用ValuetType的构造函数 -> 调用中间层：全局的construct()函数<待实现>
    // 因此目前只能使用内置类型
    // 其实此处对于自定义数据结构调用的是拷贝构造/拷贝赋值函数，但是由于无法调用析构函数，因此整体功能受限
    p->data_ = x;

    return p;
  }

  // TODO: 因该调用ValuetType的析构函数 -> 调用中间层：全局的deconstruct()函数<待实现>
  void destory_node(NodeType* p) noexcept { BaseType::put_node(p); }

 public:
  /* TODO: construct/copy/destroy
   */
  List() : BaseType() {}
  explicit List(const AllocatorType& a) : BaseType(NodeAllocType(a)) {}

  // List copy constructor.
  // List(const List& rhs){this->impl_}

  // List move constructor: need to implement move constructor in ListBase

  // copy assign
  // List& operator=(const List& rhs) noexcept {}

  // move assign
  // List& operator=(List&& rhs) noexcept {}

  /* iterators function
   */
  using Iterator = ListIterator<DataType>;
  Iterator begin() noexcept {
    // 获取dummy node的next,并构造成迭代器
    return Iterator(this->impl_.node_.next_);
  }
  Iterator end() noexcept {
    // 获取dummy node,并构造成迭代器
    return Iterator(&(this->impl_.node_));
  }

  // TODO: reverse_iterator

  /* capacity
   */
  bool empty() const noexcept { return this->impl_.node_.next_ == &this->impl_.node_; }
  SizeType size() const noexcept {
    SizeType distance = 0;
    Iterator iter = begin();
    while (iter != end()) {
      ++distance;
      ++iter;
    }

    return distance;
  }

  /* element access
   */
  Reference front() noexcept { return *begin(); }
  Reference back() noexcept {
    Iterator temp = end();
    --temp;
    return *temp;
  }

  /* TODO: modifiers
   */

  void push_front(const DataType& x) { this->insert(begin(), x); }

  void pop_front() noexcept { this->erase(begin()); }

  void push_back(const DataType& x) { this->insert(end(), x); }

  void pop_back() noexcept { this->erase(Iterator(this->impl_.node_.prev_)); }

  void insert(Iterator position, const DataType& x) {
    NodeType* temp = create_node(x);
    temp->hook(position.node_);
  }

  Iterator erase(Iterator position) noexcept {
    // 返回删除节点的下一个节点
    Iterator ret = Iterator(position.node_->next_);

    NodeType* temp = static_cast<NodeType*>(position.node_);
    // 从链表断开
    temp->unhook();
    // 释放空间
    destory_node(temp);

    return ret;
  }

  void swap(List& other) noexcept;

  void clear() noexcept {
    BaseType::clear();
    BaseType::init();
  }

  /* TODO: list operations
   */
  void splice(Iterator pos, List& other);
  void splice(Iterator pos, List& other, Iterator first, Iterator last);

  void reverse() noexcept;
  void unique();

  template <typename Compare>
  void sort(Compare cmp);
};
}  // namespace tiny_stl

#endif