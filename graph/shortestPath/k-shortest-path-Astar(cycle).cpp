/* 估价函数f(x)=g(x)+h(x);h(x)=h*(x);所以符合A*条件,k短路含环
 * status中f为A*的估价函数。f(x2)=f(x1)-h(x1)+h(x2)+c(x1)(x2);*/
const int V = 1000, E = 100000, INF = 100000000;
struct status
{
    int v, f;
    status() {}
    status(int _v, int _f)
    {v = _v, f = _f;}
    bool operator <(const status a)const
    {return f > a.f;}
};

struct edge
{
    int v, dist;
    edge *nxt;
}*pp, *g[V], *rg[V], pool[E * 2];
int d[V], c[V];
bool chk[V];
void initialize()
{
    pp = pool;
    memset(g, 0, sizeof(g));
    memset(rg, 0, sizeof(rg));
}

void addedge(int u, int v,int dist, edge *g[])
{
    pp->v = v;
    pp->dist = dist;
    pp->nxt = g[u];
    g[u] = pp++;
}

void dijkstra(int n, int t)
{
    fill(d, d + n, INF);
    memset(chk, false, sizeof(chk));
    priority_queue<status> pq;
    pq.push(status(t, 0));
    while(!pq.empty())
    {
        status now = pq.top();
        pq.pop();
        if(chk[now.v]) continue;
        chk[now.v] = true;
        d[now.v] = now.f;
        for(edge *i = rg[now.v]; i != NULL; i = i->nxt)
            pq.push(status(i->v, now.f + i->dist));
    }
}

int Astar(int s, int t, int k)
{
    if(d[s] == INF) return -1;
    memset(c, 0, sizeof(c));
    priority_queue<status> pq;
    pq.push(status(s, d[s]));
    while(!pq.empty())
    {
        status now = pq.top();
        pq.pop();
        c[now.v]++;
        if(c[t] == k) return now.f;
        if(c[now.v] > k) continue;
        for(edge *i = g[now.v]; i!= NULL; i = i->nxt)
            pq.push(status(i->v, now.f - d[now.v] + d[i->v] + i->dist));
    }
    return -1;
}

int main()
{
    int n, m, s, t, k;
    int u, v, dist;
    while(scanf("%d %d", &n, &m) == 2)
    {
        initialize();
        for(int i = 0; i < m; i++)
        {
            scanf("%d %d %d", &u, &v, &dist);
            u--, v--;
            addedge(u, v, dist, g);
            addedge(v, u, dist, rg);
        }
        scanf("%d %d %d", &s, &t, &k);
        s--, t--;
        if(s == t) k++;
        dijkstra(n, t);
        printf("%d\n", Astar(s, t, k));
    }
    return 0;
}

