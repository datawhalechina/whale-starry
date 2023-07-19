#include <chrono>
#include <iostream>
#include <map>
#include <string>
#include <unordered_map>

class Timer {
 public:
  std::chrono::time_point<std::chrono::steady_clock> start, end;
  std::chrono::duration<float> last_time;

  // constructor
  Timer() { start = std::chrono::high_resolution_clock::now(); }
  // destructor
  ~Timer() {
    end = std::chrono::high_resolution_clock::now();
    last_time = end - start;
    float result_ms = last_time.count() * 1000.0f;
    std::cout << "Took " << result_ms << "ms" << std::endl;
  }
};

/*
    cost: 0.137237ms
*/
void SeeBasicUsage() {
  Timer timer;
  std::string key = "Albert";
  int value = 1;
  std::unordered_map<std::string, int> unordmap;
  unordmap.emplace(key, value);
  unordmap.emplace("Bob", 2);
  unordmap.emplace("Chris", 3);
  unordmap.emplace("Doggo", 4);
  unordmap.emplace("Ed", 5);
  unordmap.emplace("Flamming", 6);

  std::cout << "unordmap[Albert]: " << unordmap["Albert"] << ' '
            << "unordmap[Bob]: " << unordmap["Bob"] << std::endl;

  std::cout << "*****************************" << std::endl;
  // we can see that the order is messed up.
  for (auto elem : unordmap) std::cout << elem.first << " " << elem.second << std::endl;
  // the order here is also messed up.
  for (auto elem : unordmap)
    std::cout << "value is : " << unordmap[elem.first] << std::endl;

  std::cout << "********This is the end of test.**********" << std::endl;
}

/*
    cost: 0.059176ms
    We can find that adding elements and visiting all elements in map is faster
    than that in unordered_map.
*/
void SeeDiffWithMap() {
  Timer timer;
  std::string key = "Albert";
  int value = 1;
  std::map<std::string, int> ordmap;
  ordmap.emplace(key, value);
  ordmap.emplace("Bob", 2);
  ordmap.emplace("Chris", 3);
  ordmap.emplace("Doggo", 4);
  ordmap.emplace("Ed", 5);
  ordmap.emplace("Flamming", 6);

  std::cout << "ordmap[Albert]: " << ordmap["Albert"] << ' '
            << "ordmap[Bob]: " << ordmap["Bob"] << std::endl;

  std::cout << "*****************************" << std::endl;

  // In contrast, we can see the order is in sequence.
  for (auto elem : ordmap) std::cout << elem.first << " " << elem.second << std::endl;
  // Here is the same.
  for (auto elem : ordmap) std::cout << "value is : " << ordmap[elem.first] << std::endl;

  std::cout << "********This is the end of test.**********" << std::endl;
}

void TryInterface() {
  std::unordered_map<std::string, int> fruit_price;

  fruit_price["apple"] = 10;
  fruit_price.emplace("banana", 15);
  fruit_price["pineapple"] = 30;
  fruit_price.emplace("orange", 13);
  fruit_price["mango"] = 43;

  // find() test
  std::string key = "apple";
  if (fruit_price.find(key) == fruit_price.end()) {
    std::cout << key << " is not in our price list." << std::endl;
  } else {
    std::cout << key << " can be found in our price list." << std::endl;
  }

  key = "cherry";
  if (fruit_price.find(key) == fruit_price.end()) {
    std::cout << key << " is not in our price list." << std::endl;
  } else {
    std::cout << key << " can be found in our price list." << std::endl;
  }

  // erase() & count() test
  std::string key_delete = "apple";
  if (fruit_price.count(key_delete)) {
    fruit_price.erase(key_delete);
    std::cout << key_delete << " is deleted from our price list." << std::endl;
  } else {
    std::cout << key_delete << " is not found in our price list." << std::endl;
  }

  key_delete = "cherry";
  if (fruit_price.count(key_delete)) {
    fruit_price.erase(key_delete);
    std::cout << key_delete << " is deleted from our price list." << std::endl;
  } else {
    std::cout << key_delete << " is not found in our price list." << std::endl;
  }

  // insert() and iterator test
  std::pair<std::string, int> cherry_price("cherry", 65);
  std::unordered_map<std::string, int>::iterator iter;
  iter = fruit_price.insert(fruit_price.begin(), cherry_price);
  std::cout << "iter -> " << iter->first << " " << iter->second << std::endl;

  // we can see the apple has been removed and cherry has been inserted.
  for (auto elem : fruit_price)
    std::cout << "fruit: " << elem.first << " price: " << elem.second << std::endl;
  std::cout << "**********************************" << std::endl;
}