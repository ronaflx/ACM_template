const int SIZE = 33 * 100000;
const int KIND = 26;
struct node {
	node *child[KIND];
	int final;
} pool[SIZE],*root,*last;
void build() {
	last = root = pool;
	memset(pool, 0, sizeof(pool));
}
void insert(const char *from) {
	node * p = root;
	for (char *i = from; *i; i++) {
		if (p->child[*i - 'a'] == NULL)
			p->child[*i - 'a'] = ++last;
		p = p->child[*i - 'a'];
		p->final++;
	}
}
int query(const char *from) {
	node * p = root;
	for (char *i = from; *i; i++) {
		p = p->child[*i - 'a'];
		if (p == NULL) return 0;
	}
	return p->final;
}
