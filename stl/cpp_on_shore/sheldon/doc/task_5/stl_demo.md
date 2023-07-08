# 使用vector+list+map造一个案例场景

我们需要创建一个订单管理系统，用于处理电子商务平台上的订单流程。系统需要能够追踪订单的状态、管理订单的生命周期以及提供统计数据和报告。

## 实现自定义Order类的排序

需要在类内重载`<`的运算符，这样就可以调用STL中的set和map，实现自定义类作为key的排序。用于提供订单价值和订单时间的报告。

```cpp
/* 排序规则：
   * 按照订单价值降序排列；
   * 如果订单价值相同，则根据订单时间降序排列
   */
  bool operator<(const Order& temp) const {
    if (orderValue == temp.orderValue) {
      return temp.orderTime < this->orderTime;
    } else {
      return temp.orderValue < this->orderValue;
    }
  }
```

## OrderSystem类

OrderSystem类中维护：

1. `setOrder`，用于实现自定义类的排序；
2. `listOrder`，用于实现类似LRU的功能，提供最近访问的报告；
3. `mapOrder`，方便查找

实现的功能：订单的增加、删除、访问。

需要注意的一点是，对于`list`和`vector`，我们要删除指定的元素，复杂度是`O(n)`，当用迭代器迭代时，需要注意删除的逻辑如下：

```cpp
// 删除vector里的order
auto it_vec = vecOrder.begin();
while (it_vec != vecOrder.end()) {
    if (*it_vec == it->second) {
        // 注意不能直接erase(it_vec)，这会导致迭代器失效，it_vec++;不起作用
        auto iter_tmp_vec = it_vec;
        it_vec++;
        
        if (it_vec != vecOrder.end()) {
            // 如果不是最后一个元素，调用erase即可
            vecOrder.erase(iter_tmp_vec);
        } else {
            // 如果是最后一个元素，调用erase不管用，需要调用pop_back()
            vecOrder.pop_back();
            break;
        }
    } else {
        it_vec++;
    }
}
```

