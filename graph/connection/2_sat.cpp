#include <iostream>
using namespace std;
/* 2-sat template node is from 0
 * i and i^1 is a bool variable(true or false)
 * conjunctive normal form with 2-sat
 * x V y == 1 => edge(~x-->y) and  edge(~y-->x)
 * x V y == 0 => (~x V ~x) & (~y V ~y)
 * x ^ y == (~x V ~y) & (x V y)
 * x & y == 1 (x V x) & (y V y)
 * x & y == 0  (~x V ~y) */
const int V = 20000, E = 20480 * 4;
const int RED = 1, BLUE = 2;
struct edge
{
    int v;
    edge * nxt;
} pool[E], *g[V], *pp, *gscc[V];
int st[V], top, tms[V], pt;
bool reach[V];
int dfn[V], low[V], idx[V], sccCnt, depth;
int color[V], pre[V];
void addedge(int a, int b, edge *g[])
{
    pp->v = b;
    pp->nxt = g[a];
    g[a] = pp++;
}
void initialize()
{
    memset(reach, 0, sizeof (reach));
    memset(dfn, 0, sizeof (dfn));
    memset(g, 0, sizeof (g));
    top = sccCnt = depth = 0, pp = pool;
}
void dfs(int x)
{
    st[++top] = x;
    dfn[x] = low[x] = ++depth;
    int w;
    for (edge * i = g[x]; i != NULL; i = i->nxt)
    {
        w = i->v;
        if (reach[w]) continue;
        else if (dfn[w] == 0)
        {
            dfs(w);
            low[x] = min(low[x], low[w]);
        }
        else low[x] = min(low[x], dfn[w]);
    }
    if (low[x] == dfn[x])
    {
        sccCnt++;
        do
        {
            w = st[top--];
            idx[w] = sccCnt - 1;
            reach[w] = true;
        }while (w != x);
    }
}
void toposort(int v)
{
    reach[v] = true;
    for (edge *i = gscc[v]; i != NULL; i = i->nxt)
        if (!reach[i->v]) toposort(i->v);
    tms[pt++] = v;
}
void build_regraph(int n)//anti-graph
{
    memset(gscc, 0, sizeof (gscc));//anti-graph scc
    memset(pre, -1, sizeof (pre));//the new node to every scc
    for (int i = 0; i < n; i++)
    {
        if (pre[idx[i]] == -1)
            pre[idx[i]] = i;
        for (edge * ptr = g[i]; ptr != NULL; ptr = ptr->nxt)
        {
            int w = ptr->v;
            if (idx[i] != idx[w]) addedge(idx[w], idx[i], gscc);
        }
    }
}
void becolor(int v)
{
    color[v] = BLUE;
    for (edge *i = gscc[v]; i != NULL; i = i->nxt)
        if (!color[i->v]) becolor(i->v);
}
void output(int n)//Topological Sort
{
    memset(color, 0, sizeof (color));//color white
    for (int i = 0; i < pt; i++)
    {
        if (!color[tms[i]])//color as Topological order
        {
            color[tms[i]] = RED;
            int v = idx[pre[tms[i]] ^ 1];
            if (color[v] == 0)
                becolor(v);
        }
    }
    for (int i = 0; i < n; i += 2)
    {
        if (color[idx[i]] == RED)
            printf("%d\n", i + 1);
        else //if (color[idx[i ^ 1]] == RED)
            printf("%d\n", (i ^ 1) + 1);
    }
}
bool solve(int n)//i and ~i can not be in the same scc
{
    for (int i = 0; i < n; i++) if (!reach[i]) dfs(i);
    for (int i = 0; i < n; i++)
        if (idx[i] == idx[i ^ 1])return false;
    build_regraph(n);
    pt = 0;
    memset(reach, 0, sizeof (reach));
    for (int i = 0; i < sccCnt; i++)
        if (!reach[i]) toposort(i);
    reverse(tms, tms + pt);
    output(n);
    return true;
}
int main()
{
    int n, m;
    while (scanf("%d %d", &n, &m) == 2)
    {
        initialize();
        n *= 2;
        while (m--)
        {
            int a, b;
            scanf("%d %d", &a, &b);
            a--, b--;
            addedge(a, b ^ 1, g);
            addedge(b, a ^ 1, g);
        }
        if (!solve(n)) printf("NIE\n");
    }
    return 0;
}

