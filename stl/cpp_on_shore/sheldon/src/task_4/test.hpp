/*用来测试C++11特性：完美转发的代码*/
#include <iostream>

template <typename T>
void print(T& t) {
  std::cout << "lvalue" << std::endl;
}
template <typename T>
void print(T&& t) {
  std::cout << "rvalue" << std::endl;
}

template <typename T>
void test_forward(T&& v) {
  print(v);
  print(std::forward<T>(v));
  print(std::move(v));
}

int test() {
  test_forward(1);
  /*
   print(v);
   //lvalue. 1 是右值，传入TestForward后 v是右值引用，v是左值
   print(std::forward<T>(v));
   //rvalue. 1 是右值，传入TestForward后v是右值引用，forward转发后是右值引用
   print(std::move(v));
   //rvalue. 1 是右值，传入TestForward后v是右值引用，std::move后返回右值引用
  */
  int x = 1;
  test_forward(x);
  /*
   print(v);
   //lvalue v是左值，传入TestForward后 v是左值引用，v是左值
   print(std::forward<T>(v));
   //lvalue v被forward转发后依然是左值引用
   print(std::move(v));
   //rvalue v被move后转化为右值引用
  */
  test_forward(std::forward<int>(x));
  /*
   print(v);
   //lvalue std::forward<int>(x)是右值，v是右值引用
   print(std::forward<T>(v));
   //rvalue v被forward转发后依然是右值引用
   print(std::move(v));
   //rvalue v被move后依然是右值引用
  */
  return 0;
}