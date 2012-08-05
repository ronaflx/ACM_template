#define OP(i) (((i) - (pool))^1)
class sap
{
private:
    const static int V = 20010, E = 1000000, INF = 100000000;
    int dis[V], numdis[V], pre[V], b[V];
    struct edge
    {
        int v, cap, low;
        edge *nxt;
    } pool[E], *g[V], *pp, *e[V], *pree[V];
    void bfs(int v, int n)
    {
        int que[V], tail = 0;
        bool vst[V] = {0};
        memset(numdis, 0, sizeof(numdis));
        fill(dis, dis + n, n);
        dis[v] = 0, vst[v] = 1, que[0] = v;
        for (int j = 0; j <= tail; j++)
        {
            int tmp = que[j % n];
            for (edge *i = g[tmp]; i != NULL; i = i->nxt)
            {
                if (pool[OP(i)].cap > 0 && !vst[i->v])
                {
                    tail++;
                    vst[i->v] = 1;
                    que[tail % n] = i->v;
                    dis[i->v] = dis[tmp] + 1;
                    numdis[dis[i->v]]++;
                }
            }
        }
    }
    int findArgumentPath(int &v, int s, int t)
    {
        while (e[v] != NULL)
        {
            if (e[v]->cap > 0 && dis[v] == dis[e[v]->v] + 1)
            {
                pre[e[v]->v] = v, pree[e[v]->v] = e[v], v = e[v]->v;
                if (v == t)
                {
                    int minf = INF;
                    for (int i = t; i != s; i = pre[i])
                        minf = min(minf,pree[i]->cap);
                    for (int i = t; i != s; i = pre[i])
                    {
                        pree[i]->cap -= minf;
                        pool[OP(pree[i])].cap += minf;
                    }
                    v = s;
                    return minf;
                }
            }
            else e[v] = e[v]->nxt;
        }
        return 0;
    }
    void createEdge(int i, int j, int cap, int low)
    {
        pp->v = j,      pp->low = low;
        pp->cap = cap, pp->nxt = g[i];
        g[i] = pp++;
    }
public:
    void addedge(int i, int j, int cap, int low)
    {
        createEdge(i, j, cap - low, low);
        createEdge(j, i, 0, low);
        b[j] += low, b[i] -= low;
    }
    int getLimit(int n, int s, int t)
    {
        int tmpans = 0;
        for(int i = 0; i < n; i++)
        {
            if(i == s || i == t) continue;
            if(b[i] > 0) tmpans += b[i];
        }
        return tmpans;
    }
    int maxflowsap(int n, int s, int t)
    {//n points is from 1 to n, src is 0, tar is n + 1
        for(int i = 0; i < n; i++)
        {
            if(i == s || i == t) continue;
            if(b[i] < 0) addedge(i, t, -b[i], 0);
            if(b[i] > 0) addedge(s, i,  b[i], 0);
        }
        bfs(t, n);
        int v = s, maxflow = 0;
        copy(g, g + n, e);
        while (dis[s] < n)
        {
            int add = findArgumentPath(v, s, t);
            maxflow += add;
            if (add == 0)
            {
                int mindis = n;
                numdis[dis[v]]--;
                if (!numdis[dis[v]]) break;
                for (edge *i = g[v]; i != NULL; i = i->nxt)
                    if (i->cap > 0) mindis = min(mindis,dis[i->v] + 1);
                dis[v] = mindis;
                numdis[dis[v]]++;
                e[v] = g[v];
                if (v != s) v = pre[v];
            }
        }
        return maxflow;
    }
    void firststart()
    {
        pp = pool;
        memset(g, 0, sizeof(g));
        memset(b, 0, sizeof(b));
    }
    void getAnswer(int n, int m)
    {
        edge * j;
        int i = 0;
        for(i = 0,j = &pool[1]; i < m; i++, j += 2)
        {
            printf("%d\n", j->low + j->cap);
        }
    }
}flow;
