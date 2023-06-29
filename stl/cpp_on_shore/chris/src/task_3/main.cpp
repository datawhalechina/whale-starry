#include <iostream>

#include "rb_tree.h"

int main() {
  RbTree<int> rb;
  int arr[] = {11, 6, 7, 13, 4, 7, 10, 14, 16};
  int n = sizeof(arr) / sizeof(int);
  for (int i = 0; i < n; i++) rb.Insert(arr[i]);
  rb.InOrder();
  std::cout << std::endl;
  rb.InOrderPrint();
  std::cout << std::endl;
  rb.Remove(10);
  rb.InOrder();
  std::cout << std::endl;
  rb.Remove(0);
}