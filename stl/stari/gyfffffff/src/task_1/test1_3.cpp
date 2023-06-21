// test constuct functions and other list functions
#include <iostream>
#include <list>

template <typename T>
void print(std::list<T> listx) {
  for (T x : listx) {
    std::cout << x << " ";
  }
  std::cout << std::endl;
}
int main() {
  std::list<int> list1{1, 2, 3, 4, 5, 6};

  // 复制构造
  std::list<int> list2(list1);
  print(list2);

  // 范围构造
  auto Iterf = ++list1.begin();
  auto Iterl = --(--list1.end());
  std::list<int> list3(Iterf, Iterl);
  print(list3);

  // copy =
  std::list<int> list4 = list1;
  std::cout << "list4:";
  print(list4);

  // assign
  std::list<int> list5;
  list5.assign(3, 8);
  std::cout << "list5:";
  print(list5);

  // assign a range
  std::list<int> list6;
  list6.assign(Iterf, Iterl);
  std::cout << "list6:";
  print(list6);

  // maxsize
  std::cout << list1.max_size() << std::endl;

  // push_front&emplace_front
  list1.push_front(99);
  print(list1);
  list1.emplace_front(88);
  print(list1);

  list1.sort();
  print(list1);
}