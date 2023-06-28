/**
 * 以自己理解写的红黑树代码有不好解决的死循环问题
 * 以下代码参考 https://blog.csdn.net/AI_ELF/article/details/123092343 进行学习和复写。
 */

#pragma once

enum Color { RED, BLACK };

template <class K, class V>
struct RBTreeNode {
  RBTreeNode(const K& k, const V& v)
      : left(nullptr), right(nullptr), parent(nullptr), key(k), value(v), col(RED) {}

  RBTreeNode<K, V>* left;
  RBTreeNode<K, V>* right;
  RBTreeNode<K, V>* parent;
  K key;
  V value;
  Color col;
};

template <class K, class V>
class RBTree {
  typedef RBTreeNode<K, V> Node;

 public:
  ~RBTree() { Destroy(root_); };

  bool Insert(const K& k, const V& v) {
    if (root_ == nullptr) {
      root_ = new Node(k, v);
      root_->col = BLACK;
      return true;
    }

    Node* cur = root_;
    Node* parent = cur;
    while (cur) {
      if (cur->key > k) {
        parent = cur;
        cur = cur->left;
      } else if (cur->key < k) {
        parent = cur;
        cur = cur->right;
      } else {
        return false;
      }
    }

    cur = new Node(k, v);
    if (parent->key > k) {
      parent->left = cur;
      cur->parent = parent;
    } else {
      parent->right = cur;
      cur->parent = parent;
    }

    while (parent && parent->col == RED) {
      Node* grandParent = parent->parent;
      if (grandParent->left == parent) {
        Node* uncle = grandParent->right;
        if (uncle && uncle->col == RED) {
          parent->col = uncle->col = BLACK;
          grandParent->col = RED;
          cur = grandParent;
          parent = cur->parent;
        } else {
          if (cur == parent->left) {
            Dextrorotation(grandParent);
            grandParent->col = RED;
            parent->col = BLACK;
          } else {
            Levorotation(parent);
            Dextrorotation(grandParent);
            cur->col = BLACK;
            grandParent->col = RED;
          }
          break;
        }
      } else {
        Node* uncle = grandParent->left;
        if (uncle && uncle->col == RED) {
          parent->col = uncle->col = BLACK;
          grandParent->col = RED;
          cur = grandParent;
          parent = cur->parent;
        } else {
          if (cur == parent->right) {
            Levorotation(grandParent);
            grandParent->col = RED;
            parent->col = BLACK;
          } else {
            Dextrorotation(parent);
            Levorotation(grandParent);
            cur->col = BLACK;
            grandParent->col = RED;
          }
          break;
        }
      }
    }
    root_->col = BLACK;
    return true;
  }

  void Dextrorotation(Node* parent) {
    if (parent == nullptr) return;
    Node* childL = parent->left;
    Node* childLR = childL->right;
    parent->left = childLR;
    if (childLR) childLR->parent = parent;

    Node* cur = parent->parent;
    childL->right = parent;
    if (parent == root_) {
      root_ = childL;
      childL->parent = nullptr;
      parent->parent = childL;
    } else {
      if (cur->left == parent) {
        cur->left = childL;
        childL->parent = cur;
        parent->parent = childL;
      } else {
        cur->right = childL;
        childL->parent = cur;
        parent->parent = childL;
      }
    }
  }

  void Levorotation(Node* parent) {
    if (parent == nullptr) return;
    Node* childR = parent->right;
    Node* childRL = childR->left;
    parent->right = childRL;
    if (childRL) childRL->parent = parent;

    Node* cur = parent->parent;
    childR->left = parent;
    if (parent == root_) {
      root_ = childR;
      childR->parent = nullptr;
      parent->parent = childR;
    } else {
      if (cur->left == parent) {
        cur->left = childR;
        childR->parent = cur;
        parent->parent = childR;
      } else {
        cur->right = childR;
        childR->parent = cur;
        parent->parent = childR;
      }
    }
  }

  Node* Find(const K& key) {
    Node* cur = root_;
    while (cur) {
      if (cur->key > key) {
        cur = cur->left;
      } else if (cur->key < key) {
        cur = cur->right;
      } else {
        return cur;
      }
    }
    return nullptr;
  }

 private:
  void Destroy(Node* root) {
    if (root == nullptr) {
      return;
    }
    Destroy(root->left);
    Destroy(root->right);
    delete root;
  }

  Node* root_ = nullptr;
};
