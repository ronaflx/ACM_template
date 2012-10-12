/* tarjan-scc, new graph is a dag from 0 to sccCnt - 1
tms is the topo-order*/
const int V = 50001, E = 150000;
struct edge/*graph*/ {
    int v;
    edge *nxt;
} pool[E * 3], *g[V], *pp, *gscc[V];
int st[V], top, tms[V], pt;//toposort
bool reach[V];//reach is used to label is reached or not
int dfn[V], low[V], idx[V], sccCnt, depth;
void addedge(int u, int v, edge* g[]) {
    pp->v = v;
    pp->nxt = g[u];
    g[u] = pp++;
}
void initialize(int n) {
    memset(g, 0, sizeof (g));
    memset(reach, false, sizeof (reach));
    memset(dfn, 0, sizeof (dfn));
    pp = pool, depth = pt = top = sccCnt = 0;
}
void dfs(int x) {
    st[++top] = x;
    dfn[x] = low[x] = ++depth;
    int w;
    for (edge *i = g[x]; i != NULL; i = i->nxt) {
        w = i->v;
        if (reach[w]) continue;
        else if (dfn[w] == 0) {
            dfs(w);
            low[x] = min(low[x], low[w]);
        }
        else low[x] = min(low[x], dfn[w]);
    }
    if (low[x] == dfn[x]) {
        sccCnt++;
        do {
            w = st[top--];
            idx[w] = sccCnt - 1;
            reach[w] = true;
        }while (w != x);
    }
}
void toposort(int x) {
    reach[x] = true;
    for (edge *i = gscc[x]; i != NULL; i = i->nxt)
        if (!reach[i->v]) toposort(i->v);
    tms[pt++] = x;
}
void build_newgraph(int n) {
    memset(gscc, 0, sizeof (gscc));
    for (int i = 0; i < n; i++)
        for (edge *j = g[i]; j != NULL; j = j->nxt)
            if (idx[i] != idx[j->v])addedge(idx[i],idx[j->v], gscc);
}
void solve(int n) {
    for (int i = 0; i < n; i++)
        if (!reach[i]) dfs(i);
    build_newgraph(n);
    memset(reach, false, sizeof (reach));//reuse reach
    for (int i = 0; i < sccCnt; i++)
        if (!reach[i]) toposort(i);
    reverse(tms, tms + pt);//Topological Sort
}
