#include <iostream>
#include <unordered_map>

void print_map(std::unordered_map<std::string, int>& hashtable) {
  // 输出哈希表中的所有键值对
  std::cout << "Hashtable contents:" << std::endl;
  for (const auto& pair : hashtable) {
    std::cout << "Key: " << pair.first << ", Value: " << pair.second << std::endl;
  }
}

void check(std::string key, std::unordered_map<std::string, int>& hashtable) {
  auto it = hashtable.find(key);
  if (it != hashtable.end()) {
    std::cout << "Value for key " << key << ": " << it->second << std::endl;
  } else {
    std::cout << "Key not found: " << key << std::endl;
  }
}

int main() {
  // 创建一个哈希表，键为字符串类型，值为整数类型
  std::unordered_map<std::string, int> hashtable;

  // 插入键值对
  hashtable["sheldon"] = 10;
  hashtable["roger"] = 5;
  hashtable["chris"] = 8;

  // 查找元素并输出值
  std::string key = "sheldon";
  check(key, hashtable);

  // 查找不存在的元素
  key = "somebody";
  check(key, hashtable);

  // 插入元素
  key = "somebody";
  hashtable[key] = 6;
  check(key, hashtable);

  // 删除元素
  key = "sombody";
  size_t num_elements = hashtable.erase(key);
  if (num_elements > 0) {
    std::cout << "Key " << key << " removed." << std::endl;
  } else {
    std::cout << "Key not found: " << key << std::endl;
  }

  print_map(hashtable);

  return 0;
}
