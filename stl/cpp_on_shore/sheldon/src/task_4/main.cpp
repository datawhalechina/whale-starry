#include <map>
#include <string>

#include "test.hpp"

class Person {
  /*这个类是想测试针对自定义的类，实现一种自定义的_Compare，
  来替代map中默认的std::less<_Key>
  但是难度有点大，没有完成。
  */
 public:
  Person() {}
  Person(std::string name, int age) {
    m_name = name;
    m_age = age;
  }
  bool operator<(Person& rp) { return this->m_age < rp.m_age; }

 private:
  std::string m_name;
  int m_age;
};
void print_map(std::map<std::string, bool>& umap) {
  std::cout << "-----Print map--------" << std::endl;
  if (umap.empty()) {
    std::cout << "map is empty" << std::endl;
  } else {
    for (auto p : umap) {
      std::cout << "Book name: " << p.first << std::endl;
    }
  }
}
void test_map() {
  // 测试map常用接口
  std::map<std::string, bool> umap;
  Person p = Person("c", 8);
  umap["c"] = true;
  umap.insert(std::pair<std::string, bool>("c++", true));
  umap["python"] = true;
  umap["java"] = true;
  umap["deep learning"] = true;
  print_map(umap);
  std::cout << "map size: " << umap.size() << std::endl;

  auto it = umap.find("c");
  if (it != umap.end()) {
    std::cout << "find: " << (*it).first << std::endl;
  }

  umap.erase(it);
  print_map(umap);

  umap.clear();
  print_map(umap);

  return;
}

int main() {
  test();
  test_map();

  Person P1 = Person("andy", 10);
  Person P2 = Person("liu", 20);
  bool flag = P1 < P2;
  std::cout << flag << std::endl;

  // std::map<Person, bool> umap;
  // umap.insert(std::pair<Person, bool>(Person("a", 10), true));

  return 0;
}