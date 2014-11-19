#include <iterator>
#include <memory>
#include <cstdlib>
#include <cassert>

template <typename KeyType, typename Comp = std::less<KeyType>,
          typename Alloc = std::allocator<KeyType>>
class SkipList {
 public:
  class Iterator;
  class ConstIterator;

  typedef KeyType key_type;
  typedef key_type& reference;
  typedef const key_type& const_reference;
  typedef Comp key_compare;
  typedef Alloc allocator_type;

  typedef Iterator iterator;
  typedef ConstIterator const_iterator;

  typedef ptrdiff_t difference_type;
  typedef std::size_t size_type;

  // Any value for head_ is OK, since this is a virtual node.
  SkipList(const key_compare& comp = key_compare(),
           const allocator_type alloc = allocator_type())
      : comp_(comp),
        alloc_(alloc),
        head_(ConstructNode(0)),
        max_level_(1),
        size_(0) {}
  ~SkipList();

  iterator begin();
  iterator end();
  const_iterator begin() const;
  const_iterator end() const;

  void clear();
  void insert(const_reference key);
  void erase(const_reference key);
  size_type size() const;
  bool find(const_reference key) const;
  iterator lower_bound(const_reference);
  iterator upper_bound(const_reference);
  const_iterator lower_bound(const_reference) const;
  const_iterator upper_bound(const_reference) const;

 private:
  class Node;
  static const int kMaxHeight = 16;

  typedef typename Alloc::template rebind<Node>::other NodeAlloctorType;
  // Internal field.
  Comp comp_;               // Compare function
  NodeAlloctorType alloc_;  // Memory allocator
  Node* head_;              // Virtual head node
  size_t max_level_;        // current max_level
  size_t size_;             // Size of list

  inline Node* ConstructNode(const key_type key) {
    size_++;
    Node* to_insert = alloc_.allocate(1);
    alloc_.construct(to_insert, key);
    return to_insert;
  }

  inline void DestroyNode(Node* node) {
    size_--;
    alloc_.destroy(node);
    alloc_.deallocate(node, 1);
  }

  // prev[i] record the last element small than key on ith level.
  Node* GreaterEqual(const_reference& key, Node** prev) const;
  int RandomHeight() const;
};

// *** Internal class definition ***
template <typename KeyType, typename Comp, typename Alloc>
class SkipList<KeyType, Comp, Alloc>::Node {
 public:
  KeyType key_;
  Node* next_[kMaxHeight];
  Node(const KeyType& key) : key_(key) { memset(next_, 0, sizeof(next_)); }
};

// *** Internal function definition ***
template <typename KeyType, typename Comp, typename Alloc>
typename SkipList<KeyType, Comp, Alloc>::Node*
SkipList<KeyType, Comp, Alloc>::GreaterEqual(const_reference& key,
                                             Node** prev) const {
  Node* x = head_;
  for (int i = max_level_ - 1; i >= 0; i--) {
    Node* next_node = x->next_[i];
    while (next_node != nullptr && comp_(next_node->key_, key)) {
      x = next_node;
      next_node = x->next_[i];
    }
    if (prev != nullptr) prev[i] = x;
  }
  x = x->next_[0];
  return x;
}

template <typename KeyType, typename Comp, typename Alloc>
int SkipList<KeyType, Comp, Alloc>::RandomHeight() const {
  // the probability of increase height by one is 1/kProbability.
  static const unsigned kProbability = 4;
  int height = 1;
  while (rand() % kProbability == 0 && height < kMaxHeight) {
    height++;
  }
  return height;
}

// *** Iterator definition ***
// Iterator always point to level 0 node.
template <typename KeyType, typename Comp, typename Alloc>
class SkipList<KeyType, Comp, Alloc>::Iterator
    : public std::iterator<std::forward_iterator_tag, key_type> {
 public:
  Iterator(SkipList* skip_list) : list_(skip_list), node_(skip_list->next[0]) {}
  Iterator(SkipList* skip_list, Node* node) : list_(skip_list), node_(node) {}
  Iterator(const Iterator& iter) : list_(iter.list_), node_(iter.node_) {}

  Iterator operator++(int) {
    Iterator ret(*this);
    ++(*this);
    return ret;
  }

  Iterator& operator++() {
    node_ = node_->next_[0];
    return *this;
  }

  bool operator !=(const Iterator& other) const {
    return list_ != other.list_ || node_ != other.node_;
  }

  bool operator ==(const Iterator& other) const {
    return list_ == other.list_ && node_ == other.node_;
  }

  const_reference operator* () const {
    return node_->key_;
  }

 private:
  SkipList* list_;
  Node* node_;
};

template <typename KeyType, typename Comp, typename Alloc>
class SkipList<KeyType, Comp, Alloc>::ConstIterator
    : public std::iterator<std::forward_iterator_tag, const key_type> {
 public:
  ConstIterator(const SkipList* skip_list)
      : list_(skip_list), node_(skip_list->next[0]) {}
  ConstIterator(const SkipList* skip_list, const Node* node)
      : list_(skip_list), node_(node) {}
  ConstIterator(const Iterator& iter) : list_(iter.list_), node_(iter.node_) {}
  ConstIterator(const ConstIterator& iter)
      : list_(iter.list_), node_(iter.node_) {}

  Iterator operator++(int) {
    Iterator ret(*this);
    ++(*this);
    return ret;
  }

  Iterator& operator++() {
    node_ = node_->next_[0];
    return *this;
  }

  bool operator !=(const Iterator& other) const {
    return list_ != other.list_ || node_ != other.node_;
  }

  bool operator ==(const Iterator& other) const {
    return list_ == other.list_ && node_ == other.node_;
  }

  const_reference operator* () const {
    return node_->key_;
  }

 private:
  const SkipList* list_;
  const Node* node_;
};

