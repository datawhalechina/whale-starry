#include <algorithm>
#include <chrono>
#include <execution>
#include <iostream>
#include <map>
#include <random>
#include <string>
#include <vector>

#include "BSTree.h"
#include "RBTree.h"

namespace {
constexpr int DATA_COUNT = 1000000;
constexpr int INDEX = 435435;
}  // namespace

int main() {
  std::cout << "type\t|\t插入耗时\t|\t查询耗时\t" << std::endl;
  std::vector<int> data(DATA_COUNT);
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(1, DATA_COUNT);
  std::generate(data.begin(), data.end(), [&]() { return dis(gen); });

  // 排序二叉树
  BSTree<int32_t, int32_t> bs;
  auto start = std::chrono::high_resolution_clock::now();
  for (auto& i : data) {
    bs.Insert(i, i + 1);
  }
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = end - start;
  std::cout << "BSTree\t|\t" << elapsed.count() << "\t|\t";

  start = std::chrono::high_resolution_clock::now();
  bs.Find(data[INDEX]);
  end = std::chrono::high_resolution_clock::now();
  elapsed = end - start;
  std::cout << elapsed.count() << std::endl;

  // std::map
  std::map<int32_t, int32_t> mm;
  start = std::chrono::high_resolution_clock::now();
  for (auto& i : data) {
    mm.insert({i, i + 1});
  }
  end = std::chrono::high_resolution_clock::now();
  elapsed = end - start;
  std::cout << "std::map|\t" << elapsed.count() << "\t|\t";

  start = std::chrono::high_resolution_clock::now();
  mm.find(data[INDEX]);
  end = std::chrono::high_resolution_clock::now();
  elapsed = end - start;
  std::cout << elapsed.count() << std::endl;

  // 红黑树
  RBTree<int32_t, int32_t> rb;
  start = std::chrono::high_resolution_clock::now();
  for (auto& i : data) {
    rb.Insert(i, i + 1);
  }
  end = std::chrono::high_resolution_clock::now();
  elapsed = end - start;
  std::cout << "RBTree\t|\t" << elapsed.count() << "\t|\t";

  start = std::chrono::high_resolution_clock::now();
  rb.Find(data[INDEX]);
  end = std::chrono::high_resolution_clock::now();
  elapsed = end - start;
  std::cout << elapsed.count() << std::endl;

  return 0;
}