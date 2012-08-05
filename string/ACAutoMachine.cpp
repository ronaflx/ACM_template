#define code(ch) ((ch) - 'A')
const int KIND = 26, MAXN = 3000000;
struct node {
	node* nxt[KIND], *fail;
	int count, id, ori;
} pool[MAXN], *pp, *root, *q[MAXN];
node *newNode() {
	pp->fail = NULL;
	pp->count = pp->ori = 0;
	memset(pp->nxt, 0, sizeof (pp->nxt));
	return pp++;
}
void initialize() {
	pp = pool;
	root = newNode();
}
void insert(const char * str, int id) {
	node * now = root;
	while (*str) {
		int i = code(*str);
		now->nxt[i] = now->nxt[i] == 0 ? newNode() : now->nxt[i];
		now = now->nxt[i];
		str++;
	}
	now->count++, now->ori++, now->id = id;
}
void buildFail(node*& now, int ith) {//build now's ith son 
	if(now == root) now->nxt[ith]->fail = root;
	node* tmp = now->fail;
	while(tmp) {
		if(tmp->nxt[ith] != NULL) {
			now->nxt[ith]->fail = tmp->nxt[ith];
			return;
		}
		tmp = tmp->fail;
	}
	if(tmp == NULL) now->nxt[ith]->fail = root;
}
void build() {
	int head, tail = 0;
	q[tail++] = root;
	while (head != tail) {
		node * beg = q[head++];
		for (int i = 0; i < KIND; i++) {
			if (beg->nxt[i] == NULL) continue;
			buildFail(beg, i);
			q[tail++] = beg->nxt[i];
		}
	}
}
/*注意这里更改了nxt, 如果需要nxt等于NULL把tmp去掉*/
node* goStatus(node* now, int ith) {
	node * tmp = now;
	while(now->nxt[ith] == NULL && now != root)
		now = now->fail;
	now = now->nxt[ith];
	return tmp->nxt[ith] = (now == NULL ? root : now);
}
void query(const char* str) {
	node * p = root, * tmp;
	int tail = 0;
	while (*str) {
		tmp = p = goStatus(p, code(*str));
		while (tmp != root && tmp->count != -1) {
			q[tail++] = tmp;
			tmp->count = -1;
			tmp = tmp->fail;
		}
		str++;
	}
	REP(i, 0, tail) {/*加入了查询恢复功能*/
		q[i]->count = q[i]->ori;
	}
}
