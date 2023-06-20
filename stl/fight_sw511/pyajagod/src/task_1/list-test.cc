#include "include/list.h"
#include <iostream>

using namespace std;

int main() {
  int a[5] = {1, 2, 3, 4, 5};
  list<int> mylist;

  // test push_front
  for (int i = 0; i < 5; i++) {
    mylist.push_front(a[i]);
  }
  mylist.show();
  for (int i = 6; i < 10; i++) {
    mylist.push_back(i);
  }
  mylist.show();
  mylist.show_iterator();
  // cout << mylist.front() << endl;
  // cout << mylist.back() << endl;
}