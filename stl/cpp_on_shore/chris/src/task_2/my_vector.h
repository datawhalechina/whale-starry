#include <cstring>
#include <iostream>

template <typename T>
class MyVector {
 public:
  // 无参构造的初始化
  MyVector() : p(NULL), capacity(0), size(0) {}
  // 有参构造的初始化方式
  MyVector(int size, T data) {
    this->capacity = 20 + size;
    this->size = size;
    this->p = new T[capacity];
    for (int i = 0; i < this->size; i++) {
      this->p[i] = data;
    }
  }
  // 析构函数，释放掉唯一的指针
  ~MyVector() {
    if (p != NULL) {
      delete[] p;
    }
  }
  // 拷贝构造函数
  MyVector(const MyVector& v) {
    this->capacity = v.capacity;
    this->size = v.size;
    this->p = new T[this->capacity];
    memcpy(this->p, v.p, this->size * sizeof(T));
  }

  // 打印容器内容
  void print() {
    for (int i = 0; i < this->size; i++) {
      std::cout << this->p[i] << '\t';
    }
    std::cout << std::endl;
  }
  // 插入，要判断溢出没
  void push_back(T data) {
    if (this->p == NULL) {
      this->capacity = 20;
      this->size = 0;
      T* p = new T[capacity];
    }
    if (this->size == this->capacity) {
      // 如果满了，每次容量拓展到2倍
      T* new_p = new T[this->capacity * 2];
      memcpy(new_p, p, this->size * sizeof(T));
      this->capacity *= 2;
      delete[] p;
      p = new_p;
    }
    this->print();
    this->p[this->size] = data;
    this->size++;
  }
  // 删除最后一个元素，无返回值
  void pop_back() {
    if (this->size > 1) {
      this->p[this->size - 1] = 0;
      this->size--;
    }
  }
  // 插入
  void insert(int pos, T data) {
    if (pos >= 0 && pos <= this->size) {
      if (this->size == this->capacity) {
        // 如果满了，每次容量拓展到2倍
        T* new_p = new T[this->capacity * 2];
        memcpy(new_p, p, this->size * sizeof(T));
        this->capacity *= 2;
        delete[] p;
        p = new_p;
      }
      for (int i = this->size; i > pos; i--) {
        this->p[i] = this->p[i - 1];
      }
      this->p[pos] = data;
      this->size++;
    }
  }

  void clear() { this->size = 0; }

  // 重载[]运算符，可以用[]修改函数
  T& operator[](int index) {
    if (index > 0 && index < this->size) {
      return this->p[index];
    }
  }
  // 重载赋值=
  void operator=(const MyVector& v) {
    if (this->p != NULL) {
      delete[] this->p;
      this->capacity = 0;
      this->size = 0;
      this->p = NULL;
    }
    this->capacity = v.capacity;
    this->size = v.size;
    this->p = new T[this->capacity];
    memcpy(this->p, v.p, this->size * sizeof(T));
  }

  // 来看看内部的capacity和size的不同之处
 public:
  T* p;
  int capacity;
  int size;
};

void test() {
  MyVector<int> v(3, 1);
  v.print();
  std::cout << "size: " << v.size << " capacity: " << v.capacity << std::endl;
  for (int iter = 0; iter < 11; iter++) {
    v.push_back(iter);
  }
  v.print();
  v.pop_back();
  v.pop_back();
  v.pop_back();
  v.print();
  v[3] = 140623;
  v.print();

  std::cout << "what" << std::endl;
  MyVector<int> v2(v);
  v2.print();

  MyVector<int> v3 = v;
  v3.print();

  MyVector<int> v4;
  v4 = v;
  v4.print();

  std::cout << "size: " << v4.size << " capacity: " << v4.capacity << std::endl;
  v4.insert(1, 321);
  v4.insert(0, 897);
  v4.insert(v4.size - 1, 456);
  v4.insert(v4.size, 876);
  // v4.insert(2,11111);
  v4.print();
  v4.clear();
  v4.print();
}
