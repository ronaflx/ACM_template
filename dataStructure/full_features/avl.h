
// This implementation has bad insert performance.
// Because the cost of AVL re-balance.

// We can keep the iteration in O(n) time complexity
// without prev_ and next_. The algorithm is simple,
// find the prev node and next node by using parent.
// The worst of time complexity is O(log(n)), while
// the amortized complexity is O(1).
// TODO(ronaflx): operator[](C++11), at(C++11),insert(C++11)
// emplace(c++11), emplace_hint(c++11) and move constructor
#include <functional>
#include <memory>
#include <iterator>
#include <algorithm>

template <typename KeyType, typename ValType,
          typename Compare = std::less<KeyType>,
          typename Alloc = std::allocator<std::pair<const KeyType, ValType>>>
class Avl {
 public:
  class Iterator;
  class ConstIterator;

  // **** Types ****
  // Member types.
  typedef KeyType key_type;
  typedef ValType mapped_type;
  typedef std::pair<const KeyType, ValType> value_type;
  typedef Compare key_compare;
  typedef Alloc allocator_type;
  typedef value_type& reference;
  typedef const value_type& const_reference;
  typedef typename std::allocator_traits<Alloc>::pointer pointer;
  typedef typename std::allocator_traits<Alloc>::const_pointer const_pointer;
  // Iterator types.
  typedef Iterator iterator;
  typedef ConstIterator const_iterator;
  typedef std::reverse_iterator<iterator> reverse_iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

  typedef ptrdiff_t difference_type;
  typedef std::size_t size_type;

  // **** Constructor and destructor ****
  explicit Avl(const key_compare& comp = key_compare(),
               const allocator_type& alloc = allocator_type())
      : comp_(comp), alloc_(alloc) {
    Init();
  }

  explicit Avl(const allocator_type& alloc)
      : comp_(key_compare()), alloc_(alloc) {
    Init();
  }

  template <typename InputIterator>
  Avl(InputIterator first, InputIterator last,
      const key_compare& comp = key_compare(),
      const allocator_type& alloc = allocator_type())
      : comp_(comp), alloc_(alloc) {
    Init();
    insert(first, last);
  }

  Avl(const Avl& x) : comp_(key_compare()), alloc_(allocator_type()) {
    Init();
    insert(x.begin(), x.end());
  }

  Avl(const Avl& x, const allocator_type& alloc)
      : comp_(key_compare()), alloc_(alloc) {
    Init();
    insert(x.begin(), x.end());
  }
  Avl(std::initializer_list<value_type> il,
      const key_compare& comp = key_compare(),
      const allocator_type& alloc = allocator_type())
      : comp_(comp), alloc_(alloc) {
    Init();
    insert(il.begin(), il.end());
  }

  ~Avl() { clear(); }

  // **** Modifiers ****
  // Modifiers: Insert operation.
  // Single element.
  std::pair<iterator, bool> insert(const value_type& value);
  // TODO(ronaflx): find a good way to implement this.
  // template<typename P>
  // std::pair<iterator, bool> insert(P&& value);
  // With hint. TODO(ronaflx): find a good way to implement this.
  iterator insert(const_iterator position, const value_type& val);
  template <typename P>
  iterator insert(const_iterator position, P&& val);
  // Range.
  template <typename InputIterator>
  void insert(InputIterator first, InputIterator last);
  // Initializer list.
  void insert(std::initializer_list<value_type> il);

  // Modifiers: Erase operation.
  iterator erase(const_iterator position);
  size_type erase(const key_type& k);
  iterator erase(const_iterator first, const_iterator last);
  // Modifiers: Swap operation.
  void swap(Avl& other);
  // Modifiers: Clear operation.
  void clear();

  // **** Iterators ****
  // TODO(ronaflx): rend() is equal to end() and nullptr now, is this OK?
  // forward iterator for C++98.
  iterator begin() { return Iterator(head_, this); }
  iterator end() { return Iterator(nullptr, this); }
  // const forward iterator for C++98.
  const_iterator begin() const { return ConstIterator(head_, this); }
  const_iterator end() const { return ConstIterator(nullptr, this); }
  // const forward iterator for C++11.
  const_iterator cbegin() const { return ConstIterator(head_, this); }
  const_iterator cend() const { return ConstIterator(nullptr, this); }
  // reverse iterator for C++98.
  reverse_iterator rbegin() { return reverse_iterator(end()); }
  reverse_iterator rend() { return reverse_iterator(begin()); }
  // const reverse iterator for C++98.
  reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
  reverse_iterator rend() const { return const_reverse_iterator(begin()); }
  // const reverse iterator for C++11.
  const_reverse_iterator crbegin() const {
    return const_reverse_iterator(end());
  }
  const_reverse_iterator crend() const {
    return const_reverse_iterator(begin());
  }

