/*
 * 利用stl_tree.h实现set的自定义接口
 */

#include <bits/stl_tree.h>

#include <initializer_list>

template <typename _Key, typename _Compare = std::less<_Key>,
          typename _Alloc = std::allocator<_Key>>
class my_set {
 private:
  std::_Rb_tree<_Key, _Key, std::_Identity<_Key>, _Compare, _Alloc> rbtree;
  std::_Rb_tree_iterator<_Key> iter = rbtree.begin();

 public:
  void insert(_Key x) {
    /*插入 利用_M_insert_unique的特性，不用判断是否重复*/
    rbtree._M_insert_unique(x);
  }

  int size() {
    // _M_impl; declared protected in rbtree, so the below command doesn't work
    // return rbtree._M_impl._M_node_count;
    return rbtree.size();
  }

  // 一些自定义的接口
  void print() {
    /*打印set*/
    iter = rbtree.begin();
    while (iter != rbtree.end()) {
      std::cout << *iter << " ";
      iter++;
    }
    std::cout << std::endl;
  }

  bool exist(_Key x) {
    /*返回 x 是否存在*/
    if (rbtree.find(x) != rbtree.end()) {
      return true;
    } else {
      return false;
    }
  }

  bool empty() { return rbtree.empty(); }

  _Key max() { return *rbtree.rbegin(); }

  _Key min() { return *rbtree.begin(); }

  void erase(_Key x) { rbtree.erase(x); }
};
