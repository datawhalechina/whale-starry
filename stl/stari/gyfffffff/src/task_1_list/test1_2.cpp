// test a list is a recursive or not
// the program still exits some problem ... for it doesn't do as excepted

#include <iostream>
#include <list>

using std::cout;
using std::endl;

int main() {
  std::list<int> mylist{1, 2, 3, 6};

  auto a = mylist.end();  // 最后一个节节点的下一个点
  auto b = ++a;           // 应该是第一个节点，如果是环状
  auto c = --a;           // 最后一个节点

  cout << "end.next val:" << *a << endl;       // expect unknow num
  cout << "end.next.next val:" << *b << endl;  // expect first num
  cout << "end val:" << *c << endl;            // except last num
}