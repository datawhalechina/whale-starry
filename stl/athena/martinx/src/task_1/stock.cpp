#include "stock.h"

#include <iostream>

#include "httplib.h"
#include "simdjson.h"

// 使用股票代码初始化
Stock::Stock(const std::string& code) { this->code = code; };

// 获取股票最后一个交易日的分钟K线数据
std::string Stock::GetData() {
  // 创建 HTTP 客户端
  httplib::Client cli(STOCK_SERVER_HOST, STOCK_SERVER_PORT);

  // 发送请求
  auto res = cli.Get(GetStockServerEndpoint(this->code));

  // 如果请求成功，打印结果
  if (res && res->status == HTTP_STATUS_OK) {
    return res->body;
  } else {
    std::cout << "error code: " << res->status << std::endl;
    return "{}";
  }
}

// 解析JSON数据，获取K线数据列表
std::list<StockData> Stock::ParseData(std::string& data) {
  auto res_body_padded = simdjson::padded_string(this->HandleData(data));

  // 解析 string 为 json
  simdjson::ondemand::parser parser;
  simdjson::ondemand::document json = parser.iterate(res_body_padded);
  std::list<StockData> values;

  // 获取json中的line数组，并遍历
  for (auto line : json["line"].get_array()) {
    auto item = line.get_array();
    // 获取K线时间
    // int64_t time = item.at(0).get_int64();
    // times.push_back(time);

    // 获取当前价格
    StockData stock_data = StockData();
    stock_data.close = item.at(1).get_double();
    values.push_back(stock_data);

    // 获取成交量
    // std::cout << "volume: " << item.at(2) << std::endl;
    // 获取均价
    // std::cout << "avg_price: " << item.at(3) << std::endl;
    // 获取成交额
    // std::cout << "amount: " << item.at(4) << std::endl;
    // 获取最高价
    // std::cout << "highest: " << item.at(5) << std::endl;
    // 获取最低价
    // std::cout << "lowest: " << item.at(6) << std::endl;
  }

  return values;
}

// 打印结果
void Stock::PrintData(const std::list<StockData>& stocks) {
  for (auto stock : stocks) {
    std::cout << "price: " << stock.close << std::endl;
  }
}

// 处理数据
std::string Stock::HandleData(std::string& data) {
  if (data != "{}") {
    int data_size = data.size();
    std::string res_body = data.erase(0, 1).erase(data_size - 2, data_size - 1);
    return res_body;
  }

  return "{}";
}

std::string GetStockServerEndpoint(const std::string& code) {
  return "/v1/sh1/line/" + code + "?callback=&select=" + STOCK_FIELDS;
}