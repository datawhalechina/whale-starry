
#include "my_func.h"

bool comp(Player x, Player y) { return x.GetHeight() > y.GetHeight(); }

int main() {
  std::list<int> my_list1(2);  // 构造函数没传参，默认值为0
  my_list1.push_back(10);
  my_list1.push_back(20);
  MyList my_list2(my_list1);

  std::cout << "constructor 1:  " << std::endl;
  // 容器取值
  for (auto iter = my_list1.begin(); iter != my_list1.end(); ++iter) {
    std::cout << *iter << " ";
  }
  std::cout << std::endl;
  TestPushAndEmplaceTime(my_list1);  // 明显能看出emplace_back运行速度更快

  // 测试自定义运算符重载[]，希望实现和vector一样的索引取值方式
  std::cout << "my [] output: " << my_list2[2] << std::endl;

  // 测试自定义输出运算符<<，希望能直接通过cout打印list
  std::cout << "my << output: " << my_list2 << std::endl;

  // 测试自定义运动员类，添加到list中
  std::list<Player> player_list;
  Player LBJ;
  LBJ.SetBasicInfo(203, 140, 36, "LBJ", "Lakers", "SF", "all-round");
  std::cout << "This is LBJ's info: " << std::endl;
  LBJ.PrintBasicInfo();
  Player Kyrie;
  Kyrie.SetBasicInfo(191, 88, 29, "Kyrie", "Nets", "PG", "scoring");
  std::cout << "This is Kyrie's info: " << std::endl;
  Kyrie.PrintBasicInfo();
  Player KD;
  KD.SetBasicInfo(211, 109, 32, "KD", "Sun", "SF", "scoring and shooting");
  std::cout << "This is KD's info: " << std::endl;
  KD.PrintBasicInfo();
  player_list.push_back(LBJ);
  player_list.push_back(Kyrie);
  player_list.push_back(KD);
  std::cout << std::endl;
  std::cout << std::endl;
  player_list.sort(comp);  // 按照身高排序
  // 按身高打印list中的球员信息
  std::cout << "This is the player list sorted by height: " << std::endl;
  for (auto it : player_list) {
    it.PrintBasicInfo();
  }
  return 0;
}
