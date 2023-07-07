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