  // **** Capacity ****
  bool empty() const;
  size_type size() const;
  size_type max_size() const;

  // **** Operations ****
  iterator find(const key_type& k);
  const_iterator find(const key_type& k) const;
  size_t count(const key_type& k) const;
  iterator lower_bound(const key_type& k);
  const_iterator lower_bound(const key_type& k) const;
  iterator upper_bound(const key_type& k);
  const_iterator upper_bound(const key_type& k) const;
  std::pair<iterator, iterator> equal_range(const key_type& k);
  std::pair<const_iterator, const_iterator> equal_range(const key_type& k)
      const;
  mapped_type& operator[](const key_type& k);

  // **** Allocator ****
  allocator_type get_allocator() const;

 private:
  struct Node {
    Avl::value_type value_;
    Node* left_, *right_;
    Node* prev_, *next_;
    Node* parent_;
    int height_;

    Node(const Avl::value_type& value)
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
          height_(1) {}
  };
  typedef typename Alloc::template rebind<Node>::other NodeAlloctorType;

  // Real field.
  Compare comp_;
  NodeAlloctorType alloc_;
  Node* head_, *tail_, *root_;
  size_type node_size_;

  // Node related operations.
  // Use 2 functions to operate the field height in case errors.
  inline int GetHeight(const Node* node) const {
    return node == nullptr ? 0 : node->height_;
  }
  inline void UpdateHeight(Node* node) {
    node->height_ =
        std::max(GetHeight(node->left_), GetHeight(node->right_)) + 1;
  }
  // Node memory management.
  inline Node* ConstructNode(const value_type& value) {
    node_size_++;
    Node* to_insert = alloc_.allocate(1);
    alloc_.construct(to_insert, value);
    return to_insert;
  }
  inline void DestroyNode(Node* node) {
    node_size_--;
    alloc_.destroy(node);
    alloc_.deallocate(node, 1);
  }
  inline Node** GetNodePointer(Node* node) {
    if (node->parent_ == nullptr) {
      return nullptr;
    }
    return node->parent_->left_ == node ? &node->parent_->left_
                                        : &node->parent_->right_;
  }

  // **** Helper ****
  // Balance operator of balanced binary search tree.
  void BalanceTree(Node*);
  void BalanceNode(Node**);
  void RightRotate(Node**);
  void LeftRotate(Node**);
  // Link the node as a doubly linked list.
  // Order after this operation finish: to_insert, curr
  inline void LinkPrev(Node* curr, Node* to_insert);
  // Order after this operation finish: curr, to_insert
  inline void LinkNext(Node* curr, Node* to_insert);
  // Swap two neighbors nodes in doubly linked node.
  inline void SwapNode(Node* curr, Node* to_swap);
  inline void Init();


 protected:
  class value_compare
      : public std::binary_function<value_type, value_type, bool> {
   private:
    friend class Avl<KeyType, ValType, Compare, Alloc>;

   protected:
    value_compare(Compare c) : c_(c) {}
    Compare c_;

   public:
    bool operator()(const value_type& lhs, const value_type& rhs) const {
      return c_(lhs, rhs);
    }
  };
};

