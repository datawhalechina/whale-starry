#include "myvector.h"

#include <iostream>
#include <vector>

int main() {
  // construct()
  std::cout << "======= construct ========" << std::endl;
  myvector<int> mv1;
  myvector<int> mv2(5, 3);
  std::cout << "mv1:";
  mv1.print();
  std::cout << "mv2:";
  mv2.print();

  // push_back()
  std::cout << "======= mv1.push_back() ========" << std::endl;
  mv1.push_back(8);
  mv1.push_back(9);
  mv1.push_back(10);
  mv1.push_back(11);
  mv1.push_back(12);
  mv1.push_back(16);
  mv1.push_back(22);
  mv1.push_back(32);
  mv1.push_back(44);
  std::cout << "mv1:";
  mv1.print();

  // begin(), end()
  std::cout << "======= begin,end ========" << std::endl;
  std::cout << "mv1.begin():" << mv1.begin() << std::endl;
  std::cout << "mv1.end():" << mv1.end() << std::endl;
  std::cout << "mv2.begin():" << mv2.begin() << std::endl;
  std::cout << "mv2.end():" << mv2.end() << std::endl;

  // size(), max_size()
  std::cout << "======= size,max_size ========" << std::endl;
  std::cout << "mv1.size():" << mv1.size();
  std::cout << " mv1.maxsize():" << mv1.max_size() << std::endl;
  std::cout << "mv2.size():" << mv2.size();
  std::cout << " mv2.maxsize():" << mv2.max_size() << std::endl;

  // []
  std::cout << "======= operator[] ========" << std::endl;
  std::cout << "mv1[2]:" << mv1[2] << std::endl;
  std::cout << "mv1[4]:" << mv1[4] << std::endl;
  std::cout << "mv2[2]:" << mv2[2] << std::endl;
  std::cout << "mv2[4]:" << mv2[4] << std::endl;

  // empty()
  std::cout << "======= empty ========" << std::endl;
  std::cout << mv1.empty() << std::endl;
  std::cout << mv2.empty() << std::endl;
}
