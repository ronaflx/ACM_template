const int V = 10000, E = V << 1;
struct edge {
	int v, c;
	edge *nxt;
}pool[E], *g[V], *pp;
//initialize and add_edge Function
int _size[V], vis[V], dist[V], root, maxn;
void select(int v, int pre, int tree_size) {
	int max_sub = 0;
	_size[v] = 1;
	for(edge* i = g[v];i != NULL;i = i->nxt) {
		if(i->v == pre || vis[i->v]) continue;
		select(i->v, v, tree_size);
		_size[v] += _size[i->v];
		checkmax(max_sub, _size[i->v]);
	}
	checkmax(max_sub, tree_size - _size[v]);
	if(checkmin(maxn, max_sub))
		root = v;
}
int _count(int beg, int end, int k) {
	int ret = 0, lo = beg, hi = end - 1;
	sort(dist + beg, dist + end);
	while(lo < hi) {
		if(dist[hi] + dist[lo] <= k) 
			ret += hi - lo++;
		else
			hi--;
	}
	return ret;
}
void dfs(int root, int pre, int len, int& end) {
	dist[end++] = len, _size[root] = 1;
	for(edge *i = g[root];i != NULL;i = i->nxt) {
		if(i->v == pre || vis[i->v]) continue;
		dfs(i->v, root, len + i->c, end);
		_size[root] += _size[i->v];
	}
}
int get_sub_solve(int root, int k) {
	int beg = 0, end = 0, res = 0;
	for(edge*i = g[root];i != NULL;i = i->nxt) {
		if(vis[i->v]) continue;
		dfs(i->v, root, i->c, end);
		res -= _count(beg, end, k);
		beg = end;
	}
	dist[end++] = 0;
	res += _count(0, end, k);
	return res;
}
//number of path less than k
void solve(int n, int k) {
	queue<int> q;
	q.push(0);
	_size[0] = n;
	CC(vis, 0);
	int res = 0;
	while(q.empty() == false) {
		int now = q.front();
		q.pop();
		maxn = numeric_limits<int>::max();
		select(now, -1, _size[now]);
		vis[root] = 1;
		for(edge*i = g[root];i != NULL;i = i->nxt)
			if(vis[i->v] == false) q.push(i->v);
		res += get_sub_solve(root, k);
	}
	printf("%d\n", res);
}