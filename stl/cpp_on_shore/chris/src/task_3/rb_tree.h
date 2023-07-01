#include <cassert>
#include <iostream>

typedef enum { RED = 0, BLACK } Color;

// 节点类型
template <typename Type>
struct RbTreeNode {
  Color color;         // 颜色
  Type key;            // 关键字
  RbTreeNode* left;    // 左子节点
  RbTreeNode* right;   // 右子节点
  RbTreeNode* parent;  // 父节点
};

template <typename Type>
class RbTree {
 public:
  // constructor
  RbTree() {
    nil = ApplyNode();
    root = nil;
    nil->color = BLACK;
  }
  // deconstructor
  ~RbTree() {
    Destroy(root);  // 销毁创建的非nil节点
    delete nil;
    nil = nullptr;
  }

  // 中序遍历
  void InOrder() { InOrder(root); }

  // insert func
  // 1. BST方式插入
  // 2. 调整平衡
  bool Insert(const Type& value) {
    RbTreeNode<Type>* pr = nil;  // 用pr来记录父节点
    RbTreeNode<Type>* s = root;  // 定义s指向根节点
    while (s != nil) {
      if (value == s->key) return false;
      pr = s;  // 每次记住s的父节点
      if (value < s->key)
        s = s->left;  // 小的节点要一直往左找位置
      else
        s = s->right;  // 大的节点则要往右子树找
    }
    // 循环之后 s==nil
    s = ApplyNode(value);  // 申请节点
    if (pr == nil) {
      // 如果父节点pr是根节点, 第一次root指向nil，所以默认pr==nil
      root = s;
      root->parent = pr;
    } else {  // 如果父节点不是根节点
      if (value < pr->key)
        pr->left = s;
      else
        pr->right = s;
    }
    s->parent = pr;  // 设置新节点s的父节点

    // 插入节点之后就需要按照红黑树规则调整节点
    RbTreeRebalance(s);
    return true;
  }

  void Remove(Type key) {
    RbTreeNode<Type>* t;
    if ((t = Search(root, key)) != nil) {
      Remove(t);
    } else
      std::cout << "The key does not exist." << std::endl;
  }

  void InOrderPrint() { InOrderPrint(root); }

 protected:
  // 申请节点，将节点的颜色的初始化为红色，初始化节点的颜色为红色
  // 初始化节点的关键字，其他初始化为空
  RbTreeNode<Type>* ApplyNode(const Type& x = Type()) {
    RbTreeNode<Type>* s = new RbTreeNode<Type>();
    assert(s != nullptr);
    s->color = RED;
    s->left = s->right = s->parent = nil;
    s->key = x;
    return s;
  }

  // 中序遍历
  void InOrder(RbTreeNode<Type>* root) {
    if (root != nil) {
      InOrder(root->left);
      std::cout << root->key << " ";
      InOrder(root->right);
    }
  }

  // 向左翻转
  void LeftRotate(RbTreeNode<Type>* z) {
    RbTreeNode<Type>* y = z->right;
    z->right = y->left;
    // y所指左节点不为空时
    if (y->left == nil) y->left->parent = z;
    y->parent = z->parent;
    // z为根节点时
    if (root == z)
      root = y;
    else if (z == z->parent->left) {  // z在左节点
      z->parent->left = y;
    } else {  // z在右节点
      z->parent->right = y;
    }
    y->left = z;
    z->parent = y;
  }

  // 向右翻转
  void RightRotate(RbTreeNode<Type>* z) {
    RbTreeNode<Type>* y = z->left;
    z->left = y->right;
    // y所指左节点不为空时
    if (y->right == nil) y->right->parent = z;
    y->parent = z->parent;
    // z为根节点时
    if (root == z)
      root = y;
    else if (z == z->parent->left) {  // z在左节点
      z->parent->left = y;
    } else {  // z在右节点
      z->parent->right = y;
    }
    y->right = z;
    z->parent = y;
  }

  // ReBalance函数
  void RbTreeRebalance(RbTreeNode<Type>* s) {
    RbTreeNode<Type>* uncle;           // 伯父节点
    while (s->parent->color == RED) {  // 父节点颜色也为红色时

      if (s->parent == s->parent->parent->left) {  // 父节点是左节点时
        uncle = s->parent->parent->right;

        if (uncle->color == RED) {  // 伯父节点若也为红色
          // 将父节点和伯父节点都转为黑色
          s->parent->color = BLACK;
          uncle->color = BLACK;
          // 然后将祖父节点转为红色
          s->parent->parent->color = RED;
          // 将s指针指向祖父节点, 下一次循环继续判断祖父的父节点是否为红色
          s = s->parent->parent;
        }

        else {  // 若没有伯父节点, 或者伯父节点为黑色(过程中有可能)
          if (s == s->parent->right) {  // 若调整节点是右节点
            s = s->parent;              // 先将s指向其父节点
            LeftRotate(s);              // 再左翻转
          }

          // 如果调整的节点再左节点，则将s的父节点转变为黑色,
          // 将祖父的节点变为红色 将s的祖父节点右转
          s->parent->color = BLACK;
          s->parent->parent->color = RED;
          RightRotate(s->parent->parent);
        }
      } else {
        if (s->parent == s->parent->parent->right) {  // 父节点是右节点
          uncle = s->parent->parent->left;
          if (uncle->color == RED) {  // 伯父节点为红色
            // 父节点和伯父节点都变为黑色
            s->parent->color = BLACK;
            uncle->color = BLACK;
            // 祖父节点变为红色
            s->parent->parent->color = RED;
            // 将s指针指向祖父节点,
            // 下一次循环继续判断祖父的父节点是否为红色
            s = s->parent->parent;
          } else {  // 没有伯父节点, 或者伯父节点为黑色
            if (s == s->parent->left) {  // 如果调整节点在左节点
              s = s->parent;
              RightRotate(s);  // 右转
            }
            // 若调整的节点在右节点, 将s的父节点变为黑色, 将祖父的节点变为红色
            // 将祖父节点再左转
            s->parent->color = BLACK;
            s->parent->parent->color = RED;
            LeftRotate(s->parent->parent);
          }
        }
      }
    }
    root->color = BLACK;
  }

