#include "iterator.h"

#include <iostream>

int main() {
  std::vector<int> my_vector{21, 31, 41, 51, 61};
  SequenceIterator<std::vector<int>::iterator, std::vector<int>> my_vec_iterator(
      my_vector.begin());
  std::deque<int> my_deque{21, 31, 41, 51, 61};
  SequenceIterator<std::deque<int>::iterator, std::deque<int>> my_deque_iterator(
      my_deque.end());

  // for (; my_iterator != my_vector.end(); my_iterator++) {
  //  std::cout << *my_iterator << std::endl;
  // }
  std::cout << *(my_vec_iterator + 1) << std::endl;
  my_vec_iterator += 2;
  std::cout << *my_vec_iterator << std::endl;
  my_vec_iterator--;
  std::cout << *my_vec_iterator << std::endl;
  std::cout << "overload [] expression: " << my_vec_iterator[0] << std::endl;
  // std::cout << (my_vec_iterator == my_vector.begin()) << std::endl;

  std::cout << *(my_deque_iterator - 1) << std::endl;
  my_deque_iterator -= 2;
  std::cout << *my_deque_iterator << std::endl;
  my_deque_iterator++;
  std::cout << *my_deque_iterator << std::endl;

  std::cout << "Hello Iterator!" << std::endl;
}