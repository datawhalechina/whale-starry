#include <iostream>
#include <vector>

int main() {
  std::vector<int> g1;

  for (int i = 1; i <= 5; i++) g1.push_back(i);

  std::cout << "Output of begin and end: ";
  for (auto i = g1.begin(); i != g1.end(); ++i) std::cout << *i << " ";

  std::cout << "\nOutput of cbegin and cend: ";
  for (auto i = g1.cbegin(); i != g1.cend(); ++i) std::cout << *i << " ";

  std::cout << "\nOutput of rbegin and rend: ";
  for (auto ir = g1.rbegin(); ir != g1.rend(); ++ir) std::cout << *ir << " ";

  std::cout << "\nOutput of crbegin and crend : ";
  for (auto ir = g1.crbegin(); ir != g1.crend(); ++ir) std::cout << *ir << " ";

  g1.clear();

  for (int i = 1; i <= 5; i++) g1.push_back(i);

  std::cout << "Size : " << g1.size();
  std::cout << "\nCapacity : " << g1.capacity();
  std::cout << "\nMax_Size : " << g1.max_size();

  // resizes the std::vector size to 4
  g1.resize(4);

  // prints the std::vector size after resize()
  std::cout << "\nSize : " << g1.size();

  // checks if thestd::vector is empty or not
  if (g1.empty() == false)
    std::cout << "\nVector is not empty";
  else
    std::cout << "\nVector is empty";

  // Shrinks the std::vector
  g1.shrink_to_fit();
  std::cout << "\nVector elements are: ";
  for (auto it = g1.begin(); it != g1.end(); it++) std::cout << *it << " ";

  g1.clear();

  for (int i = 1; i <= 10; i++) g1.push_back(i * 10);

  std::cout << "\nReference operator [g] : g1[2] = " << g1[2];

  std::cout << "\nat : g1.at(4) = " << g1.at(4);

  std::cout << "\nfront() : g1.front() = " << g1.front();

  std::cout << "\nback() : g1.back() = " << g1.back();

  // pointer to the first element
  int* pos = g1.data();

  std::cout << "\nThe first element is " << *pos;

  // Assign std::vector
  std::vector<int> v;

  // fill the std::vector with 10 five times
  v.assign(5, 10);

  std::cout << "The std::vector elements are: ";
  for (int i = 0; i < v.size(); i++) std::cout << v[i] << " ";

  // inserts 15 to the last position
  v.push_back(15);
  int n = v.size();
  std::cout << "\nThe last element is: " << v[n - 1];

  // removes last element
  v.pop_back();

  // prints the std::vector
  std::cout << "\nThe std::vector elements are: ";
  for (int i = 0; i < v.size(); i++) std::cout << v[i] << " ";

  // inserts 5 at the beginning
  v.insert(v.begin(), 5);

  std::cout << "\nThe first element is: " << v[0];

  // removes the first element
  v.erase(v.begin());

  std::cout << "\nThe first element is: " << v[0];

  // inserts at the beginning
  v.emplace(v.begin(), 5);
  std::cout << "\nThe first element is: " << v[0];

  // Inserts 20 at the end
  v.emplace_back(20);
  n = v.size();
  std::cout << "\nThe last element is: " << v[n - 1];

  // erases the std::vector
  v.clear();
  std::cout << "\nVector size after clear(): " << v.size();

  // two std::vector to perform swap
  std::vector<int> v1, v2;
  v1.push_back(1);
  v1.push_back(2);
  v2.push_back(3);
  v2.push_back(4);

  std::cout << "\n\nVector 1: ";
  for (int i = 0; i < v1.size(); i++) std::cout << v1[i] << " ";

  std::cout << "\nVector 2: ";
  for (int i = 0; i < v2.size(); i++) std::cout << v2[i] << " ";

  // Swaps v1 and v2
  v1.swap(v2);

  std::cout << "\nAfter Swap \nVector 1: ";
  for (int i = 0; i < v1.size(); i++) std::cout << v1[i] << " ";

  std::cout << "\nVector 2: ";
  for (int i = 0; i < v2.size(); i++) std::cout << v2[i] << " ";
}