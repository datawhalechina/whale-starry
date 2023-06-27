## Task1 的学习笔记

1. #if,#endif,#else等都是条件编译指令；
2. #if __cplusplus >= 201103L 根据这句选择对应版本
3. _GLIBCXX_NOEXCEPT 为宏命令对应 noexcept，即声明该函数不会抛异常，但是一旦抛了编译器就terminate
4. 这种一般都是用template实现