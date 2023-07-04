#include <iostream>
#include <map>
#include <string>

// 由于map本身接口比较简单，本人最近实习也比较不好摸鱼，因此这边简单写一个购物车与商品的应用测试
// 商品类
class Product {
 private:
  std::string name;
  double price;

  // 需要注意的是只要是在map中使用的自定义类，其成员函数必须以const结尾
 public:
  Product(const std::string& name, double price) : name(name), price(price) {}

  std::string GetName() const { return name; }

  double GetPrice() const { return price; }

  bool operator<(const Product& other) const { return this->price < other.price; }
};

// 购物车类
class ShoppingCart {
 private:
  std::map<Product, int> items;  // 使用map存储商品及其数量

 public:
  void AddItem(const Product& product, int quantity) {
    items[product] += quantity;  // 将商品及其数量加入购物车
  }

  void RemoveItem(const Product& product, int quantity) {
    auto it = items.find(product);
    if (it != items.end()) {
      it->second -= quantity;  // 从购物车中减去指定数量的商品
      if (it->second <= 0) {
        items.erase(it);  // 如果商品数量为0或负数，则从购物车中移除该商品
      }
    }
  }

  void DisplayItems() const {
    for (const auto& pair : items) {
      const Product& product = pair.first;
      int quantity = pair.second;
      std::cout << "Product: " << product.GetName() << ", Quantity: " << quantity;
      std::cout << std::endl;
    }
  }
};

int main() {
  Product product1("Item 1", 10.99);
  Product product2("Item 2", 5.99);
  Product product3("Item 3", 7.99);

  ShoppingCart cart;

  cart.AddItem(product1, 2);
  cart.AddItem(product2, 3);
  cart.AddItem(product3, 1);

  cart.DisplayItems();

  cart.RemoveItem(product2, 2);

  cart.DisplayItems();

  return 0;
}
