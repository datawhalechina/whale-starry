#include "thread_pool.h"

ThreadPool::ThreadPool(size_t threadNum) {
  ths_.reserve(threadNum);  // 提前申请空间，避免vector多次扩容导致多次拷贝。
  for (size_t i = 0; i < threadNum; ++i) {
    ths_.emplace_back(std::thread(&ThreadPool::Run, this));  // 使用原位构造提高性能
  }
}

ThreadPool::~ThreadPool() {
  stop_.store(true);
  cond_.notify_all();
  for (auto& item : ths_) {
    if (item.joinable()) {
      item.join();
    }
  }
}

void ThreadPool::Run() {
  while (!stop_) {
    std::function<void()> task = nullptr;
    {
      std::unique_lock<std::mutex> lock(mtx_);
      cond_.wait(lock, [this] { return stop_ || !this->tasks_.empty(); });
      if (stop_ || tasks_.empty()) {
        return;
      }
      task = std::move(tasks_.front());
      tasks_.pop();
    }
    if (task != nullptr) {
      task();
    }
  }
}