// **** Iterators ****
template <typename KeyType, typename ValType, typename Compare, typename Alloc>
class Avl<KeyType, ValType, Compare, Alloc>::Iterator
    : public std::iterator<std::bidirectional_iterator_tag,
                           std::pair<const KeyType, ValType>> {
 public:
  friend ConstIterator;
  friend Avl<KeyType, ValType, Compare, Alloc>;
  typedef Avl::Node Node;
  Iterator() {}
  Iterator(Node* node, Avl* tree) : cur_node_(node), tree_(tree) {}
  Iterator(const Iterator& iter)
      : cur_node_(iter.cur_node_), tree_(iter.tree_) {}

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
    if (cur_node_ == nullptr) {
      cur_node_ = tree_->tail_;
    } else {
      cur_node_ = cur_node_->prev_;
    }
    return *this;
  }

  Iterator operator--(int) {
    Iterator ret = *this;
    --(*this);
    return ret;
  }

  bool operator==(const Iterator& i) {
    return cur_node_ == i.cur_node_ && tree_ == i.tree_;
  }
  bool operator!=(const Iterator& i) {
    return cur_node_ != i.cur_node_ || i.tree_ != i.tree_;
  }
  reference& operator*() { return cur_node_->value_; }
  pointer operator->() { return &cur_node_->value_; }

 private:
  Node* cur_node_;
  Avl* tree_;
};

template <typename KeyType, typename ValType, typename Compare, typename Alloc>
class Avl<KeyType, ValType, Compare, Alloc>::ConstIterator
    : public std::iterator<std::bidirectional_iterator_tag,
                           const std::pair<const KeyType, ValType>> {
 public:
  friend Avl<KeyType, ValType, Compare, Alloc>;
  typedef Avl::Node Node;
  ConstIterator() {}
  ConstIterator(const Node* node, const Avl* tree)
      : cur_node_(node), tree_(tree) {}
  ConstIterator(const Iterator& iter)
      : cur_node_(iter.cur_node_), tree_(iter.tree_) {}
  ConstIterator(const ConstIterator& iter)
      : cur_node_(iter.cur_node_), tree_(iter.tree_) {}

  ConstIterator& operator++() {
    cur_node_ = cur_node_->next_;
    return *this;
  }

  ConstIterator operator++(int) {
    ConstIterator ret = *this;
    ++(*this);
    return ret;
  }

  ConstIterator& operator--() {
    if (cur_node_ == nullptr) {
      cur_node_ = tree_->tail_;
    } else {
      cur_node_ = cur_node_->prev_;
    }
    return *this;
  }

  ConstIterator operator--(int) {
    ConstIterator ret = *this;
    --(*this);
    return ret;
  }

  bool operator==(const ConstIterator& i) {
    return cur_node_ == i.cur_node_ && tree_ == i.tree_;
  }
  bool operator!=(const ConstIterator& i) {
    return cur_node_ != i.cur_node_ || i.tree_ != i.tree_;
  }
  const_reference& operator*() { return cur_node_->value_; }
  const_pointer operator->() { return &cur_node_->value_; }

 private:
  const Node* cur_node_;
  const Avl* tree_;
};

