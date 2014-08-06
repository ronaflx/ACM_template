// This implementation has bad performance.
// Because we use the prev_ and next_ in Node.
// We can keep the iteration in O(n) time complexity.
// The algorithm is simple, we can find the prev
// node and next node by using parent. The worst of
// time complexity is O(log(n)), while the amortized
// complexity is O(1).
#include <functional>
#include <cassert>
#include <cstdio>
#include <iterator>
#include <algorithm>

template <typename KeyType, typename ValType,
          typename Compare = std::less<KeyType>,
          typename Alloc = std::allocator<std::pair<const KeyType, ValType>>>
class AvlTree {
 public:
  // TODO(ronaflx): re-implement to be compatible with reverse_iterator.
  class Iterator;
  class ConstIterator;
  // Member types of this AvlTree.
  typedef KeyType key_type;
  typedef ValType mapped_type;
  typedef std::pair<const KeyType, ValType> value_type;
  typedef Alloc allocator_type;
  typedef std::size_t size_type;
  typedef std::ptrdiff_t difference_type;
  typedef Compare key_comp;
  typedef value_type& reference;
  typedef const value_type& const_reference;
  // typedef pointer;
  // typedef const_pointer;
  typedef Iterator iterator;
  typedef ConstIterator const_iterator;
  typedef std::reverse_iterator<iterator> reverse_iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

  // Constructor and destructor.
  AvlTree()
      : comp_(Compare()), head_(nullptr), tail_(nullptr), root_(nullptr) {}
  ~AvlTree() { clear(); }

  // Modifiers.
  void clear() noexcept;
  // Modifiers: Insert operation.
  std::pair<iterator, bool> insert(const value_type& value);
  // Modifiers: Erase operation.
  size_type erase(const key_type& key);
  // Modifiers: Swap operation.
  void swap(AvlTree& other);

  // Iterators.
  iterator begin() { return Iterator(head_); }
  iterator end() { return Iterator(nullptr); }
  const_iterator cbegin() { return ConstIterator(head_); }
  const_iterator cend() { return ConstIterator(nullptr); }
  // TODO(ronaflx): Implement these.
  reverse_iterator rbegin() {}
  reverse_iterator rend() {}
  const_reverse_iterator crbegin() {}
  const_reverse_iterator crend() {}

 private:
  struct Node {
    AvlTree::value_type value_;
    Node* left_, *right_;
    Node* prev_, *next_;
    Node* parent_;
    int height_;

    Node(const AvlTree::value_type& value)
        : value_(value),
          left_(nullptr),
          right_(nullptr),
          prev_(nullptr),
          next_(nullptr),
          parent_(nullptr),
          height_(1) {}
    Node()
        : left_(nullptr),
          right_(nullptr),
          prev_(nullptr),
          next_(nullptr),
          parent_(nullptr),
          height_(0) {}
  };
  typedef typename Alloc::template rebind<Node>::other NodeAlloctorType;
  Compare comp_;
  Node* head_, *tail_, *root_;
  NodeAlloctorType alloc_;
  // Node related operator.
  // I use 2 functions to operate the field height in case errors.
  inline int GetHeight(const Node* node) const {
    return node == nullptr ? 0 : node->height_;
  }
  inline void UpdateHeight(Node* node) {
    node->height_ =
        std::max(GetHeight(node->left_), GetHeight(node->right_)) + 1;
  }

  // Only Called in modifiers.
  // Balance operator of balanced binary search tree.
  inline void Balance(Node**) noexcept;
  inline void RightRotate(Node**) noexcept;
  inline void LeftRotate(Node**) noexcept;

  // Link the node as a doubly linked list.
  inline void LinkPrev(Node* prev, Node* to_insert);
  inline void LinkNext(Node* prev, Node* to_insert);

 protected:
  class value_compare
      : public std::binary_function<value_type, value_type, bool> {
   private:
    friend class AvlTree<KeyType, ValType, Compare, Alloc>;

   protected:
    value_compare(Compare c) : c_(c) {}
    Compare c_;

   public:
    bool operator()(const value_type& lhs, const value_type& rhs) const {
      return c_(lhs, rhs);
    }
  };
};

