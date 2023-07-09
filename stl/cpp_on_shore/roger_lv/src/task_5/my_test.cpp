#include "my_order.h"

int main() {
  System sys;
  sys.AddOrder(1001, "computer", "created");
  sys.AddOrder(1002, "computer", "processing");
  sys.AddOrder(1003, "apple", "on way");
  sys.ShowAll();
  sys.DelOrder(1003);
  sys.ShowAll();
  sys.CountOrder("computer");
  sys.CompleteOrder(1001);
  return 0;
}