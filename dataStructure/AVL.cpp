template<class T>
struct node {
	T value;
	int h, size;
	node<T> *lchild, *rchild;
	node() {}
	node(T value, int h, int size, node<T> *lchild, node<T> *rchild)
		:value(value), h(h), size(size), lchild(lchild), rchild(rchild) {}
};
template<class T>
class AVL {
public:
typedef node<T>* node_ptr;
	AVL() {
		root = nilptr = new node<T>(T(), 0, 0, NULL, NULL);
	}
	~AVL() {
		clear();
		delete nilptr;
	}
	int size() {
		return root->size;
	}
	void insert(T k) {
		insert(root, k);
	}
	bool erase(T k) {
		return erase(root, k);
	}
	bool empty() {
		return root == nilptr;
	}
	node_ptr findK(int k) {
		return findK(root, k);
	}
private:
	node_ptr root, nilptr;
	node_ptr findK(const node_ptr &R, int k) {
		if (k == R->lchild->size + 1) {
			return R;
		} else if (k <= R->lchild->size) {
			return findK(R->lchild, k);
		} else {
			return findK(R->rchild, k - R->lchild->size - 1);
		}
	}
	void fix(node_ptr &R) {
		R->h = max(R->rchild->h, R->lchild->h) + 1;
		R->size = R->rchild->size + R->lchild->size + 1;
	}
	void clear(node_ptr &R) {
		if(R == nilptr) return;
		clear(R->lchild);
		clear(R->rchild);
		delete R;
	}
	void rightrotate(node_ptr &R) {
		node_ptr lc = R->lchild;
		R->lchild = lc->rchild;
		fix(R);
		lc->rchild = R;
		R = lc;
		fix(R);
	}
	void leftrotate(node_ptr &R) {
		node_ptr rc = R->rchild;
		R->rchild = rc->lchild;
		fix(R);
		rc->lchild = R;
		R = rc;
		fix(R);
	}
	void maintain(node_ptr &R) {
		if (R->lchild != nilptr) {
			if (R->lchild->lchild->h == R->rchild->h + 1) {
				rightrotate(R);
            } else if (R->lchild->rchild->h == R->rchild->h + 1) {
                leftrotate(R->lchild);
                rightrotate(R);
            }
		}
		if (R->rchild != nilptr) {
			if (R->rchild->rchild->h == R->lchild->h + 1) {
				leftrotate(R);
            } else if (R->rchild->lchild->h == R->lchild->h + 1) {
                rightrotate(R->rchild);
                leftrotate(R);
            }
		}
	}
	void insert(node_ptr &R, T value) {
		if (R == nilptr) {
			R = new node<T>(value, 0, 1, nilptr, nilptr);
			return;
		} else if (value <= R->value) {
			insert(R->lchild, value);
		} else if (value > R->value) {
			insert(R->rchild, value);
		}
		fix(R);
		maintain(R);
	}
	bool erase(node_ptr &R, T value) {
		bool ret = false;
		if (R == nilptr) {
			return false;
		} else if (R->value == value) {
			if (R->rchild == nilptr) {
				node_ptr tmp = R;
				R = tmp->lchild;
				delete tmp;
			} else {
				node_ptr tmp = R->rchild;
				while (tmp->lchild != nilptr)
					tmp = tmp->lchild;
				R->value = tmp->value;
				erase(R->rchild, tmp->value);
				fix(R);
			}
			return true;
		} else if (value < R->value) {
			ret = ret || erase(R->lchild, value);
		} else {
			ret = ret || erase(R->rchild, value);
		}
		fix(R);
		maintain(R);
		return ret;
	}
};
