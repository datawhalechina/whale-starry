#include <functional>
#include <iostream>
#include <list>
#include <vector>

// 根据学习到的hashtable写一个最简易的散列表
template <typename Key, typename Value>
class HashTable {
 private:
  std::vector<std::list<std::pair<Key, Value>>> buckets;
  std::hash<Key> hash_function;

 public:
  HashTable(size_t size) : buckets(size) {}

  void insert(const Key& key, const Value& value) {
    size_t index = hash_function(key) % buckets.size();
    auto& bucket = buckets[index];

    for (auto& pair : bucket) {
      if (pair.first == key) {
        pair.second = value;
        return;
      }
    }

    bucket.emplace_back(key, value);
  }

  bool find(const Key& key, Value& value) {
    size_t index = hash_function(key) % buckets.size();
    auto& bucket = buckets[index];

    for (const auto& pair : bucket) {
      if (pair.first == key) {
        value = pair.second;
        return true;
      }
    }

    return false;
  }

  void erase(const Key& key) {
    size_t index = hash_function(key) % buckets.size();
    auto& bucket = buckets[index];

    for (auto it = bucket.begin(); it != bucket.end(); ++it) {
      if (it->first == key) {
        bucket.erase(it);
        return;
      }
    }
  }
};

int main() {
  HashTable<std::string, int> myHashtable(10);

  myHashtable.insert("apple", 5);
  myHashtable.insert("banana", 2);
  myHashtable.insert("orange", 8);

  int value;
  if (myHashtable.find("apple", value)) {
    std::cout << "The value of 'apple' is: " << value << std::endl;
  }

  myHashtable.erase("orange");

  if (!myHashtable.find("orange", value)) {
    std::cout << "The key 'orange' does not exist in the hashtable." << std::endl;
  }

  return 0;
}
