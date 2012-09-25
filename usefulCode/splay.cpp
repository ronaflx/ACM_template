struct node
{
/* virtual node if tot is equal to 0*/
#define __JUDGE if(tot == 0) return;
	static const int INF = 100000000;
	node* ch[2], *pre;
	int v, minn, tot, delta,  flip;
	node(int v, int tot, node* l, node* r, node* pre)
		: pre(pre), v(v), minn (v), tot(tot), delta(0), flip(0) {
		ch[0] = l, ch[1] = r;
	}
	inline int min_v() { return minn; }
	inline int size() { return tot; }
	void reverse() { __JUDGE flip ^= 1; }
	void add(int d) { __JUDGE minn += d, delta += d, v += d; }
	void push_down() {
		__JUDGE
		if(delta) {
			if(ch[0]->tot) ch[0]->add(delta);
			if(ch[1]->tot) ch[1]->add(delta);
		}
		if(flip) {
			swap(ch[0], ch[1]);
			if(ch[0]->tot) ch[0]->reverse();
			if(ch[1]->tot) ch[1]->reverse();
		}
		flip = delta = 0;
	}
	void push_up() {
		__JUDGE
		tot = ch[0]->size() + ch[1]->size() + 1;
		minn = min(v, min(ch[0]->min_v(), ch[1]->min_v()));
	}
};
class splay_tree {
public:
	splay_tree() {
		null = new node(node::INF, 0, 0, 0, 0);
		root = null;
	}
	~splay_tree() {
		clear(root);
		delete null;
	}
	// make a sequence from 1 to n do build(0, n + 1, val)
	// and make sure val[0] = va[1] = INF;
	void build(int l, int r, int* val) {
		if(l > r) return;
		build(l, r, root, null, val);
	}
#define centre (root->ch[1]->ch[0])
	int min_value(int a, int b) {
		makeInterval(a, b);
		return centre->min_v();
	}
	void add_value(int a, int b, int value) {
		makeInterval(a, b);
		centre->add(value);
		splay(centre, null);
	}
	void reverse(int a, int b) {
		if(a == b) return;
		makeInterval(a, b);
		centre->reverse();
		splay(centre, null);
	}
	void revolve(int a, int b, int c) { // c < b - a + 1
		if(c == 0) return;
		int len = b - a + 1;
		reverse(a, a + len - c - 1);
		reverse(a + len - c, b);
		reverse(a, b);
	}
	void insert(int a, int c) {
		makeInterval(a + 1, a);
		centre = new node(c, 1, null, null, root->ch[1]);
		root->ch[1]->push_up();
		root->push_up();
		splay(centre, null);
	}
	void erase(int a) {
		makeInterval(a, a);
		delete centre;
		centre = null;
		root->ch[1]->push_up();
		root->ch[0]->push_up();
	}
#undef centre
	void clear() { clear(root); }
private:
	node* root, * null;
	void clear(node*& now) {
		if(now == null) return;
		clear(now->ch[0]);
		clear(now->ch[1]);
		delete now;
		now = null;
	}
	/* 0: right rotate
	 * 1: left  rotate*/
	void rotate(node* x, int type) {
		node *y = x->pre;
		y->push_down(), x->push_down();
		y->ch[!type] = x->ch[type];
		if (x->ch[type] != null) 
			x->ch[type]->pre = y;
		x->pre = y->pre;
		if (y->pre != null) {
			if(y->pre->ch[1] == y)
				y->pre->ch[1] = x;
			else
				y->pre->ch[0] = x;
		}
		x->ch[type] = y, y->pre = x;
		if (y == root) root = x; 	
		y->push_up(), x->push_up();
	}
	void splay(node* x, node* f) {
		x->push_down();
		while(x->pre != f) {
			if (x->pre->pre == f) {
				if (x->pre->ch[0] == x) 
					rotate(x, 1);
				else 
					rotate(x, 0);
			} else {
				node *y = x->pre;
				node *z = y->pre;
				if (z->ch[0] == y) {
					if (y->ch[0] == x) // 1
						rotate(y, 1), rotate(x, 1);
					else // z
						rotate(x, 0), rotate(x, 1);
				} else {
					if (y->ch[1] == x) // 1
						rotate(y, 0), rotate(x, 0);
					else // z
						rotate(x, 1), rotate(x, 0);
				}
			}
		}
		x->push_up();
	}
	void build(int l, int r, node*& now, node* pre, int* val) {
		if(l > r) return;
		int mid = (l + r) / 2;
		now = new node(val[mid], 1, null, null, pre);
		build(l, mid - 1, now->ch[0], now, val);
		build(mid + 1, r, now->ch[1], now, val);
		now->push_up();
	}
	// the flag node is !not! included, be careful when make interval
	void findK(int k, node* pre) {
		node* now = root;
		while(true) {
			now->push_down();
			int s = now->ch[0]->size();
			if(s == k)
				break;
			else if(s > k)
				now = now->ch[0];
			else {
				now = now->ch[1];
				k -= s + 1;
			}
		}
		splay(now, pre);
	}
	void makeInterval(int a, int b) {
		findK(a - 1, null);
		findK(b + 1, root);
	}
}tree;
const int N = 300000;
int val[N];
int main() {
	int n, m;
	int a, b, c;
	char cmd[100];
	while(scanf("%d", &n) == 1) {
		for(int i = 1;i <= n;i++)
			scanf("%d", &val[i]);
		val[0] = val[n + 1] = node::INF;
		tree.clear();
		tree.build(0, n + 1, val);
		scanf("%d", &m);
		REP(i, 0, m) {
			scanf("%s", cmd);
			if(!strcmp(cmd, "ADD")) {
				scanf("%d %d %d", &a, &b, &c);
				tree.add_value(a, b, c);
			} else if(!strcmp(cmd, "REVERSE")) {
				scanf("%d %d", &a, &b);
				tree.reverse(a, b);
			} else if(!strcmp(cmd, "REVOLVE")) {
				scanf("%d %d %d", &a, &b, &c);
				int tot = b - a + 1;
				c = (c % tot + tot) % tot;
				tree.revolve(a, b, c);
			} else if(!strcmp(cmd, "INSERT")) {
				scanf("%d %d", &a, &c);
				tree.insert(a, c);
			} else if(!strcmp(cmd, "DELETE")) {
				scanf("%d", &a);
				tree.erase(a);
			} else if(!strcmp(cmd, "MIN")) {
				scanf("%d %d", &a, &b);
				printf("%d\n", tree.min_value(a, b));
			}
		}
	}
	return 0;
}


