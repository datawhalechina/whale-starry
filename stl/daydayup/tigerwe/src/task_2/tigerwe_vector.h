#ifndef _TIGERWE_VECTOR_H
#define _TIGERWE_VECTOR_H

#include <iostream>
#include <memory>

template <typename T>
class vector {
 public:
  class iterator {
   public:
    using value_type = T;
    using reference = T&;
    using pointer = T*;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::forward_iterator_tag;

    iterator() : p_(nullptr) {}
    explicit iterator(pointer p) : p_(p) {}
    ~iterator() = default;

    reference operator*() const noexcept { return *p_; }

    pointer operator->() const noexcept { return p_; }

    iterator& operator++() noexcept {
      ++p_;
      return *this;
    }

    iterator operator++(int) noexcept {
      iterator temp(*this);
      ++p_;
      return temp;
    }
    // 迭代器的指针是私有成员，无法被直接访问，所以我们需要使用友元函数的方式来实现重载
    friend operator+(const iterator& lhs, difference_type rhs) noexcept {
      return iterator(lhs.p_ + rhs);
    }

    friend operator+(difference_type lhs, const iterator& rhs) noexcept {
      return iterator(rhs.p_ + lhs);
    }

    bool operator==(const iterator& other) const noexcept { return p_ == other.p_; }

    bool operator!=(const iterator& other) const noexcept { return !(*this == other); }

   private:
    pointer p_;
  };

  class reverse_iterator {
   public:
    using value_type = T;
    using reference = T&;
    using pointer = T*;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::bidirectional_iterator_tag;

    reverse_iterator() : p_(nullptr) {}
    explicit reverse_iterator(pointer p) : p_(p) {}
    ~reverse_iterator() = default;

    reference operator*() const noexcept { return *p_; }

    pointer operator->() const noexcept { return p_; }

    reverse_iterator& operator++() noexcept {
      --p_;
      return *this;
    }

    reverse_iterator operator++(int) noexcept {
      reverse_iterator temp(*this);
      --p_;
      return temp;
    }

    friend operator+(const reverse_iterator& lhs, difference_type rhs) noexcept {
      return reverse_iterator(lhs.p_ - rhs);
    }

    friend operator+(difference_type lhs, const reverse_iterator& rhs) noexcept {
      return reverse_iterator(rhs.p_ - lhs);
    }

    bool operator==(const reverse_iterator& other) const noexcept {
      return p_ == other.p_;
    }

    bool operator!=(const reverse_iterator& other) const noexcept {
      return !(*this == other);
    }

   private:
    pointer p_;
  };

  vector() : size_(0), capacity_(8), data_(std::make_unique<T[]>(capacity_)) {}
  ~vector() = default;

  void push_back(const T& value) {
    // 如果元素满了，动态增加当前两倍的容量
    if (size_ == capacity_) {
      resize(capacity_ * 2);
    }
    data_[size_++] = value;
  }

  void pop_back() {
    if (size_ > 0) {
      --size_;
    }
  }

  void erase(const iterator& pos) {
    auto to_remove = pos.p_ - &data_[0];
    for (size_t i = to_remove; i < size_ - 1; ++i) {
      data_[i] = std::move(data_[i + 1]);
    }
    --size_;
  }

  T& front() noexcept { return data_[0]; }

  const T& front() const noexcept { return data_[0]; }

  T& back() noexcept { return data_[size_ - 1]; }

  const T& back() const noexcept { return data_[size_ - 1]; }

  T& operator[](size_t index) noexcept { return data_[index]; }

  const T& operator[](size_t index) const noexcept { return data_[index]; }

  size_t size() const noexcept { return size_; }

  bool empty() const noexcept { return size_ == 0; }

  void clear() noexcept {
    size_ = 0;
    capacity_ = 8;
    data_ = std::make_unique<T[]>(capacity_);
  }

  iterator begin() noexcept { return iterator(&data_[0]); }

  iterator end() noexcept { return iterator(&data_[size_]); }

  reverse_iterator rbegin() noexcept { return reverse_iterator(&data_[size_ - 1]); }

  reverse_iterator rend() noexcept { return reverse_iterator(&data_[-1]); }

 private:
  void resize(size_t new_capacity) {
    auto new_data = std::make_unique<T[]>(new_capacity);
    for (size_t i = 0; i < size_; ++i) {
      new_data[i] = std::move(data_[i]);
    }
    data_ = std::move(new_data);
    capacity_ = new_capacity;
  }

  size_t size_;
  size_t capacity_;
  std::unique_ptr<T[]> data_;
};

#endif