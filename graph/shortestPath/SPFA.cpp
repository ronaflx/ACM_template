/* SPFA模板，队列实现
 * 如果要求负环，设置人工顶点
 * 与已知点连边权值为0
 * 人工顶点先入队列
 * 记录每个点入队列的次数，如果大于n则有负环
 */
const int V = 1010, E = 300001, INF = 10000000;
struct edges
{
	int v, val;
	edges *next;
} pool[E], *g[V], *pp;
int cnt[V], dist[V];
bool vst[V];
void initialize()
{
	memset(g, 0, sizeof (g));
	pp = pool;
}
void addedge(int a, int b, int v)
{
	pp->v = b;
	pp->val = v;
	pp->next = g[a];
	g[a] = pp++;
}
void SPFA(int n)
{
	memset(cnt, 0, sizeof (cnt));
	memset(vst, false, sizeof (vst));
	fill(dist, dist + n, INF);
	queue<int> q;
	q.push(n - 1);
	dist[n - 1] = 0;
	vst[n - 1] = true;
	while (!q.empty())
	{
		int beg = q.front();
		q.pop();
		vst[beg] = false;
		for (edges * i = g[beg]; i != NULL; i = i->next)
		{
			int tmp = i->v, val = i->val;
			if (dist[tmp] > dist[beg] + val)
			{
				dist[tmp] = dist[beg] + val;
				if (!vst[tmp])
				{
					vst[tmp] = true;
					cnt[tmp]++;
					q.push(tmp);
					if (cnt[tmp] > n)
					{
						puts("-1");
						return;
					}
				}
			}
		}
	}
	for(int i = 1;i < n - 1;i++)
		printf("%d", dist[i] - dist[i - 1]);
	puts("");
}
int main()
{
	int n ,a0, b0, l0, a1, b1, l1;
	while(scanf("%d %d %d %d %d %d %d", &n, &a0, &b0, &l0, &a1, &b1, &l1) == 7)
	{
		initialize();
		for(int i = 1;i  + l0 - 1 <= n;i++)
		{
			addedge(i - 1, i + l0 - 1, l0 - a0);
			addedge(i + l0 - 1, i - 1, b0 - l0);
		}
		for(int i = 1;i + l1 - 1 <= n;i++)
		{
			addedge(i - 1, i + l1 - 1, b1);
			addedge(i + l1 - 1, i - 1, -a1);
		}
		for(int i = 1;i <= n;i++)
		{
			addedge(i - 1, i, 1);
			addedge(i, i - 1, 0);
		}
		for(int i = 0;i <= n;i++)
			addedge(n + 1, i, 0);
		SPFA(n + 2);
	}
	return 0;
}
