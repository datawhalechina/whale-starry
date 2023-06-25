#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>

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

  /*
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
  */

  // Test insert
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
  a.insert(a.begin(), 9);
  for (auto e : a) {
    std::cout << e << " ";
  }
  std::cout << std::endl;
  a.insert(a.begin() + 3, 99);
  for (auto e : a) {
    std::cout << e << " ";
  }
  std::cout << std::endl;

  // Test erase
  ivarnd::vector<int> e_a;
  e_a.push_back(1);
  e_a.push_back(2);
  e_a.push_back(3);
  e_a.push_back(4);
  e_a.push_back(5);
  e_a.push_back(6);
  for (auto e : e_a) {
    std::cout << e << " ";
  }
  std::cout << std::endl;
  e_a.erase(e_a.begin());
  for (auto e : e_a) {
    std::cout << e << " ";
  }
  std::cout << std::endl;

  // auto pos = std::find(e_a.begin(), e_a.end(), 0);

  // Test vector()
  ivarnd::vector<int> v_a(10, 5);
  for (auto e : v_a) {
    std::cout << e << " ";
  }
  std::cout << std::endl;

  ivarnd::vector<int> v_b(v_a.begin(), v_a.end());
  for (auto e : v_a) {
    std::cout << e << " ";
  }
  std::cout << std::endl;

  // Test sort
  int arr[] = {3, 5, 6, 2, 1};
  ivarnd::vector<int> s_v(arr, arr + sizeof(arr) / sizeof(arr[0]));
  for (auto e : s_v) {
    std::cout << e << " ";
  }
  std::cout << std::endl;
  std::sort(s_v.begin(), s_v.end());
  for (auto e : s_v) {
    std::cout << e << " ";
  }
  std::cout << std::endl;

  // Test copy
  ivarnd::vector<int> c_a(10, 5);
  for (auto e : c_a) {
    std::cout << e << " ";
  }
  std::cout << std::endl;

  ivarnd::vector<int> c_b(c_a);
  for (auto e : c_b) {
    std::cout << e << " ";
  }
  std::cout << std::endl;

  /*
  ivarnd::vector<string> s_a(5, "hello");
  for (auto e : s_a) {
    std::cout << e << " ";
  }
  std::cout << std::endl;

  ivarnd::vector<string> s_b(s_a);
  for (auto e : s_b) {
    std::cout << e << " ";
  }
  std::cout << std::endl;
  */

  /*
  // Test vector<vector<int>>
  ivarnd::vector<ivarnd::vector<int>> v_v(5);
  for (size_t i = 0; i < v_v.size(); ++i) {
    for (size_t j = 0; j < v_v[i].size(); ++j) {
      std::cout << v_v[i][j] << " ";
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
  */

  return 0;
}