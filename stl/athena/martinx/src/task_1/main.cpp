#include <iostream>
#include <list>

#include "httplib.h"
#include "simdjson.h"
#include "stock.h"

/**
 * @brief 从上海证券交易所获取股票分钟行情
 * @author Martin <martin.xus@gmail.com>
 *
 * @return int
 */
int main(int argc, char** argv) {
  if (argc == 1) {
    std::cout << "Usage: " << argv[0] << " [code]" << std::endl;
  } else {
    for (int i = 1; i < argc; ++i) {
      std::string code = argv[i];
      if (code.length() != 6) {
        std::cout << "Invalid code: " << code << std::endl;
      } else {
        Stock stock(code);
        std::string data = stock.GetData();
        std::list<StockData> stocks = stock.ParseData(data);
        stock.PrintData(stocks);
      }
    }
  }

  return 0;
}
