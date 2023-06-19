#include <iostream>
#include <iterator>
#include <list>

int main() {
  std::list<int> l;
  l.push_back(1);   // l = [1]
  l.push_back(2);   // l = [1, 2]
  l.push_front(3);  // l = [3, 1, 2]
  l.push_front(4);  // l = [4, 3, 1, 2]

  std::list<int>::iterator it = l.begin();

  int a = l.front();  // a = 4

  l.emplace_back(5);  // l = [4, 3, 1, 2, 5]

  int c = l.back();  // l = [4, 3, 1, 2], c = 5
  l.pop_back();
  std::cout << c << std::endl;

  return 0;
}