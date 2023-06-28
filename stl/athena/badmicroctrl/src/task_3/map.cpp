#include <map>

int main() {
  std::map<int, std::string> c;
  c.insert(std::pair<int, std::string>(1, "Hello std::map"));
  std::cout << c[1] << std::endl;
  c.insert(std::map<int, std::string>::value_type(2, "second test"));
  std::cout << c[2] << std::endl;
  c[3] = "third test";
  std::cout << c[3] << std::endl;
  c.insert(std::pair<int, std::string>(1, "Hello std::map else"));
  std::cout << c[1] << std::endl;
  c.insert(std::map<int, std::string>::value_type(2, "second test else"));
  std::cout << c[2] << std::endl;
  c[3] = "fourth test";
  std::cout << c[3] << std::endl;
  std::map<int, std::string>::reverse_iterator iter;
  std::map<int, std::string>::iterator it = c.begin();
  c[6] = "my test";
  std::cout << "------" << std::endl;
  for (iter = c.rbegin(); iter != c.rend(); iter++) {
    std::cout << iter->first << "---" << iter->second << std::endl;
  }
  std::cout << "------" << std::endl;

  for (; it != c.end(); it++) {
    std::cout << it->first << "---" << it->second << std::endl;
  }
  std::cout << "------" << std::endl;
  int count = c.count(1);
  std::cout << count << std::endl;
  std::map<int, std::string>::iterator temp = c.find(1);
  std::cout << temp->first << "---" << temp->second << std::endl;
  std::cout << "------" << std::endl;
  c.insert({11, "abc11"});
  c.insert(std::make_pair(4, "ab4"));
  for (iter = c.rbegin(); iter != c.rend(); iter++) {
    std::cout << iter->first << "---" << iter->second << std::endl;
  }
}
