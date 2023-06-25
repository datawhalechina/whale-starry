#include <sys/syscall.h>
#include <unistd.h>

#include "thread_pool.h"

namespace {
constexpr int32_t MAX_ARGC = 2;
constexpr int32_t ARGC_1 = 1;
constexpr int32_t TASK_NUM = 50;
}  // namespace

int32_t func(int32_t arg) {
  sleep(1);
  std::cout << "thread id: " << static_cast<pid_t>(::syscall(SYS_gettid))
            << ", arg: " << arg << std::endl;
  return 0;
}

void finish() { std::cout << "finish!!!" << std::endl; }

int32_t main(int32_t argc, const char* argv[]) {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  if (argc != MAX_ARGC) {
    std::cout << "Please enter a parameter indicating the number of threads."
              << std::endl;
    return 0;
  }

  if (atoi(argv[ARGC_1]) <= 0) {  // 防止用户有非数字输入。
    std::cout << "Please enter a positive integer greater than 0." << std::endl;
    return 0;
  }

  ThreadPool th(static_cast<size_t>(atoi(argv[ARGC_1])));

  for (int32_t i = 0; i < TASK_NUM; ++i) {
    th.PushTask(func, i);
  }

  th.PushTask(finish).get();  // 实现闭锁效果，此目标任务完成主线程才可以结束。
  return 0;
}