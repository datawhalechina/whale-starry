#include <algorithm>
#include <ctime>
#include <functional>
#include <iostream>
#include <list>

// 以下为测试结果，单位秒
// 1.9787 2.6895 2.1298
void test_list_insert(std::list<int>& l) {
  for (int i = 0; i < (int)1e7; ++i) l.push_back(i);
}

// 在1e7个数中找随机数，时间单位秒
// 1.3307 3.0759 0.6905
void test_list_search(std::list<int>& l) {
  int t;
  srand((unsigned)time(NULL));
  t = rand() % (int)1e7;
  std::find(l.begin(), l.end(), t);
}

// 从小到大排序
// 找0花费时间：4e-06 找1e7-1花费时间：0.8823 找中间位置花费的时间：0.3623
void test_sorted_list_search(std::list<int>& l) {
  std::find(l.begin(), l.end(), (int)(1e7 - 1) / 2);
}

int main() {
  std::list<int> l;
  clock_t stime = clock();
  test_list_insert(l);  // 插入1e7个数据
  std::cout << (double)(clock() - stime) / CLOCKS_PER_SEC << std::endl;

  stime = clock();
  test_list_search(l);  // 在1e7个数据中找随机数
  std::cout << (double)(clock() - stime) / CLOCKS_PER_SEC << std::endl;

  l.sort(std::less<int>());
  stime = clock();
  test_sorted_list_search(l);  // 有序链表计算头部、中间、尾部搜索时间
  std::cout << (double)(clock() - stime) / CLOCKS_PER_SEC << std::endl;

  return 0;
}