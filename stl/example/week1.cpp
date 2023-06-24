#include <iostream>
#include <list>
#include <map>
#include <vector>

struct Order {
  int orderNumber;
  std::string customerName;
  std::string status;
  // 其他订单信息...
};

int main() {
  std::vector<Order> orderVector;
  std::list<Order> orderList;
  std::map<int, Order> orderMap;

  // 添加订单到向量、列表和映射
  Order order1{1, "Alice", "Pending"};
  orderVector.push_back(order1);
  orderList.push_back(order1);
  orderMap.insert({order1.orderNumber, order1});

  Order order2{2, "Bob", "Shipped"};
  orderVector.push_back(order2);
  orderList.push_back(order2);
  orderMap.insert({order2.orderNumber, order2});

  // 通过订单号从映射中查找订单
  int orderNumber = 2;
  auto it = orderMap.find(orderNumber);
  if (it != orderMap.end()) {
    Order &order = it->second;
    std::cout << "Order " << orderNumber << ": " << order.customerName << " - "
              << order.status << std::endl;
  }

  // 遍历订单列表
  for (const auto &order : orderList) {
    std::cout << "Order " << order.orderNumber << ": " << order.customerName
              << " - " << order.status << std::endl;
  }

  return 0;
}
