#include <iostream>
#include <list>
#include <map>
#include <set>
#include <string>
#include <vector>

class Order {
 public:
  //  订单编号
  int orderNumber;
  // 顾客姓名
  std::string customerName;
  // 订单状态
  std::string status;
  // 订单价值
  int orderValue;
  // 订单周期
  int orderTime;

  Order(int n, std::string cn, std::string s, int ov, int ot)
      : orderNumber(n), customerName(cn), status(s), orderValue(ov), orderTime(ot) {}

  /* 排序规则：
   * 按照订单价值降序排列；
   * 如果订单价值相同，则根据订单时间降序排列
   */
  bool operator<(const Order& temp) const {
    if (orderValue == temp.orderValue) {
      return temp.orderTime < this->orderTime;
    } else {
      return temp.orderValue < this->orderValue;
    }
  }
  bool operator==(const Order& temp) const {
    return temp.orderNumber == this->orderNumber;
  }
};

class OrderSystem {
 private:
  std::vector<Order> vecOrder;
  std::list<Order> listOrder;
  std::set<Order> setOrder;
  std::map<int, Order> mapOrder;

 public:
  void init() {
    // 添加订单到向量、列表和映射
    Order order1 = Order(1, "Alice", "Pending", rand() % 500, rand() % 1000);
    vecOrder.push_back(order1);
    listOrder.push_front(order1);
    mapOrder.insert({order1.orderNumber, order1});
    setOrder.insert(order1);

    Order order2{2, "Bob", "Shipped", rand() % 500, rand() % 1000};
    vecOrder.push_back(order2);
    listOrder.push_front(order2);
    mapOrder.insert({order2.orderNumber, order2});
    setOrder.insert(order2);

    Order order3{3, "Chris", "Pending", rand() % 500, rand() % 1000};
    vecOrder.push_back(order3);
    listOrder.push_front(order3);
    mapOrder.insert({order3.orderNumber, order3});
    setOrder.insert(order3);

    Order order4{4, "Roger", "Shipped", rand() % 500, rand() % 1000};
    vecOrder.push_back(order4);
    listOrder.push_front(order4);
    mapOrder.insert({order4.orderNumber, order4});
    setOrder.insert(order4);

    Order order5{5, "Sheldon", "Shipped", rand() % 500, rand() % 1000};
    vecOrder.push_back(order5);
    listOrder.push_front(order5);
    mapOrder.insert({order5.orderNumber, order5});
    setOrder.insert(order5);

    Order order6{6, "LightCity", "Shipped", order5.orderValue, rand() % 1000};
    vecOrder.push_back(order6);
    listOrder.push_front(order6);
    mapOrder.insert({order6.orderNumber, order6});
    // mapOrder[6] = order6;
    setOrder.insert(order6);
  }

  // 增加订单
  void add_order(int n, std::string cn, std::string s, int ov, int ot) {
    Order temp{n, cn, s, ov, ot};
    vecOrder.push_back(temp);
    listOrder.push_front(temp);
    mapOrder.insert({temp.orderNumber, temp});
    setOrder.insert(temp);
  }

  // 删除指定订单编号的订单
  void del_order(int key) {
    auto it = mapOrder.find(key);
    std::cout << "DEL: " << mapOrder.at(key).customerName << std::endl;
    if (it != mapOrder.end()) {
      // std::cout << mapOrder.at(key).status << std::endl;
      // The bellow code doesn't work? why??
      // std::cout << mapOrder[key].status << std::endl;
      // 删除list里的order
      auto it_list = listOrder.begin();
      while (it_list != listOrder.end()) {
        if (*it_list == it->second) {
          auto iter_tmp_list = it_list;
          it_list++;
          if (it_list != listOrder.end()) {
            listOrder.erase(iter_tmp_list);
          } else {
            listOrder.pop_back();
            break;
          }

        } else {
          it_list++;
        }
      }

      // 删除vector里的order
      auto it_vec = vecOrder.begin();
      while (it_vec != vecOrder.end()) {
        if (*it_vec == it->second) {
          auto iter_tmp_vec = it_vec;
          it_vec++;
          if (it_vec != vecOrder.end()) {
            vecOrder.erase(iter_tmp_vec);
          } else {
            vecOrder.pop_back();
            break;
          }
        } else {
          it_vec++;
        }
      }

      // 删除set里的order
      setOrder.erase(it->second);

      // 删除map里的order
      mapOrder.erase(it);

    } else {
      std::cout << "Not Found the order of " << key << std::endl;
    }
  }

  // 按照id打印订单
  void print_map() {
    std::cout << "-----Print based on id--------" << std::endl;
    if (mapOrder.empty()) {
      std::cout << "mapOrder is empty" << std::endl;
    } else {
      for (auto p : mapOrder) {
        std::cout << "orderNum: " << p.second.orderNumber
                  << " customerName: " << p.second.customerName
                  << " orderValue: " << p.second.orderValue
                  << " orderTime: " << p.second.orderTime << std::endl;
      }
    }
  }
  // 按照价值打印订单
  void print_set() {
    std::cout << "-----Print based on value--------" << std::endl;
    if (setOrder.empty()) {
      std::cout << "setOrder is empty" << std::endl;
    } else {
      for (auto p : setOrder) {
        std::cout << "orderNum: " << p.orderNumber << " customerName: " << p.customerName
                  << " orderValue: " << p.orderValue << " orderTime: " << p.orderTime
                  << std::endl;
      }
    }
  }

  std::string get_status(int key) {
    auto it = mapOrder.find(key);
    if (it != mapOrder.end()) {
      // std::cout << mapOrder.at(key).status << std::endl;
      // The bellow code doesn't work? why??
      // std::cout << mapOrder[key].status << std::endl;
      auto it_list = listOrder.begin();
      while (it_list != listOrder.end()) {
        if (*it_list == it->second) {
          Order temp = *it_list;
          listOrder.erase(it_list);
          listOrder.push_front(temp);
        }
        it_list++;
      }
      return it->second.status;
    } else {
      std::cout << "Not Found the order of " << key << std::endl;
    }
  }

  // 按照最近访问打印
  void print_list() {
    std::cout << "-----Print based on recent access--------" << std::endl;
    if (listOrder.empty()) {
      std::cout << "setOrder is empty" << std::endl;
    } else {
      for (auto p : listOrder) {
        std::cout << "orderNum: " << p.orderNumber << " customerName: " << p.customerName
                  << " orderValue: " << p.orderValue << " orderTime: " << p.orderTime
                  << std::endl;
      }
    }
  }
};
