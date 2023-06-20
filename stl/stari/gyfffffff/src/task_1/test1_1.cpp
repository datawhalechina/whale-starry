// to have a look of  the size of any list
// can be revise to a template function ...
#include <iostream>
#include <list>

using std::cout;
using std::endl;

int main() {
  std::list<int> mylist(3, 5);

  cout << "mylist:" << endl;
  for (int x : mylist) {
    cout << x << " ";
  }
  cout << endl;
  cout << "sizeof mylist:" << sizeof(mylist) << endl;

  cout << "push_back(7),mylist:" << endl;
  mylist.push_back(7);
  for (int x : mylist) {
    cout << x << " ";
  }
  cout << endl;
  cout << "sizeof mylist:" << sizeof(mylist) << endl;

  mylist.clear();
  cout << "mylist is empty? " << mylist.empty() << endl;
  cout << "sizeof empty list:" << sizeof(mylist) << endl;

  std::list<double> mydoublelist(3, 5.2);
  cout << "mydoublelist:" << endl;
  for (double x : mydoublelist) {
    cout << x << " ";
  }
  cout << endl;
  cout << "sizeof mydoublelist:" << sizeof(mydoublelist) << endl;
}