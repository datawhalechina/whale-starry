#include <iostream>
#include <list>
#include <vector>

// 开链法简易hashtable实现
// 定义节点
template <typename KeyType, typename ValueType>
class HashNode {
 public:
  KeyType key;
  ValueType value;

  HashNode(const KeyType& k, const ValueType& v) : key(k), value(v) {}
};

template <typename KeyType, typename ValueType, typename Hash = std::hash<KeyType>>
class HashTable {
 private:
  using Bucket = std::list<HashNode<KeyType, ValueType>>;
  std::vector<Bucket> buckets;  // 使用vector来存储桶元素，和stl一致
  size_t size;

 public:
  HashTable(size_t num_buckets = 10) : size(0) { buckets.resize(num_buckets); }

  // 插入键值对
  void insert(const KeyType& key, const ValueType& value) {
    size_t index = hash_function(key) % buckets.size();
    Bucket& bucket = buckets[index];

    // 判断hashtable中是否存在了相同的key，更新值
    for (auto& node : bucket) {
      if (node.key == key) {
        std::cout << "Key already exists! Updating value." << std::endl;
        node.value = value;
        return;
      }
    }

    bucket.emplace_back(key, value);
    size++;
  }

  // 根据key查找对应的value
  bool find(const KeyType& key, ValueType& value) const {
    size_t index = hash_function(key) % buckets.size();
    const Bucket& bucket = buckets[index];

    for (const auto& node : bucket) {
      if (node.key == key) {
        value = node.value;
        return true;
      }
    }

    return false;
  }

  // 移除键值对
  bool remove(const KeyType& key) {
    size_t index = hash_function(key) % buckets.size();
    Bucket& bucket = buckets[index];

    for (auto it = bucket.begin(); it != bucket.end(); ++it) {
      if (it->key == key) {
        bucket.erase(it);
        size--;
        return true;
      }
    }

    return false;
  }

  size_t getSize() const { return size; }

 private:
  Hash hash_function;
};

int main() {
  HashTable<std::string, int> ht;

  ht.insert("roger", 24);
  ht.insert("tom", 25);
  ht.insert("roger", 3);
  ht.insert("jack", 27);

  int value;
  if (ht.find("roger", value)) {
    std::cout << "Found key 'roger' with value: " << value << std::endl;
  } else {
    std::cout << "Key 'roger' not found!" << std::endl;
  }

  if (ht.remove("jack")) {
    std::cout << "Key 'jack' removed successfully." << std::endl;
  } else {
    std::cout << "Key 'jack' not found! Removal failed." << std::endl;
  }

  std::cout << "Current size of the hashtable: " << ht.getSize() << std::endl;

  return 0;
}
