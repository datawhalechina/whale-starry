#include <iostream>
#include <list>
#include <string>

void printList(const std::list<int>& l) {
  for (std::list<int>::const_iterator it = l.begin(); it != l.end(); it++) {
    std::cout << (*it) << "  ";
  }
  std::cout << std::endl;
}

// list构造函数
void Test1() {
  std::list<int> l;
  l.push_back(10);
  l.push_back(10);
  l.push_back(10);
  l.push_back(10);
  l.push_back(10);
  printList(l);

  std::list<int> l2(l.begin(), l.end());
  printList(l2);

  std::list<int> l3(l2);
  printList(l3);
}

// list赋值与交换
void Test2() {
  std::list<int> l;
  l.push_back(10);
  l.push_back(10);
  l.push_back(10);
  l.push_back(10);
  l.push_back(10);

  std::list<int> l2 = l;
  printList(l2);

  std::list<int> l3;
  l3.assign(l2.begin(), l2.end());
  printList(l3);

  std::list<int> l4;
  l4.assign(5, 100);
  printList(l4);

  l.swap(l4);
  printList(l4);
}

// list大小操作
void Test3() {
  std::list<int> l;
  l.push_back(10);
  l.push_back(10);
  l.push_back(10);
  l.push_back(10);
  l.push_back(10);
  printList(l);

  if (l.empty()) {
    cout << "l为空" << endl;
  } else {
    cout << "l不为空" << endl;
  }

  cout << l.size() << endl;
}

/*
排序案例：
描述：将Person自定义数据类型进行排序，Person中属性有姓名、年龄、身高
规则：按照年龄进行升序，如果年龄相同按照身高进行降序
*/

class Person {
 public:
  Person(string name, int age, int height) {
    this->m_Name = name;
    this->m_Age = age;
    this->m_Height = height;
  }

  string m_Name;
  int m_Age;
  int m_Height;
};

bool comparePerson(Person& p1, Person& p2) {
  if (p1.m_Age == p2.m_Age) {
    return p1.m_Height > p2.m_Height;
  }
  return p1.m_Age < p2.m_Age;
}

void Test() {
  std::list<Person> L;
  Person p1("刘备", 35, 175);
  Person p2("关羽", 33, 180);
  Person p3("张飞", 30, 178);
  Person p4("曹操", 40, 175);
  Person p5("孙权", 35, 190);

  L.push_back(p1);
  L.push_back(p2);
  L.push_back(p3);
  L.push_back(p4);
  L.push_back(p5);

  for (std::list<Person>::const_iterator it = L.begin(); it != L.end(); it++) {
    std::cout << "姓名：" << it->m_Name << " 年龄：" << it->m_Age << " 身高："
              << it->m_Height << std::endl;
  }

  L.sort(comparePerson);
  std::cout << "-----------排序后------------" << endl;
  for (std::list<Person>::const_iterator it = L.begin(); it != L.end(); it++) {
    std::cout << "姓名：" << it->m_Name << " 年龄：" << it->m_Age << " 身高："
              << it->m_Height << std::endl;
  }
}

int main() {
  Test();
  return 0;
}
