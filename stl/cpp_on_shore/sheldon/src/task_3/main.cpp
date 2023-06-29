#include <iostream>
#include "my_set.hpp"

// 测试自定义接口
int main(){
  std::cout << "hello world" << std::endl;
  //std::_Rb_tree<int, int, std::_Identity<int>, std::less<int>> itree;
  my_set<int> m_set;
  m_set.insert(5);
  m_set.insert(50);
  m_set.insert(1);
  m_set.insert(78);
  m_set.insert(80);

  // 测试
  // std::cout << itree._M_impl._M_node_count << std::endl; 
  std::cout << m_set.size() << std::endl; 

  m_set.print();

  std::cout << m_set.exist(50) << std::endl; // 1
  std::cout << m_set.exist(100) << std::endl;  // 0

  std::cout << m_set.max() << std::endl; // 
  std::cout << m_set.min() << std::endl; // 

  m_set.erase(5);
  m_set.print();
  
  return 0;
}
