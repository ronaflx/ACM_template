/*HOJ2360
* idx is new node of the tree
* pool should be big enough */
const  int SIZE = 5000, ROOT = 0, E = 80000;
struct edge {
    int v, id;
    edge *nxt;
} pool[E], *g[SIZE], *pp, *bg[SIZE];
stack<int> st;
bool flag[E];//label the edge in case of multi-edge
int depth, ebcc, dfn[SIZE], low[SIZE], idx[SIZE];
void initialize() {
    memset(g, 0, sizeof(g));
    memset(flag, 0, sizeof(flag));
    memset(bg, 0, sizeof(bg));
    memset(dfn, 0, sizeof(dfn));
    pp = pool, depth = 1, ebcc = 0;
}
void addedge(int v, int w, edge *g[], int id = 0) {
    pp->v = w, pp->nxt = g[v];
    pp->id = id, g[v] = pp++;
}
void dfs(int v) {
    st.push(v);
    dfn[v] = low[v] = depth++;
    int w, x;
    for (edge* i = g[v]; i != NULL; i = i->nxt) {
        w = i->v;
        if (flag[i->id]) continue;
        flag[i->id] = true;
        if (dfn[w]) low[v] = min(low[v], dfn[w]);
        else {
            dfs(w);
            low[v] = min(low[v], low[w]);
            if (low[w] > dfn[v]) {
                ebcc++;
                do {
                    x = st.top();
                    st.pop();
                    idx[x] = ebcc;
                }while (x != w);
            }
        }
    }
}
void solve()/*find out the cut and build  the tree*/ {
    dfs(ROOT);//ROOT = 0 as usual
    if (!st.empty()) ebcc++;
    while (!st.empty()) {
        idx[st.top()] = ebcc;
        st.pop();
    }
}