  // 查找节点s
  RbTreeNode<Type>* Search(RbTreeNode<Type>* root, Type key) const {
    // root 为空, 或key与根的key相同
    if (root == nil) return nil;
    if (root->key == key) return root;
    if (key < root->key)
      return Search(root->left, key);
    else
      return Search(root->right, key);
  }

  // 将u的子节点指向u的指针改变为指向v, 将v的父节点指针改变为指向u的父节点
  void Transfer(RbTreeNode<Type>* u, RbTreeNode<Type>* v) {
    if (u->parent == nil) root = v;  // 若u的父节点为空，令根为v
    // u父节点不为空, 且u在左子树
    else if (u == u->parent->left)
      u->parent->left = v;
    else
      u->parent->right = v;  // u在右子树
    v->parent = u->parent;
  }

  // 找最小值
  RbTreeNode<Type>* Minimum(RbTreeNode<Type>* x) {
    if (x->left == nil) return x;
    return Minimum(x->left);
  }

  void Remove(RbTreeNode<Type>* z) {
    RbTreeNode<Type>* x = nil;
    RbTreeNode<Type>* y = z;  // y记住传进来的z结点
    Color ycolor = y->color;  //
    if (z->left == nil)       // z只有右孩子
    {
      x = z->right;
      Transfer(z, z->right);
    } else if (z->right == nil)  // z只有右孩子
    {
      x = z->left;
      Transfer(z, z->left);
    } else  // 右左孩子和右孩子
    {
      y = Minimum(z->right);  // y是z右子树的的最左子树
      ycolor = y->color;
      x = y->right;
      if (y->parent == z)  // z的右子结点没有左节点或为nil
      {
        x->parent = y;
      } else  // z的右子结点有左节点或为nil
      {
        Transfer(y, y->right);
        y->right = z->right;
        y->right->parent = y;
      }
      Transfer(z, y);
      // 改变指向
      y->left = z->left;
      z->left->parent = y;
      y->color = z->color;
    }
    if (ycolor == BLACK) {
      RemoveFixup(x);
    }
  }
  // 红黑树删除调整
  void RemoveFixup(RbTreeNode<Type>* x) {
    while (x != root && x->color == BLACK)  // 当结点x不为根并且它的颜色不是黑色
    {
      if (x == x->parent->left)  // x在左子树
      {
        RbTreeNode<Type>* w = x->parent->right;  // w是x的兄结点

        if (w->color == RED)  // 情况1
        {
          w->color = BLACK;
          x->parent->color = RED;
          LeftRotate(x->parent);
          w = x->parent->right;
        }
        if (w->left->color == BLACK && w->right->color == BLACK)  // 情况2
        {
          w->color = RED;
          x = x->parent;
        } else {
          if (w->right->color == BLACK)  // 情况3
          {
            w->color = RED;
            w->left->color = BLACK;
            RightRotate(w);
            w = x->parent->right;
          }
          // 情况4
          w->color = w->parent->color;
          w->parent->color = BLACK;
          w->right->color = BLACK;
          LeftRotate(x->parent);
          x = root;  // 结束循环
        }
      } else  // x在右子树
      {
        RbTreeNode<Type>* w = x->parent->left;
        if (w->color == RED)  // 情况1
        {
          w->parent->color = RED;
          w->color = BLACK;
          RightRotate(x->parent);
          w = x->parent->left;
        }
        if (w->right->color == BLACK && w->right->color == BLACK)  // 情况2
        {
          w->color = RED;
          x = x->parent;
        } else {
          if (w->left->color == BLACK)  // 情况3
          {
            w->right->color = BLACK;
            w->color = RED;
            LeftRotate(w);
            w = x->parent->left;
          }
          // 情况4
          w->color = x->parent->color;
          x->parent->color = BLACK;
          w->left->color = BLACK;
          RightRotate(x->parent);
          x = root;  // 结束循环
        }
      }
    }
    x->color = BLACK;
  }
  void Destroy(RbTreeNode<Type>*& root) {
    if (root == nil) return;
    if (root->left != nil) Destroy(root->left);
    if (root->right != nil) Destroy(root->right);
    delete root;
    root = nullptr;
  }
  // 中序遍历打印结点详细的结点颜色
  void InOrderPrint(RbTreeNode<Type>* node) {
    if (node == nil) {
      return;
    }
    if (node->left != NULL) {
      InOrderPrint(node->left);
    }
    std::cout << node->key << "(" << ((node->color == BLACK) ? "BLACK" : "RED") << ")"
              << " ";
    if (node->right != nil) {
      InOrderPrint(node->right);
    }
  }

 private:
  RbTreeNode<Type>* root;  // 根指针
  RbTreeNode<Type>* nil;   // 外部节点，表示空节点，颜色为黑色
};