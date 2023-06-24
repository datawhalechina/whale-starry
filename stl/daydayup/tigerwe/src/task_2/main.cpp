#include "tigerwe_vector.h"

int main() {
  vector<int> v;
  v.push_back(1);
  v.push_back(2);
  v.push_back(3);

  std::cout << "Size: " << v.size() << std::endl;
  std::cout << "Elements:";
  for (auto it = v.begin(); it != v.end(); ++it) {
    std::cout << " " << *it << "(forward)";
  }
  std::cout << std::endl;

  std::cout << "Reverse elements:";
  for (auto it = v.rbegin(); it != v.rend(); ++it) {
    std::cout << " " << *it << "(backward)";
  }
  std::cout << std::endl;

  v.pop_back();
  std::cout << "After pop_back(), size: " << v.size() << ", back element: " << v.back()
            << std::endl;
  // 还存在问题
  // auto it = v.begin() + 1;
  // v.erase(it);

  // std::cout << "After erase(), size: " << v.size() << ", second element: " << v[1] <<
  // std::endl;

  std::cout << "Front element: " << v.front() << ", back element: " << v.back()
            << std::endl;

  v.clear();
  std::cout << "Size after clear: " << v.size() << std::endl;

  return 0;
}