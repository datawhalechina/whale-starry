#include <algorithm>
#include <iostream>
#include <list>

int main() {
  std::list<int> m_list;

  m_list.push_back(4);
  m_list.push_back(5);
  m_list.push_back(6);
  m_list.push_front(3);
  m_list.push_front(2);
  m_list.push_front(1);

  std::cout << "Init:" << std::endl;
  for (const auto& data : m_list) {
    std::cout << data << " ";
  }
  std::cout << std::endl;

  m_list.insert(++m_list.begin(), 10);
  std::cout << "Insert:" << std::endl;
  for (const auto& data : m_list) {
    std::cout << data << " ";
  };
  std::cout << std::endl;

  std::cout << "Delete:" << std::endl;
  m_list.erase(std::find(m_list.begin(), m_list.end(), 4));
  for (const auto& data : m_list) {
    std::cout << data << " ";
  };
  std::cout << std::endl;

  return 0;
}
