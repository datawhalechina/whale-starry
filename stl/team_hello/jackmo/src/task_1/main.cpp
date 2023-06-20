#include <algorithm>
#include <iostream>
#include <list>

int main() {
  std::list<int> tlist;
  // Before inserting an element, the size should be 0
  // In fact, the test size is 0
  std::cout << "size=" << tlist.size() << std::endl;

  for (int num = 0; num < 10; num++) {
    tlist.push_back(num);
  }
  // After inserting 10 elements, the size should be 10
  // In fact, the test size is 10
  std::cout << "size=" << tlist.size() << std::endl;

  // Test result: 0 1 2 3 4 5 6 7 8 9
  std::list<int>::iterator ite;
  for (ite = tlist.begin(); ite != tlist.end(); ite++) {
    std::cout << *ite << " ";
  }
  std::cout << std::endl;

  tlist.push_back(13);
  tlist.push_front(15);
  tlist.push_back(12);
  tlist.push_front(3);

  // Test result: 3 15 0 1 2 3 4 5 6 7 8 9 13 12
  for (ite = tlist.begin(); ite != tlist.end(); ite++) {
    std::cout << *ite << " ";
  }
  std::cout << std::endl;

  // Sort of list
  tlist.sort();

  // Test result: 0 1 2 3 3 4 5 6 7 8 9 12 13 15
  for (ite = tlist.begin(); ite != tlist.end(); ite++) {
    std::cout << *ite << " ";
  }
  std::cout << std::endl;

  // Find the value, return the first element's iterator
  // Find nothing, return end()
  ite = std::find(tlist.begin(), tlist.end(), 3);
  if (tlist.end() != ite) {
    tlist.insert(ite, 42);
  }

  // Test result: 0 1 2 42 3 3 4 5 6 7 8 9 12 13 15
  for (ite = tlist.begin(); ite != tlist.end(); ite++) {
    std::cout << *ite << " ";
  }
  std::cout << std::endl;

  ite = std::find(tlist.begin(), tlist.end(), 5);
  if (tlist.end() != ite) {
    tlist.insert(ite, 5, 1024);
  }

  // Test result: 0 1 2 42 3 3 4 1024 1024 1024 1024 1024 5 6 7 8 9 12 13 15
  for (ite = tlist.begin(); ite != tlist.end(); ite++) {
    std::cout << *ite << " ";
  }
  std::cout << std::endl;

  return 0;
}