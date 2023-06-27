#include <chrono>
#include <iostream>
#include <list>

void TestPushAndEmplaceTime(std::list<int>& ll) {
  auto start = std::chrono::steady_clock::now();
  for (int i = 0; i < 100000; ++i) {
    ll.push_back(rand());
  }
  auto end = std::chrono::steady_clock::now();
  std::cout << "Test the push time: "
            << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()
            << " ns" << std::endl;
  start = std::chrono::steady_clock::now();
  for (int i = 0; i < 100000; ++i) {
    ll.emplace_back(rand());
  }
  end = std::chrono::steady_clock::now();
  std::cout << "Test the emplace time: "
            << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()
            << " ns" << std::endl;
}

// 简单的mylist，重载两个运算符
class MyList {
 private:
  std::list<int> my_list;

 public:
  MyList(std::list<int> x) { my_list = x; }

  // 希望能实现直接通过[]索引取值
  auto operator[](int pos) {
    int temp = 0;
    for (auto l : this->my_list) {
      if (temp == pos) return l;
      ++temp;
    }
  }

  // 希望能重载<<输出流，直接能通过cout打印list的值
  friend std::ostream& operator<<(std::ostream& os, const MyList& m_list) {
    for (auto it : m_list.my_list) {
      os << it << " ";
    }
    return os;
  }
};

// 使用list玩一些小东西，以NBA球员为例
class Player {
 private:
  int height;
  int weight;
  int age;
  std::string name;
  std::string team;
  std::string position;
  std::string style;

 public:
  // 设置基础信息，身高、体重、年龄、姓名、所属球队、打的位置和球风
  void SetBasicInfo(int h, int w, int a, std::string n, std::string t, std::string p,
                    std::string s) {
    height = h;
    weight = w;
    age = a;
    name = n;
    team = t;
    position = p;
    style = s;
  }
  // 打印基本信息
  void PrintBasicInfo() {
    std::cout << "Playername = " << name << " ";
    std::cout << "Height = " << height << " ";
    std::cout << "Weight = " << weight << " ";
    std::cout << "Age = " << age << " ";
    std::cout << "His team = " << team << " ";
    std::cout << "Position = " << position << " ";
    std::cout << "Style = " << style << " ";
    std::cout << std::endl;
  }
  // 获取球员身高
  int GetHeight() { return height; }
};
