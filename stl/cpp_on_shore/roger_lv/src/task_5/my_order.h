#include <iostream>
#include <list>
#include <map>
#include <vector>

// 我们希望做一个订单管理系统于处理电子商务平台上的订单流程。
// 系统需要能够追踪订单的状态、管理订单的生命周期以及提供统计数据和报告。
// 显然我们希望能够通过订单编号能查询到订单的信息。因此我们需要一个map来处理映射，同时还能够根据时间排序
// 为了能够处理订单的生命周期，我们需要一个list来处理订单变化，我希望list存储订单总量，只要用户没有收货就不变
// 为了能够提供统计数据和报告，我们需要一个vector来处理订单的统计数据

class Order {
 public:
  Order() = default;
  Order(int id, std::string name, std::string status) {
    this->order_id = id;
    this->order_name = name;
    this->order_status = status;
  }
  void PrintInfo() const {
    std::cout << "order_id: " << order_id << " order_name: " << order_name;
    std::cout << " order_status: " << order_status << std::endl;
  }
  int GetId() const { return order_id; }
  std::string GetName() const { return order_name; }
  bool operator<(const Order& rhs) const { return order_id < rhs.order_id; }

 private:
  int order_id;
  std::string order_name;
  std::string order_status;
};

class System {
 public:
  void AddOrder(int id, std::string name, std::string status) {
    Order new_order(id, name, status);
    std::cout << "Current order infos: " << std::endl;
    new_order.PrintInfo();
    order_map[id] = new_order;
    order_list.push_back(new_order);
    order_vector.push_back(new_order);
  }
  void DelOrder(int id) {
    order_map.erase(id);
    for (auto it = order_vector.begin(); it != order_vector.end(); ++it) {
      if (it->GetId() == id) {
        order_vector.erase(it);
        break;
      }
    }
    for (auto it = order_list.begin(); it != order_list.end(); ++it) {
      if (it->GetId() == id) {
        order_list.erase(it);
        break;
      }
    }
  }
  void SearchOrder(int id) {
    if (order_map.find(id) != order_map.end()) {
      order_map[id].PrintInfo();
    } else {
      std::cout << "Order not found" << std::endl;
    }
  }
  void ShowAll() {
    std::cout << "Order List now in system: " << std::endl;
    for (auto it = order_list.begin(); it != order_list.end(); ++it) {
      it->PrintInfo();
    }
  }
  void CompleteOrder(int id) {
    if (order_map.find(id) != order_map.end()) {
      for (auto it = order_list.begin(); it != order_list.end(); ++it) {
        if (it->GetId() == id) {
          order_list.erase(it);
          break;
        }
      }
    }
    ShowAll();
  }
  void CountOrder(std::string name) {
    int count = 0;
    for (auto it = order_vector.begin(); it != order_vector.end(); ++it) {
      if (it->GetName() == name) {
        count++;
      }
    }
    std::cout << "Order " << name << " count: " << count << std::endl;
  }

 private:
  std::map<int, Order> order_map;
  std::list<Order> order_list;
  std::vector<Order> order_vector;
};