// *** Public function. ***
template <typename KeyType, typename Comp, typename Alloc>
SkipList<KeyType, Comp, Alloc>::~SkipList() {
  clear();
  DestroyNode(head_);
}

template <typename KeyType, typename Comp, typename Alloc>
typename SkipList<KeyType, Comp, Alloc>::iterator
SkipList<KeyType, Comp, Alloc>::begin() {
  return iterator(this, head_->next_[0]);
}

template <typename KeyType, typename Comp, typename Alloc>
typename SkipList<KeyType, Comp, Alloc>::iterator
SkipList<KeyType, Comp, Alloc>::end() {
  return iterator(this, nullptr);
}

template <typename KeyType, typename Comp, typename Alloc>
typename SkipList<KeyType, Comp, Alloc>::const_iterator
SkipList<KeyType, Comp, Alloc>::begin() const {
  return iterator(this, head_->next_[0]);
}

template <typename KeyType, typename Comp, typename Alloc>
typename SkipList<KeyType, Comp, Alloc>::const_iterator
SkipList<KeyType, Comp, Alloc>::end() const {
  return iterator(this, nullptr);
}

template <typename KeyType, typename Comp, typename Alloc>
void SkipList<KeyType, Comp, Alloc>::clear() {
  while (head_->next_[0] != nullptr) {
    Node* to_erase = head_->next_[0];
    for (int i = 0; i < max_level_; i++) {
      head_->next_[i] = to_erase->next_[i];
    }
    DestroyNode(to_erase);
  }
  max_level_ = 1;
}

template <typename KeyType, typename Comp, typename Alloc>
void SkipList<KeyType, Comp, Alloc>::insert(const_reference& key) {
  Node* prev[kMaxHeight];
  const Node* x = GreaterEqual(key, prev);
  if (x != nullptr && !comp_(key, x->key_)) {
    return;
  }
  const int level = RandomHeight();
  if (level > max_level_) {
    for (int i = max_level_; i < level; i++) {
      prev[i] = head_;
    }
    max_level_ = level;
  }

  Node* to_insert = ConstructNode(key);
  for (int i = 0; i < level; i++) {
    to_insert->next_[i] = prev[i]->next_[i];
    prev[i]->next_[i] = to_insert;
  }
}

template <typename KeyType, typename Comp, typename Alloc>
void SkipList<KeyType, Comp, Alloc>::erase(const_reference& key) {
  Node* prev[kMaxHeight];
  const Node* x = GreaterEqual(key, prev);
  if (x != nullptr && !comp_(key, x->key_)) {
    for (int i = 0; i < max_level_; i++) {
      if (prev[i]->next_[i] != x) break;
      prev[i]->next_[i] = x->next_[i];
    }
    DestroyNode(x);
    while (max_level_ > 1 && head_->next_[max_level_ - 1] == nullptr) {
      max_level_--;
    }
  }
}

template <typename KeyType, typename Comp, typename Alloc>
typename SkipList<KeyType, Comp, Alloc>::size_type
SkipList<KeyType, Comp, Alloc>::size() const {
  return size_;
}

template <typename KeyType, typename Comp, typename Alloc>
bool SkipList<KeyType, Comp, Alloc>::find(const_reference& key) const {
  const Node* x = GreaterEqual(key, nullptr);
  if (x != nullptr && !comp_(key, x->key_)) {
    return true;
  } else {
    return false;
  }
}

template <typename KeyType, typename Comp, typename Alloc>
typename SkipList<KeyType, Comp, Alloc>::const_iterator
SkipList<KeyType, Comp, Alloc>::lower_bound(const_reference& key) const {
  const Node* x = GreaterEqual(key, nullptr);
  return const_iterator(this, x);
}

template <typename KeyType, typename Comp, typename Alloc>
typename SkipList<KeyType, Comp, Alloc>::const_iterator
SkipList<KeyType, Comp, Alloc>::upper_bound(const_reference& key) const {
  const Node* x = GreaterEqual(key, nullptr);
  if (x != nullptr && !comp_(key, x->key_)) {
    x = x->next_[0];
  }
  return const_iterator(this, x);
}

template <typename KeyType, typename Comp, typename Alloc>
typename SkipList<KeyType, Comp, Alloc>::iterator
SkipList<KeyType, Comp, Alloc>::lower_bound(const_reference& key) {
  Node* x = GreaterEqual(key, nullptr);
  return iterator(this, x);
}

template <typename KeyType, typename Comp, typename Alloc>
typename SkipList<KeyType, Comp, Alloc>::iterator
SkipList<KeyType, Comp, Alloc>::upper_bound(const_reference& key) {
  Node* x = GreaterEqual(key, nullptr);
  if (x != nullptr && !comp_(key, x->key_)) {
    x = x->next_[0];
  }
  return iterator(this, x);
}
