#include <algorithm>
#include <chrono>
#include <iostream>
#include <list>
#include <random>
#include <vector>

// 定义一个计时器类
class Timer {
 public:
  Timer() : m_start(std::chrono::high_resolution_clock::now()) {}
  void reset() { m_start = std::chrono::high_resolution_clock::now(); }
  double elapsed() const {
    return std::chrono::duration_cast<std::chrono::duration<double>>(
               std::chrono::high_resolution_clock::now() - m_start)
        .count();
  }

 private:
  std::chrono::time_point<std::chrono::high_resolution_clock> m_start;
};

void TestVector(const int N, const int M) {
  std::vector<int> v;  // 定义一个vector

  // 生成随机数
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(1, 100);

  // 插入元素
  double insert_time = 0;
  {
    Timer t;
    for (int i = 0; i < N; ++i) {
      v.push_back(dis(gen));
    }
    insert_time += t.elapsed();
  }
  std::cout << "time to insert " << N
            << " elements into vector: " << insert_time << " s\n";

  // 随机访问
  double access_time = 0;
  for (int i = 0; i < M; ++i) {
    long long sum = 0;
    Timer t;
    for (int i = 0; i < N; ++i) {
      sum += v[i];
    }
    access_time += t.elapsed();
  }
  std::cout << "time to access " << N << " elements in vector: " << access_time
            << " s\n";

  // 删除元素
  double erase_middle_time = 0;
  for (int i = 0; i < 10000; ++i) {
    auto it = v.begin();
    std::advance(it,
                 N / 2);  // 查找"中间"那个，虽然不是真的中间，但不影响测试结果
    Timer t;
    v.erase(it);
    erase_middle_time += t.elapsed();
  }
  std::cout << "time to erase an element from the middle of vector: "
            << erase_middle_time << " s\n";

  // 排序
  {
    Timer t;
    std::sort(v.begin(), v.end());
    double sort_time = t.elapsed();
    std::cout << "time to sort of vector: " << sort_time << " s\n";
  }
}

void TestList(const int N, const int M) {
  std::list<int> l;  // 定义一个list

  // 生成随机数
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(1, 100);

  // 插入元素
  double insert_time = 0;
  {
    Timer t;
    for (int i = 0; i < N; ++i) {
      l.push_back(dis(gen));
    }
    insert_time += t.elapsed();
  }
  std::cout << "time to insert " << N << " elements into list: " << insert_time
            << " s\n";

  // 随机访问
  double access_time = 0;
  for (int i = 0; i < M; ++i) {
    long long sum = 0;
    Timer t;
    for (auto it = l.begin(); it != l.end(); ++it) {
      sum += *it;
    }
    access_time += t.elapsed();
  }
  std::cout << "time to access " << N << " elements in list: " << access_time
            << " s\n";

  // 删除元素
  double erase_middle_time = 0;
  for (int i = 0; i < 10000; ++i) {
    auto it = l.begin();
    std::advance(it, N / 2);
    Timer t;
    l.erase(it);
    erase_middle_time += t.elapsed();
  }
  std::cout << "time to erase an element from the middle of list: "
            << erase_middle_time << " s\n";

  // 排序
  {
    Timer t;
    l.sort();
    double sort_time = t.elapsed();
    std::cout << "time to sort of list: " << sort_time << " s\n";
  }
}
void Compare() {
  const int N = 1000000;  // 数据量大小
  const int M = 10;       // 每个操作执行的次数
  std::cout << "Testing list:\n";
  TestList(N, M);

  std::cout << "\nTesting vector:\n";
  TestVector(N, M);
}

void Expansion() {
  std::vector<int> vec;

  // 输出vector的初始容量和大小
  std::cout << "Initial capacity: " << vec.capacity() << std::endl;
  std::cout << "Initial size: " << vec.size() << std::endl;

  // 添加1000个整数到vector中，并在容量发生变化时输出相关信息
  int mlen = 0;
  for (int i = 0; i < 1000; i++) {
    vec.push_back(i);
    if (vec.capacity() != mlen) {
      std::cout << "size: " << vec.size() << " capacity: " << vec.capacity()
                << std::endl;
      mlen = vec.capacity();
    }
  }

  // 手动控制内存分配和释放
  vec.reserve(2000);  // 预分配2000个元素的空间
  std::cout << "Capacity after reserving 2000 elements: " << vec.capacity()
            << std::endl;

  vec.shrink_to_fit();  // 释放多余的内存
  std::cout << "Capacity after shrinking to fit: " << vec.capacity()
            << std::endl;
}

int main() {
  // 动态扩容机制
  Expansion();

  // vector, list效率对比
  Compare();
  return 0;
}