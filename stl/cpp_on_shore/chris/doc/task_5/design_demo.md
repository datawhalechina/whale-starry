# This markdown is to design an order management system.
1. 在本程序中, 我主要使用了两个文件: 一个是头文件, 主要存放**订单**类和**订单系统**类; 第二个是主函数接口, 用来调用订单系统类来调试并显示输出
2. 在订单类中, 我们设置了**序号**, **订单状态**, **客户名称**, **价格**四个成员变量; 其中, 订单状况主要分为以下几种状态: **推迟付款**(***Pending***), **已发货**(***Shipped***), **已付款**(***Paid***), **已收货**(***Received***), **拒收**(***Rejected***)五种状态
3. 对于五种收获状态, 对应着**3种**收益情况: 其中, 推迟付款(Pending)认为收益(price)为**0**; 拒收(Rejected)则认为收益(price)为**负数**收益; 其余三种状态都认为是**正数**收益
4. 设计到的类使用了两个, 一个为订单类, 即设计了每个订单的状态和必要信息; 另外一个为订单系统类, 用于使用序列化方式或者关联化方式存储订单信息, 组成订单系统. 整体的设计模式类似于工厂模式, 不过产品和工厂的依附关系换了一下.(不知道理解的对不对)
5. 发现了一些问题: 类中的默认拷贝函数对使用的对象是浅拷贝, 为了避免订单系统的异常拷贝状态, 我将默认的拷贝函数禁用(=delete实现)
6. 有一个设计思路上的问题, 目前存疑: 如何在析构函数中释放构造函数中开辟的内存空间? 目前我是将所有的delete操作都放在了构造函数中, 但这种设计会使构造函数过于臃肿, 同时也让析构函数失去了他应有的作用, 目前还没有想到很好的替代方法.