#include <iostream>
#include <vector>

int main() {
  std::vector<int> vector1 = {1, 2, 3, 4, 5};
  std::vector<int> vector2{6, 7, 8, 9, 10};
  std::vector<int> vector3(5, 12);
  std::cout << "vector1 = ";

  for (const int& i : vector1) {
    std::cout << i << "  ";
  }

  std::vector<int> num{1, 2, 3, 4, 5};

  std::cout << "Initial Vector: ";

  for (const int& i : num) {
    std::cout << i << "  ";
  }

  num.push_back(6);
  num.push_back(7);

  std::cout << "\nUpdated Vector: ";

  for (const int& i : num) {
    std::cout << i << "  ";
  }

  std::cout << "Element at Index 0: " << num.at(0) << std::endl;
  std::cout << "Element at Index 2: " << num.at(2) << std::endl;
  std::cout << "Element at Index 4: " << num.at(4);

  std::cout << "Initial Vector: ";

  for (const int& i : num) {
    std::cout << i << "  ";
  }

  // change elements at indexes 1 and
  num.at(1) = 9;
  num.at(4) = 7;

  std::cout << "\nUpdated Vector: ";

  for (const int& i : num) {
    std::cout << i << "  ";
  }

  std::cout << "Initial Vector: ";
  for (int i : num) {
    std::cout << i << " ";
  }

  // remove the last element
  num.pop_back();

  // final vector
  std::cout << "\nUpdated Vector: ";
  for (int i : num) {
    std::cout << i << " ";
  }

  // declare iterator
  std::vector<int>::iterator iter;

  // initialize the iterator with the first element
  iter = num.begin();

  // print the vector element
  std::cout << "num[0] = " << *iter << std::endl;

  // iterator points to the 3rd element
  iter = num.begin() + 2;
  std::cout << "num[2] = " << *iter;

  // iterator points to the last element
  iter = num.end() - 1;
  std::cout << "num[4] = " << *iter;

  return 0;
}