template <typename KeyType, typename ValType, typename Compare, typename Alloc>
class AvlTree<KeyType, ValType, Compare, Alloc>::Iterator
    : public std::iterator<std::bidirectional_iterator_tag,
                           std::pair<const KeyType, ValType>> {
 public:
  typedef AvlTree::Node Node;
  Iterator() {}
  Iterator(Node* node) : cur_node_(node) {}
  Iterator(const Iterator& iter) : cur_node_(iter.cur_node_) {}

  Iterator& operator++() {
    cur_node_ = cur_node_->next_;
    return *this;
  }

  Iterator operator++(int) {
    Iterator ret = *this;
    ++(*this);
    return ret;
  }

  Iterator& operator--() {
    cur_node_ = cur_node_->prev_;
    return this;
  }

  Iterator operator--(int) {
    Iterator ret = *this;
    --(*this);
    return ret;
  }

  bool operator==(const Iterator& i) { return cur_node_ == i.cur_node_; }
  bool operator!=(const Iterator& i) { return cur_node_ != i.cur_node_; }
  reference& operator*() { return cur_node_->value_; }

 private:
  Node* cur_node_;
};
template <typename KeyType, typename ValType, typename Compare, typename Alloc>
class AvlTree<KeyType, ValType, Compare, Alloc>::ConstIterator
    : public std::iterator<std::bidirectional_iterator_tag,
                           const std::pair<const KeyType, ValType>> {
 public:
  typedef AvlTree::Node Node;
  ConstIterator() {}
  ConstIterator(Node* node) : cur_node_(node) {}
  ConstIterator(const Iterator& iter) : cur_node_(iter.cur_node_) {}
  ConstIterator(const ConstIterator& iter) : cur_node_(iter.cur_node_) {}

  ConstIterator& operator++() {
    cur_node_ = cur_node_->next_;
    return this;
  }

  ConstIterator operator++(int) {
    ConstIterator ret = *this;
    ++(*this);
    return ret;
  }

  ConstIterator& operator--() {
    cur_node_ = cur_node_->prev_;
    return this;
  }

  ConstIterator operator--(int) {
    ConstIterator ret = *this;
    --(*this);
    return ret;
  }
  reference& operator*() { return cur_node_->value_; }

 private:
  const Node* cur_node_;
};

template <typename KeyType, typename ValType, typename Compare, typename Alloc>
inline void AvlTree<KeyType, ValType, Compare, Alloc>::LeftRotate(
    Node** node) noexcept {
  Node* rc = (*node)->right_;
  (*node)->right_ = rc->left_;
  if (rc->left_ != nullptr) {
    rc->left_->parent_ = *node;
  }
  UpdateHeight(*node);
  rc->left_ = *node;

  rc->parent_ = (*node)->parent_;
  (*node)->parent_ = rc;
  *node = rc;
  UpdateHeight(*node);
}

template <typename KeyType, typename ValType, typename Compare, typename Alloc>
inline void AvlTree<KeyType, ValType, Compare, Alloc>::RightRotate(
    Node** node) noexcept {
  Node* lc = (*node)->left_;
  (*node)->left_ = lc->right_;
  if (lc->right_ != nullptr) {
    lc->right_->parent_ = *node;
  }
  UpdateHeight(*node);
  lc->right_ = *node;

  lc->parent_ = (*node)->parent_;
  (*node)->parent_ = lc;
  *node = lc;
  UpdateHeight(*node);
}

template <typename KeyType, typename ValType, typename Compare, typename Alloc>
inline void AvlTree<KeyType, ValType, Compare, Alloc>::Balance(
    Node** node) noexcept {
  if ((*node)->left_ != nullptr) {
    if (GetHeight((*node)->left_->left_) == GetHeight((*node)->right_) + 1) {
      RightRotate(node);
    } else if (GetHeight((*node)->left_->right_) ==
               GetHeight((*node)->right_) + 1) {
      LeftRotate(&(*node)->left_);
      RightRotate(node);
    }
  }
  if ((*node)->right_ != nullptr) {
    if (GetHeight((*node)->right_->right_) == GetHeight((*node)->left_) + 1) {
      LeftRotate(node);
    } else if (GetHeight((*node)->right_->left_) ==
               GetHeight((*node)->left_) + 1) {
      RightRotate(&(*node)->right_);
      LeftRotate(node);
    }
  }
}

template <typename KeyType, typename ValType, typename Compare, typename Alloc>
void AvlTree<KeyType, ValType, Compare, Alloc>::clear() noexcept {
  for (Node* i = head_, *j; i != nullptr;) {
    j = i;
    i = i->next_;
    delete j;
  }
  root_ = head_ = tail_ = nullptr;
}

template <typename KeyType, typename ValType, typename Compare, typename Alloc>
inline void AvlTree<KeyType, ValType, Compare, Alloc>::LinkPrev(
    Node* prev, Node* to_insert) {
  to_insert->prev_ = prev->prev_;
  if (prev->prev_ != nullptr) {
    prev->prev_->next_ = to_insert;
  }
  prev->prev_ = to_insert;
  to_insert->next_ = prev;
}

