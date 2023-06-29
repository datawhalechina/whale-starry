#pragma onec

template <class K, class V>
struct BSTreeNode {
  BSTreeNode(const K& k, const V& v) : left(nullptr), right(nullptr), key(k), value(v) {}
  BSTreeNode<K, V>* left;
  BSTreeNode<K, V>* right;
  K key;
  V value;
};

template <class K, class V>
class BSTree {
  using Node = BSTreeNode<K, V>;

 public:
  BSTree() : root_(nullptr) {}
  BSTree(const BSTree<K, V>& t) { root_ = Copy(t.root_); }
  BSTree<K, V>& operator=(BSTree<K, V> t) {
    std::swap(root_, t.root_);
    return *this;
  }

  ~BSTree() { Destroy(root_); }

  bool Insert(const K& key, const V& value) {
    if (root_ == nullptr) {
      root_ = new Node(key, value);
      return true;
    }
    Node* parent = nullptr;
    Node* cur = root_;

    while (cur) {
      if (cur->key > key) {
        parent = cur;
        cur = cur->left;
      } else if (cur->key < key) {
        parent = cur;
        cur = cur->right;
      } else {
        return false;
      }
      cur = new Node(key, value);
      if (parent->key > key) {
        parent->left = cur;
      } else {
        parent->right = cur;
      }
    }
    return true;
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
  Node* Copy(Node* root) {
    if (root == nullptr) {
      return nullptr;
    }
    Node* newNode = new Node(root->key, root->value);
    newNode->left = Copy(root->left);
    newNode->right = Copy(root->right);
    return newNode;
  }

  void Destroy(Node* root) {
    if (root == nullptr) {
      return;
    }
    Destroy(root->left);
    Destroy(root->right);
    delete root;
  }

  Node* root_{nullptr};
};
