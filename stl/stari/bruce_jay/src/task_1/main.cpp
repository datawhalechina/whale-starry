#include <algorithm>
#include <iostream>
#include <list>

class A {
 private:
  int n;

 public:
  A(int n_) { n = n_; }
  friend bool operator<(const A &a1, const A &a2);
  friend bool operator==(const A &a1, const A &a2);
  friend std::ostream &operator<<(std::ostream &o, const A &a);
};
bool operator<(const A &a1, const A &a2) { return a1.n < a2.n; }
bool operator==(const A &a1, const A &a2) { return a1.n == a2.n; }
std::ostream &operator<<(std::ostream &o, const A &a) {
  o << a.n;
  return o;
}
template <class T>
void PrintList(const std::list<T> &lst) {
  // 不推荐的写法，还是用两个迭代器作为参数更好
  int tmp = lst.size();
  if (tmp > 0) {
    typename std::list<T>::const_iterator i;
    i = lst.begin();
    for (i = lst.begin(); i != lst.end(); i++) std::cout << *i << ",";
  }
}  // typename用来说明 std::list<T>::const_iterator是个类型
// 在vs中不写也可以
int main() {
  std::list<A> lst1, lst2;
  lst1.push_back(1);
  lst1.push_back(3);
  lst1.push_back(2);
  lst1.push_back(4);
  lst1.push_back(2);
  lst2.push_back(10);
  lst2.push_front(20);
  lst2.push_back(30);
  lst2.push_back(30);
  lst2.push_back(30);
  lst2.push_front(40);
  lst2.push_back(40);
  std::cout << "1) ";
  PrintList(lst1);
  std::cout << std::endl;
  // 1) 1,3,2,4,2,
  std::cout << "2) ";
  PrintList(lst2);
  std::cout << std::endl;
  // 2) 40,20,10,30,30,30,40,
  lst2.sort();
  std::cout << "3) ";
  PrintList(lst2);
  std::cout << std::endl;
  // 3) 10,20,30,30,30,40,40,
  lst2.pop_front();
  std::cout << "4) ";
  PrintList(lst2);
  std::cout << std::endl;
  // 4) 20,30,30,30,40,40,
  lst1.remove(2);  // 删除所有和A(2)相等的元素
  std::cout << "5) ";
  PrintList(lst1);
  std::cout << std::endl;
  // 5) 1,3,4,
  lst2.unique();  // 删除所有和前一个元素相等的元素
  std::cout << "6) ";
  PrintList(lst2);
  std::cout << std::endl;
  // 6) 20,30,40,
  lst1.merge(lst2);  // 合并 lst2到lst1并清空lst2
  std::cout << "7) ";
  PrintList(lst1);
  std::cout << std::endl;
  // 7) 1,3,4,20,30,40,
  std::cout << "8) ";
  PrintList(lst2);
  std::cout << std::endl;
  // 8)
  lst1.reverse();
  std::cout << "9) ";
  PrintList(lst1);
  std::cout << std::endl;
  // 9) 40,30,20,4,3,1,
  lst2.push_back(100);
  lst2.push_back(200);
  lst2.push_back(300);
  lst2.push_back(400);
  std::list<A>::iterator p1, p2, p3;
  p1 = find(lst1.begin(), lst1.end(), 3);
  p2 = find(lst2.begin(), lst2.end(), 200);
  p3 = find(lst2.begin(), lst2.end(), 400);
  lst1.splice(p1, lst2, p2, p3);
  // 将[p2,p3)插入p1之前，并从lst2中删除[p2,p3)
  std::cout << "10) ";
  PrintList(lst1);
  std::cout << std::endl;
  // 10) 40,30,20,4,200,300,3,1,
  std::cout << "11) ";
  PrintList(lst2);
  std::cout << std::endl;
  // 11) 100,400,
  return 0;
}
