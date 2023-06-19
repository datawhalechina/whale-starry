#include <iostream>
#include <list>
using namespace std;

int main() {
  list<int> mylist;
  mylist.push_back(1);
  mylist.push_back(2);
  mylist.push_back(3);
  mylist.push_front(0);
  for (auto it = mylist.begin(); it != mylist.end(); ++it) {
    cout << *it << " ";
  }
  cout << endl;
  mylist.remove(2);
  for (auto it = mylist.begin(); it != mylist.end(); ++it) {
    cout << *it << " ";
  }
  cout << endl;

  return 0;
}