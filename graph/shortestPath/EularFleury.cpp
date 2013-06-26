const int SIZE = 2 * 2000, N = 50;
/* Eular degree & connection
 * fordown path，is the smallest lexicographic path
 * hoj 1045 John's trip*/
struct edge {
    int v, id;
    bool operator<(const edge a) const {
        return id < a.id;
    }
} edges[SIZE];
vector<edge> adj[N];
int path[SIZE]. E, V, S, deg[N], stp;
bool vst[SIZE];
void dfs(int now) {
    edge tmp;
    for (size_t i = 0; i < adj[now].size(); i++) {
        tmp = adj[now][i];
        if (!vst[tmp.id] && !vst[tmp.id]) {
            vst[tmp.id] = vst[tmp.id] = 1;
            dfs(tmp.v);
            path[stp++] = tmp.id;
        }
    }
}
void solve() { {
    for (int i = 0; i < V; i++)
        sort(adj[i].begin(), adj[i].end());
    dfs(S);
    printf("%d", path[stp - 1]);
    for (int i = stp - 2; i >= 0; i--)
        printf(" %d", path[i]);
    putchar('\n');
}
void initialize(int u, int v) {
    stp = V = E = S = 0;
    for (int i = 0; i < N; i++) adj[i].clear();
    memset(vst, false, sizeof (vst));
    memset(deg, 0, sizeof (deg));
    S = min(v, u);
}
void add_edge(int u, int v, int id, int E) {
    deg[u]++, deg[v]++;
    edges[E].v = v, edges[E].id = id;
    adj[u].push_back(edges[E]);
    edges[E].v = u, edges[E].id = id;
    adj[v].push_back(edges[E]);
}