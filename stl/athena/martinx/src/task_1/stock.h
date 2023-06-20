#ifndef STOCK_H
#define STOCK_H

#include <iostream>
#include <list>

#define STOCK_SERVER_HOST "yunhq.sse.com.cn"
#define STOCK_SERVER_PORT 32041
#define STOCK_FIELDS "time,price,volume,avg_price,amount,highest,lowest"
#define HTTP_STATUS_OK 200

struct StockData {
  int64_t time;
  double open;
  double high;
  double low;
  double close;
  double volume;
  double amount;
};

class Stock {
 public:
  Stock(const std::string& code);

  // 获取股票最后一个交易日的分钟K线数据
  std::string GetData();

  // 解析JSON数据，获取K线数据列表
  std::list<StockData> ParseData(std::string& data);

  // 打印结果
  void PrintData(const std::list<StockData>& stocks);

  // 处理数据
  std::string HandleData(std::string& data);

 private:
  // 股票代码
  std::string code;
};

#endif
