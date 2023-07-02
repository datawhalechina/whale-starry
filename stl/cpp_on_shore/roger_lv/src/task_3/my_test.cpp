#include <iostream>
#include <set>

// 由于红黑树这个数据结构确实有点难度，这里仅做一些接口测试，插入/删除/查找复杂度均为O(logn)

class Person {
 public:
  Person(int age, std::string name) : age(age), name(name) {}
  // 注意我们必须自定义<运算符，不然自定义的类无法插入红黑树，rb_tree内部是一定要求有序的
  bool operator<(const Person& other) const { return this->age < other.age; }

  int GetAge() { return age; }
  void PrintInfo() { std::cout << "age: " << age << " name: " << name << std::endl; }

 private:
  int age;
  std::string name;
};

void WhetherMarry(auto t) {
  std::cout << "Guess who can get married?" << std::endl;
  auto it = t.begin();
  while (it != t.end()) {
    if (it->GetAge() > 20) {
      it->PrintInfo();
    }
    ++it;
  }
}

void PrintEdge(auto t) {
  std::cout << "The oldest person is: " << std::endl;
  auto it = t.end();
  --it;
  it->PrintInfo();
  it = t.begin();
  it->PrintInfo();
}

int main() {
  std::_Rb_tree<Person, Person, std::_Identity<Person>, std::less<Person>> my_tree;
  my_tree._M_insert_unique(Person(10, "roger"));  // unique插入
  my_tree._M_insert_unique(Person(12, "tom"));
  my_tree._M_insert_unique(Person(24, "petter"));
  my_tree._M_insert_unique(Person(10, "kate"));
  my_tree._M_insert_unique(Person(19, "tim"));
  my_tree._M_insert_unique(Person(25, "jhon"));
  std::cout << my_tree.size() << std::endl;  // 可以看到unique插入方式会自动去重
  for (auto x : my_tree) std::cout << x.GetAge() << " ";
  std::cout << std::endl;
  auto tmp = my_tree.find(*++my_tree.begin());  // 查找
  std::cout << tmp->GetAge() << std::endl;

  WhetherMarry(my_tree);
  PrintEdge(my_tree);
  return 0;
}