template <typename KeyType, typename ValType, typename Compare, typename Alloc>
inline void AvlTree<KeyType, ValType, Compare, Alloc>::LinkNext(
    Node* prev, Node* to_insert) {
  to_insert->next_ = prev->next_;
  if (prev->next_ != nullptr) {
    prev->next_->prev_ = to_insert;
  }
  prev->next_ = to_insert;
  to_insert->prev_ = prev;
}

template <typename KeyType, typename ValType, typename Compare, typename Alloc>
std::pair<typename AvlTree<KeyType, ValType, Compare, Alloc>::iterator, bool>
AvlTree<KeyType, ValType, Compare, Alloc>::insert(const value_type& value) {
  Node* curr = root_, *parent = nullptr;
  while (curr != nullptr) {
    parent = curr;
    if (comp_(curr->value_.first, value.first)) {
      curr = curr->right_;
    } else if (comp_(value.first, curr->value_.first)) {
      curr = curr->left_;
    } else {
      return std::make_pair(Iterator(curr), false);
    }
  }
  Node* to_insert = alloc_.allocate(1);
  alloc_.construct(to_insert, value);
  if (root_ == nullptr) {
    root_ = head_ = tail_ = to_insert;
  } else {
    // Setup pointer between to_insert with parent.
    to_insert->parent_ = parent;
    if (comp_(value.first, parent->value_.first)) {
      parent->left_ = to_insert;
      LinkPrev(parent, to_insert);
      // Update head of binary search tree.
      if (parent == head_) {
        head_ = to_insert;
      }
    } else {
      parent->right_ = to_insert;
      LinkNext(parent, to_insert);
      // Update tail of binary search tree.
      if (parent == tail_) {
        tail_ = to_insert;
      }
    }
    // Balance all the node in path from to_insert to root.
    while (parent != root_) {
      UpdateHeight(parent);
      if (parent == parent->parent_->left_) {
        Balance(&parent->parent_->left_);
      } else {
        Balance(&parent->parent_->right_);
      }
      parent = parent->parent_;
    }
    UpdateHeight(root_);
    Balance(&root_);
  }
  return std::make_pair(Iterator(to_insert), true);
}
template <class T>
struct node {
  T value;
  int h, size;
  node<T>* lchild, *rchild;
  node() {}
  node(T value, int h, int size, node<T>* lchild, node<T>* rchild)
      : value(value), h(h), size(size), lchild(lchild), rchild(rchild) {}
};

#include <vector>
#include <cstdlib>
#include <map>
using namespace std;
int main() {
  AvlTree<int, int> weight_map;
  map<int, int> weight_map_benchmark;
  const int N = 500000;
  vector<int> random_keys(N), random_vals(N);
  srand(time(NULL));
  for (int i = 0; i < N; i++) {
    random_keys[i] = rand();
    random_vals[i] = rand();
  }

  time_t start = clock();
  printf("Start My Insert\n");
  for (int i = 0; i < N; i++) {
    weight_map.insert(make_pair(random_keys[i], random_vals[i]));
  }
  printf("My Insert OK\n");
  time_t end = clock();
  printf("%f\n", double(end - start) / CLOCKS_PER_SEC);

  start = clock();
  printf("Start Map Insert\n");
  for (int i = 0; i < N; i++) {
    weight_map_benchmark.insert(make_pair(random_keys[i], random_vals[i]));
  }
  printf("Map Insert OK\n");
  end = clock();
  printf("%f\n", double(end - start) / CLOCKS_PER_SEC);

  AvlTree<int, int>::iterator avl_iter = weight_map.begin();
  map<int, int>::iterator map_iter = weight_map_benchmark.begin();
  while (avl_iter != weight_map.end() &&
         map_iter != weight_map_benchmark.end()) {
    assert(*avl_iter == *map_iter);
    avl_iter++;
    map_iter++;
  }
  printf("Order OK\n");
  assert(avl_iter == weight_map.end() &&
         map_iter == weight_map_benchmark.end());
  printf("Size OK\n");

  start = clock();
  for (const auto& iter : weight_map) {
    assert(iter.first != INT_MIN);
  }
  end = clock();
  printf("My AVL loop %f\n", double(end - start) / CLOCKS_PER_SEC);

  start = clock();
  for (const auto& iter : weight_map_benchmark) {
    assert(iter.first != INT_MIN);
  }
  end = clock();
  printf("Map loop %f\n", double(end - start) / CLOCKS_PER_SEC);
  return 0;
}

