#include <cassert>
#include <iostream>

#include "ivarnd_vector.h"


int main() {
  /*
  // Test push_back()
  ivarnd::vector<int> a;

  for(int i = 1; i < 5; i++) {
      a.push_back(i);
  }
  for(int i = 0; i < a.size(); i++) {
      std::cout << a[i] << " ";
  }
  std::cout << std::endl;

  for(ivarnd::vector<int>::iterator it = a.begin(); it != a.end(); it++) {
      std::cout << *it << " ";
  }
  std::cout << std::endl;

  for(auto e : a) {
      std::cout << e << " ";
  }
  std::cout << std::endl;
  */

  // Test resize()
  ivarnd::vector<int> a;
  a.push_back(1);
  a.push_back(2);
  a.push_back(3);
  a.push_back(4);
  a.push_back(5);
  a.push_back(6);
  for (auto e : a) {
    std::cout << e << " ";
  }
  std::cout << std::endl;
  a.resize(10);
  for (auto e : a) {
    std::cout << e << " ";
  }
  std::cout << std::endl;
  a.resize(3);
  for (auto e : a) {
    std::cout << e << " ";
  }
  std::cout << std::endl;

  return 0;
}