// **** Helper ****
template <typename KeyType, typename ValType, typename Compare, typename Alloc>
inline void Avl<KeyType, ValType, Compare, Alloc>::LeftRotate(Node** node) {
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
inline void Avl<KeyType, ValType, Compare, Alloc>::RightRotate(Node** node) {
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
inline void Avl<KeyType, ValType, Compare, Alloc>::BalanceNode(Node** node) {
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
inline void Avl<KeyType, ValType, Compare, Alloc>::BalanceTree(Node* node) {
  while (node != root_) {
    UpdateHeight(node);
    BalanceNode(GetNodePointer(node));
    node = node->parent_;
  }
  UpdateHeight(root_);
  BalanceNode(&root_);
}

template <typename KeyType, typename ValType, typename Compare, typename Alloc>
inline void Avl<KeyType, ValType, Compare, Alloc>::LinkPrev(Node* curr,
                                                            Node* to_insert) {
  to_insert->prev_ = curr->prev_;
  if (curr->prev_ != nullptr) {
    curr->prev_->next_ = to_insert;
  }
  curr->prev_ = to_insert;
  to_insert->next_ = curr;
}

template <typename KeyType, typename ValType, typename Compare, typename Alloc>
inline void Avl<KeyType, ValType, Compare, Alloc>::LinkNext(Node* curr,
                                                            Node* to_insert) {
  to_insert->next_ = curr->next_;
  if (curr->next_ != nullptr) {
    curr->next_->prev_ = to_insert;
  }
  curr->next_ = to_insert;
  to_insert->prev_ = curr;
}

template <typename KeyType, typename ValType, typename Compare, typename Alloc>
inline void Avl<KeyType, ValType, Compare, Alloc>::SwapNode(Node* curr,
                                                            Node* to_swap) {
  curr->next_ = to_swap->next_;
  if (to_swap->next_ != nullptr) {
    to_swap->next_->prev_ = curr;
  }
  to_swap->next_ = curr;

  to_swap->prev_ = curr->prev_;
  if (curr->prev_ != nullptr) {
    curr->prev_->next_ = to_swap;
  }
  curr->prev_ = to_swap;
}
template <typename KeyType, typename ValType, typename Compare, typename Alloc>
inline void Avl<KeyType, ValType, Compare, Alloc>::Init() {
  head_ = nullptr;
  tail_ = nullptr;
  root_ = nullptr;
  node_size_ = 0ul;
}

// **** Modifiers ****
template <typename KeyType, typename ValType, typename Compare, typename Alloc>
void Avl<KeyType, ValType, Compare, Alloc>::clear() {
  for (Node* i = head_, *j; i != nullptr;) {
    j = i;
    i = i->next_;
    DestroyNode(j);
  }
  root_ = head_ = tail_ = nullptr;
}

template <typename KeyType, typename ValType, typename Compare, typename Alloc>
std::pair<typename Avl<KeyType, ValType, Compare, Alloc>::iterator, bool>
Avl<KeyType, ValType, Compare, Alloc>::insert(const value_type& value) {
  Node* curr = root_, *parent = nullptr;
  while (curr != nullptr) {
    parent = curr;
    if (comp_(curr->value_.first, value.first)) {
      curr = curr->right_;
    } else if (comp_(value.first, curr->value_.first)) {
      curr = curr->left_;
    } else {
      return std::make_pair(Iterator(curr, this), false);
    }
  }
  Node* to_insert = ConstructNode(value);
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
    BalanceTree(parent);
  }
  return std::make_pair(Iterator(to_insert, this), true);
}

// template <typename KeyType, typename ValType, typename Compare, typename
// Alloc>
// template <typename P>
// std::pair<typename Avl<KeyType, ValType, Compare, Alloc>::iterator, bool>
// Avl<KeyType, ValType, Compare, Alloc>::insert(P&& value) {
// }

template <typename KeyType, typename ValType, typename Compare, typename Alloc>
typename Avl<KeyType, ValType, Compare, Alloc>::iterator
Avl<KeyType, ValType, Compare, Alloc>::insert(const_iterator position,
                                              const value_type& val) {
  return insert(val).first;
}

template <typename KeyType, typename ValType, typename Compare, typename Alloc>
template <class P>
typename Avl<KeyType, ValType, Compare, Alloc>::iterator
Avl<KeyType, ValType, Compare, Alloc>::insert(const_iterator position,
                                              P&& val) {
  return insert(val).first;
}

template <typename KeyType, typename ValType, typename Compare, typename Alloc>
template <class InputIterator>
void Avl<KeyType, ValType, Compare, Alloc>::insert(InputIterator first,
                                                   InputIterator last) {
  for (; first != last; ++first) {
    insert(*first);
  }
}

template <typename KeyType, typename ValType, typename Compare, typename Alloc>
void Avl<KeyType, ValType, Compare, Alloc>::insert(
    std::initializer_list<value_type> il) {
  insert(il.begin(), il.end());
}

template <typename KeyType, typename ValType, typename Compare, typename Alloc>
typename Avl<KeyType, ValType, Compare, Alloc>::iterator
Avl<KeyType, ValType, Compare, Alloc>::erase(const_iterator position) {
  Node* to_erase = const_cast<Node*>(position.cur_node_);
  iterator ret_iter = Iterator(to_erase->next_, this);

  // Change the head and tail if erased is head/tail.
  if (to_erase == tail_) tail_ = to_erase->prev_;
  if (to_erase == head_) head_ = to_erase->next_;

  // TODO(ronaflx): Can we simptify ths or split in to functions?
  while (to_erase->right_ != nullptr) {
    Node* to_swap = to_erase->next_;
    Node** to_erase_p = GetNodePointer(to_erase);
    if (to_erase_p != nullptr) *to_erase_p = to_swap;

    // This means to_swap is right child of to_erase.
    if (to_erase == to_swap->parent_) {
      to_swap->parent_ = to_erase->parent_;
      to_erase->parent_ = to_swap;

      if (to_swap->right_ != nullptr) to_swap->right_->parent_ = to_erase;
      to_erase->right_ = to_swap->right_;
      to_swap->right_ = to_erase;
    } else {
      Node** to_swap_p = GetNodePointer(to_swap);
      if (to_swap_p != nullptr) *to_swap_p = to_erase;
      std::swap(to_swap->parent_, to_erase->parent_);

      if (to_swap->right_ != nullptr) to_swap->right_->parent_ = to_erase;
      if (to_erase->right_ != nullptr) to_erase->right_->parent_ = to_swap;
      std::swap(to_swap->right_, to_erase->right_);
    }
    // Link the right child of to_swap and to_erase
    if (to_swap->left_ != nullptr) to_swap->left_->parent_ = to_erase;
    if (to_erase->left_ != nullptr) to_erase->left_->parent_ = to_swap;
    std::swap(to_swap->left_, to_erase->left_);

    SwapNode(to_erase, to_swap);
    // Change root of the tree if erased node is root.
    if (to_erase == root_) root_ = to_swap;
  }

  Node* parent = to_erase->parent_;
  // In case to_erase is root.
  if (parent != nullptr) {
    if (parent->left_ == to_erase) {
      parent->left_ = to_erase->left_;
      if (to_erase->left_ != nullptr) {
        to_erase->left_->parent_ = parent;
      }
    } else {
      parent->right_ = to_erase->left_;
      if (to_erase->left_ != nullptr) {
        to_erase->left_->parent_ = parent;
      }
    }
    if (to_erase->prev_ != nullptr) {
      to_erase->prev_->next_ = to_erase->next_;
    }
    if (to_erase->next_ != nullptr) {
      to_erase->next_->prev_ = to_erase->prev_;
    }
  } else {
    root_ = to_erase->left_;
    if (root_ != nullptr) {
      root_->parent_ = nullptr;
    }
  }

  DestroyNode(to_erase);
  if (parent) BalanceTree(parent);
  return ret_iter;
}

template <typename KeyType, typename ValType, typename Compare, typename Alloc>
typename Avl<KeyType, ValType, Compare, Alloc>::size_type
Avl<KeyType, ValType, Compare, Alloc>::erase(const key_type& k) {
  std::pair<iterator, iterator> i = equal_range(k);
  iterator start = i.first;
  size_type res = 0;
  while (start != i.second) {
    start = erase(start);
  }
  return res;
}

template <typename KeyType, typename ValType, typename Compare, typename Alloc>
typename Avl<KeyType, ValType, Compare, Alloc>::iterator
Avl<KeyType, ValType, Compare, Alloc>::erase(const_iterator first,
                                             const_iterator last) {
  while (first != last) {
    first = erase(first);
  }
  return Iterator(const_cast<Node*>(first.cur_node_), this);
}

template <typename KeyType, typename ValType, typename Compare, typename Alloc>
void Avl<KeyType, ValType, Compare, Alloc>::swap(Avl& tree) {
  if (tree.root_ != nullptr || root_ != nullptr) {
    std::swap(comp_, tree.comp_);
    std::swap(tail_, tree.tail_);
    std::swap(head_, tree.head_);
    std::swap(root_, tree.root_);
    std::swap(node_size_, tree.node_size_);
    if (alloc_ != tree.alloc_) {
      std::swap(alloc_, tree.alloc_);
    }
  }
}

// **** Capacity ****
template <typename KeyType, typename ValType, typename Compare, typename Alloc>
bool Avl<KeyType, ValType, Compare, Alloc>::empty() const {
  return size() == 0;
}

template <typename KeyType, typename ValType, typename Compare, typename Alloc>
typename Avl<KeyType, ValType, Compare, Alloc>::size_type
Avl<KeyType, ValType, Compare, Alloc>::size() const {
  return node_size_;
}

template <typename KeyType, typename ValType, typename Compare, typename Alloc>
typename Avl<KeyType, ValType, Compare, Alloc>::size_type
Avl<KeyType, ValType, Compare, Alloc>::max_size() const {
  return alloc_.max_size();
}

// **** Operations ****
template <typename KeyType, typename ValType, typename Compare, typename Alloc>
typename Avl<KeyType, ValType, Compare, Alloc>::iterator
Avl<KeyType, ValType, Compare, Alloc>::find(const key_type& k) {
  Node* curr = root_;
  while (curr != nullptr) {
    if (comp_(curr->value_.first, k)) {
      curr = curr->right_;
    } else if (comp_(k, curr->value_.first)) {
      curr = curr->left_;
    } else {
      break;
    }
  }
  return Iterator(curr, this);
}

template <typename KeyType, typename ValType, typename Compare, typename Alloc>
typename Avl<KeyType, ValType, Compare, Alloc>::const_iterator
Avl<KeyType, ValType, Compare, Alloc>::find(const key_type& k) const {
  const Node* curr = root_;
  while (curr != nullptr) {
    if (comp_(curr->value_.first, k)) {
      curr = curr->right_;
    } else if (comp_(k, curr->value_.first)) {
      curr = curr->left_;
    } else {
      break;
    }
  }
  return Iterator(curr, this);
}

template <typename KeyType, typename ValType, typename Compare, typename Alloc>
typename Avl<KeyType, ValType, Compare, Alloc>::iterator
Avl<KeyType, ValType, Compare, Alloc>::lower_bound(const key_type& key) {
  Node* res = nullptr, *cur = root_;
  while (cur != nullptr) {
    if (comp_(cur->value_.first, key)) {
      cur = cur->right_;
    } else {
      res = cur;
      cur = cur->left_;
    }
  }
  return Iterator(res, this);
}

template <typename KeyType, typename ValType, typename Compare, typename Alloc>
typename Avl<KeyType, ValType, Compare, Alloc>::const_iterator
Avl<KeyType, ValType, Compare, Alloc>::lower_bound(const key_type& key) const {
  const Node* res = nullptr, *cur = root_;
  while (cur != nullptr) {
    if (comp_(cur->value_.first, key)) {
      cur = cur->right_;
    } else {
      res = cur;
      cur = cur->left_;
    }
  }
  return ConstIterator(res, this);
}

template <typename KeyType, typename ValType, typename Compare, typename Alloc>
typename Avl<KeyType, ValType, Compare, Alloc>::iterator
Avl<KeyType, ValType, Compare, Alloc>::upper_bound(const key_type& key) {
  Node* res = nullptr, *cur = root_;
  while (cur != nullptr) {
    if (comp_(key, cur->value_.first)) {
      res = cur;
      cur = cur->left_;
    } else {
      cur = cur->right_;
    }
  }
  return Iterator(res, this);
}

template <typename KeyType, typename ValType, typename Compare, typename Alloc>
typename Avl<KeyType, ValType, Compare, Alloc>::const_iterator
Avl<KeyType, ValType, Compare, Alloc>::upper_bound(const key_type& key) const {
  const Node* res = nullptr, *cur = root_;
  while (cur != nullptr) {
    if (comp_(key, cur->value_.first)) {
      res = cur;
      cur = cur->left_;
    } else {
      cur = cur->right_;
    }
  }
  return ConstIterator(res, this);
}

template <typename KeyType, typename ValType, typename Compare, typename Alloc>
std::pair<typename Avl<KeyType, ValType, Compare, Alloc>::iterator,
          typename Avl<KeyType, ValType, Compare, Alloc>::iterator>
Avl<KeyType, ValType, Compare, Alloc>::equal_range(const key_type& key) {
  return std::make_pair(lower_bound(key), upper_bound(key));
}

template <typename KeyType, typename ValType, typename Compare, typename Alloc>
std::pair<typename Avl<KeyType, ValType, Compare, Alloc>::const_iterator,
          typename Avl<KeyType, ValType, Compare, Alloc>::const_iterator>
Avl<KeyType, ValType, Compare, Alloc>::equal_range(const key_type& key) const {
  return std::make_pair(lower_bound(key), upper_bound(key));
}

template <typename KeyType, typename ValType, typename Compare, typename Alloc>
typename Avl<KeyType, ValType, Compare, Alloc>::mapped_type&
Avl<KeyType, ValType, Compare, Alloc>::operator[](const key_type& k) {
  auto iter = find(k);
  if (iter == end()) {
    return insert(std::make_pair(k, mapped_type())).first->second;
  } else {
    return iter->second;
  }
}

// **** Allocator ****
template <typename KeyType, typename ValType, typename Compare, typename Alloc>
typename Avl<KeyType, ValType, Compare, Alloc>::allocator_type
Avl<KeyType, ValType, Compare, Alloc>::get_allocator() const {
  return allocator_type(alloc_);
}
