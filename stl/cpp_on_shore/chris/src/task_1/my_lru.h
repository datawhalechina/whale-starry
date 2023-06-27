#include <iostream>
#include <list>
#include <string>
#include <unordered_map>

template <class K, class V>
struct Node {
  K key;
  V val;
};

template <class K, class V, class N>
class lru {
 public:
  lru(int capacity) { this->size = capacity; }
  void setVal(K key, V val) {
    if (nodes.size() == this->size) {
      // when buffer is full
      int del_key = nodes.back().key;
      std::cout << "del " << del_key << std::endl;
      std::cout << "front key" << nodes.front().key << std::endl;
      nodes.erase(map[del_key]);  // delete the iterator
      map.erase(del_key);         // delete the least value
    }
    Node<K, V> p;
    p.key = key;
    p.val = val;
    nodes.push_front(p);       // add at the front end
    map[key] = nodes.begin();  // keep the iterator of this added node
  }
  V getVal(K key) {
    Node<K, V> new_node;
    try {
      new_node = *map.at(key);
    } catch (const std::exception&) {
      return "Error: this key&val has been deleted from the buffer";
    }
    nodes.erase(map[key]);
    nodes.push_front(new_node);
    map[key] = nodes.begin();

    return new_node.val;
  }

 private:
  std::list<Node<K, V>> nodes;
  std::unordered_map<K, N> map;  // iterator
  int size;
};