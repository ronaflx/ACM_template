const int V = 1010, E = V*V*2, INF = 1<<29;
typedef struct Edge{
	int v, cap, flow;
	Edge *next, *re;
}Edge;
class MaxFlow{
public:
	Edge edge[E], *adj[V], *pre[V], *arc[V];
	int e, n, d[V], q[V], numb[V];
	void Init(int x){
		n = x;
		for (int i = 0; i < n; ++i) adj[i] = NULL;
		e = 0;
	}
	void Addedge(int x, int y, int f) {
		edge[e].v = y, edge[e].cap = f, edge[e].next = adj[x], edge[e].re = &edge[e+1]; adj[x] = &edge[e++];
		edge[e].v = x, edge[e].cap = 0, edge[e].next = adj[y], edge[e].re = &edge[e-1]; adj[y] = &edge[e++];
	}
	void Bfs(int v) {
		int front = 0, rear = 0, r = 0, dis = 0;
		for (int i = 0;i < n; ++i) d[i] = n, numb[i] = 0;
		d[v] = 0;++numb[0];
		q[rear++] = v;
		while (front != rear) {
			if (front == r) ++dis, r = rear;
			v = q[front++];
			for (Edge *i = adj[v];i != NULL;i = i->next) {
				int t = i->v;
				if (d[t] == n) d[t] = dis, q[rear++] = t, ++numb[dis];
			}
		}
	}
	int Maxflow(int s, int t){
		int ret = 0, i, j;
		Bfs(t);
		for (i = 0; i < n; ++i) pre[i] = NULL, arc[i] = adj[i];
		for (i = 0; i < e; ++i) edge[i].flow = edge[i].cap;
		i = s;
		while (d[s] < n) {
			while (arc[i] && (d[i] != d[arc[i]->v]+1 || !arc[i]->flow)) arc[i] = arc[i]->next;
			if (arc[i]) {
				j = arc[i]->v;
				pre[j] = arc[i];
				i = j;
				if (i == t) {
					int update = INF;
					for (Edge *p = pre[t];p != NULL;p = pre[p->re->v]) checkmin(update, p->flow);
					ret += update;
					for (Edge *p = pre[t];p != NULL;p = pre[p->re->v]) p->flow -= update, p->re->flow += update;
					i = s;
				}
			}
			else {
				int min = n - 1;
				for (Edge *p = adj[i];p != NULL;p = p->next) if(p->flow) checkmin(min, d[p->v]);
				if (--numb[d[i]] == 0) return ret;
				d[i] = min + 1;
				++numb[d[i]];
				arc[i] = adj[i];
				if (i != s) i = pre[i]->re->v;
			}
		}
		return ret;
	}
};
