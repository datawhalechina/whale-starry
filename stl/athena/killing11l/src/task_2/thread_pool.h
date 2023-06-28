#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <atomic>
#include <functional>
#include <future>
#include <iostream>
#include <queue>
#include <stdexcept>
#include <thread>

class ThreadPool {
 public:
  ThreadPool() = delete;
  explicit ThreadPool(size_t);

  template <class FUN, class... ARGS>
  auto PushTask(FUN&& f, ARGS&&... args)
      -> std::future<typename std::result_of<FUN(ARGS...)>::type> {
    using ret_type = typename std::result_of<FUN(ARGS...)>::type;
    auto task = std::make_shared<std::packaged_task<ret_type()>>(
        std::bind(std::forward<FUN>(f), std::forward<ARGS>(args)...));

    auto res = task->get_future();
    {
      std::unique_lock<std::mutex> lock(mtx_);
      tasks_.emplace([task] { (*task)(); });
    }
    cond_.notify_one();
    return res;
  }
  ~ThreadPool();

 private:
  void Run();

 private:
  std::vector<std::thread> ths_;
  std::queue<std::function<void()>> tasks_;
  std::mutex mtx_;
  std::condition_variable cond_;
  std::atomic_bool stop_{false};
};

#endif  // THREAD_POOL_H