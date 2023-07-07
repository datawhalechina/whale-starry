#include <iostream>
#include <list>
#include <map>
#include <string>
#include <vector>

// 增删改查
class ShoppingOrder {
 public:
  ShoppingOrder() {}
  ShoppingOrder(int givenOrderNumber, std::string givenCustomerName,
                std::string givenStatus)
      : orderNumber(givenOrderNumber),
        customerName(givenCustomerName),
        status(givenStatus){};

  // Shallow Copy
  ShoppingOrder(const ShoppingOrder& shoppingOrder) {
    this->orderNumber = shoppingOrder.orderNumber;
    this->customerName = shoppingOrder.customerName;
    this->status = shoppingOrder.status;
  }

  void SetOrderNumber(int number) { this->orderNumber = number; }
  void SetCustomerName(std::string name) { this->customerName = name; }
  void SetStatus(std::string cusStatus) { this->status = cusStatus; }

  int GetOrderNumber() { return this->orderNumber; }
  std::string GetCustomerName() { return this->customerName; }
  std::string GetStatus() { return this->status; }

 private:
  int orderNumber;
  std::string customerName;
  std::string status;
};
