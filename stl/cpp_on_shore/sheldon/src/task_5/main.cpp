#include "orderSystem.hpp"

int main() {
  OrderSystem OS;
  OS.init();
  OS.print_set();
  OS.print_map();
  OS.print_list();
  int key = 1;
  std::cout << "Access key == " << key << " , status is " << OS.get_status(key)
            << std::endl;
  OS.print_list();
  key = 4;
  std::cout << "Access key == " << key << " , status is " << OS.get_status(key)
            << std::endl;
  OS.print_list();
  std::cout << "----INSERT----" << std::endl;
  OS.add_order(7, "Somebody", "Pending", rand() % 500, rand() % 1000);
  OS.print_set();
  OS.print_map();
  OS.print_list();
  std::cout << "----DELETE----" << std::endl;
  OS.del_order(7);
  OS.print_set();
  OS.print_map();
  OS.print_list();
  std::cout << "----DELETE----" << std::endl;
  OS.del_order(4);
  OS.print_set();
  OS.print_map();
  OS.print_list();
  return 0;
}
