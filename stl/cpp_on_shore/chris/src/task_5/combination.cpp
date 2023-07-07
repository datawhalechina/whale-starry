#include "combination.h"

void PrintVector(std::vector<ShoppingOrder*> orderVector) {
  for (const auto& order : orderVector) {
    std::cout << "orderNumber: " << order->GetOrderNumber() << "\n"
              << "customerName: " << order->GetCustomerName() << "\n"
              << "status: " << order->GetStatus() << std::endl;
    std::cout << std::endl;
  }
  std::cout << "************************Done*****************************" << std::endl;
}

int main() {
  std::vector<ShoppingOrder*> orderVector;
  std::list<ShoppingOrder*> orderList;
  ShoppingOrder* order1 = new ShoppingOrder(1, "Alice", "Pending");
  orderVector.emplace_back(order1);

  // use list to realize a container that is similar to a stack
  orderList.push_front(order1);

  ShoppingOrder* order2 = new ShoppingOrder(2, "Bob", "Shipped");
  orderVector.emplace_back(order2);
  orderList.push_front(order2);

  ShoppingOrder* order3 = new ShoppingOrder();
  // Here is a Shallow Copy.
  order3 = order2;
  order3->SetOrderNumber(3);
  order3->SetCustomerName("Chris");
  // When we change the number of order3, order2 has also been changed.

  orderVector.push_back(order3);
  orderList.push_front(order3);

  std::cout << "orderNumber: " << order1->GetOrderNumber() << "\n"
            << "customerName: " << order1->GetCustomerName() << "\n"
            << "status: " << order1->GetStatus() << std::endl;
  std::cout << std::endl;

  for (const auto& order : orderList) {
    std::cout << "orderNumber: " << order->GetOrderNumber() << "\n"
              << "customerName: " << order->GetCustomerName() << "\n"
              << "status: " << order->GetStatus() << std::endl;
    std::cout << std::endl;
  }

  /*
  for (const auto& order : orderVector) {
   std::cout << "orderNumber: " << order->GetOrderNumber() << "\n"
    << "customerName: " << order->GetCustomerName() << "\n"
    << "status: " << order->GetStatus() << std::endl;
   std::cout << std::endl;
  }
  */
  PrintVector(orderVector);

  delete order1;
  delete order2;
  // delete order3;

  std::cout << "Hello World!\n";
}