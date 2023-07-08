#include <iostream>
#include <list>
#include <map>
#include <string>
#include <vector>

// 增删改查
class ShoppingOrder {
 public:
  ShoppingOrder() {}
  ShoppingOrder(int given_order_number, std::string given_customer_name,
                std::string given_status, int price)
      : order_number(given_order_number),
        customer_name(given_customer_name),
        status(given_status),
        order_price(price){};
  ~ShoppingOrder() {}

  /*
  // Shallow Copy
  ShoppingOrder(const ShoppingOrder& shoppingOrder) {
   this->orderNumber = shoppingOrder.orderNumber;
   this->customerName = shoppingOrder.customerName;
   this->status = shoppingOrder.status;
  }
  */

  // To avoid shallow copy function, we use =delete
  ShoppingOrder(const ShoppingOrder& shoppingOrder) =
      delete;  // We do not use copy functions.
  ShoppingOrder& operator=(const ShoppingOrder& shoppingOrder) =
      delete;  // Also ban the "=" operator.

  // delete
  void DeleteOrder() {
    this->SetOrderNumber(-1);
    this->SetCustomerName("");
    this->SetStatus("");
    this->SetPrice(0);
  }

  void SetOrderNumber(int number) { this->order_number = number; }
  void SetCustomerName(std::string name) { this->customer_name = name; }
  void SetStatus(std::string cusStatus) { this->status = cusStatus; }
  void SetPrice(int price) { this->order_price = price; }

  int GetOrderNumber() { return this->order_number; }
  std::string GetCustomerName() { return this->customer_name; }
  std::string GetStatus() { return this->status; }
  int GetPrice() { return this->order_price; }

  /*
   至此, ShoppingOrder内基本的
   增、删、改、查等操作都已完成
  */

 private:
  int order_number;
  std::string customer_name;
  std::string status;
  int order_price;
};

class OrderManagementSys {
 public:
  OrderManagementSys() {
    ShoppingOrder* order1 = new ShoppingOrder(1, "Alice", "Pending", 0);
    order_vector.emplace_back(order1);

    // use list to realize a container that is similar to a stack
    order_list.push_front(order1);
    // pay attention to the usage of {} in map data structure
    order_map.insert({order1->GetOrderNumber(), order1});

    ShoppingOrder* order2 = new ShoppingOrder(2, "Bob", "Shipped", 150);
    order_vector.emplace_back(order2);
    order_list.push_front(order2);
    order_map.insert({order2->GetOrderNumber(), order2});

    ShoppingOrder* order3 = new ShoppingOrder(3, "Chris", "Paid", 100);

    /*
     Here is a Shallow Copy.
     // order3 = order2;
     // order3->SetOrderNumber(3);
     // order3->SetCustomerName("Chris");
     // When we change the number of order3, order2 has also been changed.
    */
    order_vector.push_back(order3);
    order_list.push_front(order3);
    order_map.insert({order3->GetOrderNumber(), order3});

    ShoppingOrder* order4 = new ShoppingOrder(4, "Doggo", "Received", 300);
    order_vector.emplace_back(order4);
    order_list.push_front(order4);
    order_map.insert({order4->GetOrderNumber(), order4});

    ShoppingOrder* order5 = new ShoppingOrder(5, "Elk", "Rejected", -100);
    order_vector.emplace_back(order5);
    order_list.push_front(order5);
    order_map.insert({order5->GetOrderNumber(), order5});

    ShoppingOrder* order6 = new ShoppingOrder(6, "Francisco", "Shipped", 360);
    order_vector.emplace_back(order6);
    order_list.push_front(order6);
    order_map.insert({order6->GetOrderNumber(), order6});

    ShoppingOrder* order7 = new ShoppingOrder(7, "Gaus", "Paid", 240);
    order_vector.emplace_back(order7);
    order_list.push_front(order7);
    order_map.insert({order7->GetOrderNumber(), order7});
    ShoppingOrder* order8 = new ShoppingOrder(8, "Huffman", "Rejected", -200);
    order_vector.emplace_back(order8);
    order_list.push_front(order8);
    order_map.insert({order8->GetOrderNumber(), order8});

    ShoppingOrder* order9 = new ShoppingOrder(9, "Ivy", "Recieved", 400);
    order_vector.emplace_back(order9);
    order_list.push_front(order9);
    order_map.insert({order9->GetOrderNumber(), order9});

    order9->DeleteOrder();

    ShoppingOrder* order10 = new ShoppingOrder(10, "Jesse", "Shipped", 500);
    order_vector.emplace_back(order10);
    order_list.push_front(order10);
    order_map.insert({order10->GetOrderNumber(), order10});

    Print(order_vector);
    std::cout << "The next print result is a reverse version of the order sequence!."
              << std::endl;
    std::cout << "******************************************************************"
              << std::endl;
    Print(order_list);

    std::cout << "******************************************************************"
              << std::endl;
    GetStatus();

    std::cout << "There is " << order_vector.size() << " orders. "
              << "\n";
    std::cout << "And the summary income of all orders is " << IncomeSum() << "."
              << std::endl;

    delete order1;
    delete order2;
    delete order3;
    delete order4;
    delete order5;
    delete order6;
    delete order7;
    delete order8;
    delete order9;
    delete order10;
  }

  int IncomeSum() {
    int summary = 0;
    for (auto it = order_map.begin(); it != order_map.end(); it++) {
      summary += it->second->GetPrice();
    }
    return summary;
  }
  ~OrderManagementSys() {
    std::cout << "*********************Destructor****************" << std::endl;
    std::cout << "***********************************************" << std::endl;
    std::cout << "****This is the end of Order Manage System.****" << std::endl;
  }
  void GetStatus() {
    // for (auto it = orderMap.)
    std::cout << "This is a function to print order statuses in sequence." << std::endl;
    for (auto it = order_map.begin(); it != order_map.end(); it++) {
      std::cout << "The number is " << it->first << ", and the status is "
                << it->second->GetStatus() << std::endl;
    }
  }

  template <class T>
  void Print(T order_container) {
    for (const auto& order : order_container) {
      std::cout << "orderNumber: " << order->GetOrderNumber() << "\n"
                << "customerName: " << order->GetCustomerName() << "\n"
                << "status: " << order->GetStatus() << std::endl;
      std::cout << std::endl;
    }
    std::cout << "************************Done*****************************" << std::endl;
  }

 private:
  std::vector<ShoppingOrder*>
      order_vector;  // use the vector to save the order in sequence
  std::list<ShoppingOrder*>
      order_list;  // use the list to save the order in a reverse sequence
  std::map<int, ShoppingOrder*>
      order_map;  // to establish a map according to the number and the order
};
