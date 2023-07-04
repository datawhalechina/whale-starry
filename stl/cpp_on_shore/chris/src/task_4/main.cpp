#include <chrono>
#include <iostream>
#include <map>
#include <string>
/*
    因为最近在忙转正汇报,
    没有太多精力放在map这边,
    因此本次task只是熟悉map接口并调用,
    之后有时间了会重新复习
*/

class MapTimer {
 public:
  std::chrono::time_point<std::chrono::steady_clock> start, end;
  std::chrono::duration<float> last_time;

  // constructor
  MapTimer() { start = std::chrono::high_resolution_clock::now(); }
  // destructor
  ~MapTimer() {
    end = std::chrono::high_resolution_clock::now();
    last_time = end - start;
    float result_ms = last_time.count() * 1000.0f;
    std::cout << "Took " << result_ms << "ms" << std::endl;
  }
};

void print(std::map<std::string, int> M) {
  for (auto it = M.begin(); it != M.end(); it++) {
    std::cout << "key: " << it->first << ","
              << "value: " << it->second << std::endl;
  }
}

// 使用MapTimer测试一下map中emplace和insert速度快慢
/*
    emplace的用法与insert的用法不同,
    emplace是按照键值对(key-value)的顺序,
    依次输入key, value,
    输入的argements个数为2
*/
void cal_emplace(std::map<std::string, int> M) {
  MapTimer timer;
  for (int i = 0; i < 10000; i++) {
    M.emplace(std::to_string(i), i);
  }
  print(M);
}

/*
    insert的用法,
    insert是将一个键值对当成一个输入元素,
    因此insert的输入argument只有一个,
    我们需要使用花括号{}将两个元素合并为一个元素
*/
void cal_insert(std::map<std::string, int> M) {
  MapTimer timer;
  for (int i = 0; i < 10000; i++) {
    M.insert({std::to_string(i), i});
  }
  print(M);
}

int main() {
  std::map<std::string, int> Map;
  std::map<std::string, int> MapCopy(Map);
  // Map.insert({"hello", 1});

  /*
      At first, I thought that emplace is faster
      than the insert function.
      However, after 100 epoch tests,
      I finally find that insert is a little faster.
  */
  for (int i = 0; i < 100; i++) {
    std::cout << "The Emplace ";
    cal_emplace(Map);
    std::cout << "The insert ";
    cal_insert(MapCopy);
  }

  return 0;
}
