#include <functional>
#include <iostream>
#include <list>
#include <vector>

template <typename Key, typename Value>
class HashTable {
 public:
  // 构造函数，初始化哈希表大小
  HashTable(size_t size = 10) { table.resize(size); }

  // 插入键值对
  void insert(const Key& key, const Value& value) {
    size_t index = getBucketIndex(key);
    table[index].push_back(std::make_pair(key, value));
  }

  // 查找键对应的值
  Value* find(const Key& key) {
    size_t index = getBucketIndex(key);
    for (auto& kvp : table[index]) {
      if (kvp.first == key) {
        return &kvp.second;
      }
    }
    return nullptr;  // 没找到返回 nullptr
  }

  // 删除键值对
  void erase(const Key& key) {
    size_t index = getBucketIndex(key);
    auto& bucket = table[index];
    for (auto it = bucket.begin(); it != bucket.end(); ++it) {
      if (it->first == key) {
        bucket.erase(it);
        break;
      }
    }
  }

  // 打印哈希表内容
  void print() {
    for (size_t i = 0; i < table.size(); ++i) {
      std::cout << "Bucket " << i << ": ";
      for (const auto& kvp : table[i]) {
        std::cout << "(" << kvp.first << ", " << kvp.second << ") ";
      }
      std::cout << std::endl;
    }
  }

 private:
  // 哈希桶，使用链表存储冲突的元素
  using Bucket = std::list<std::pair<Key, Value>>;

  // 哈希表内部存储结构
  std::vector<Bucket> table;
  std::hash<Key> hashFunc;

  // 获取桶的索引
  size_t getBucketIndex(const Key& key) const { return hashFunc(key) % table.size(); }
};

int main() {
  HashTable<std::string, int> ht;

  ht.insert("chris", 5);
  ht.insert("roger", 3);
  ht.insert("sheldon", 8);
  ht.insert("lightcity", 10);

  ht.print();

  int* pValue = ht.find("somebody");
  if (pValue)
    std::cout << "Value of 'somebody': " << *pValue << std::endl;
  else
    std::cout << "'somebody' not found!" << std::endl;

  ht.erase("lightcity");
  ht.print();

  return 0;
}
