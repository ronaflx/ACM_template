const int N = 1010, E = N * N;
const LL INF = 10000000000LL;
template<typename T>
struct Edge {
    int u, v;
    T c;
};
Edge<LL> edge[E];
int label[N], pre[N], visit[N];
template<typename T>
T treeGraph(int n, int m, int root, Edge<T>* edge) {
    int cnt = 0;
    T inEdge[N], ans = 0;
    while(true) {
        fill(inEdge, inEdge + n, INF);
        REP(i, 0, m) {
            int u = edge[i].u, v = edge[i].v;
            if(v != u && edge[i].c < inEdge[v])
            {
                pre[v] = u;
                inEdge[v] = edge[i].c;
            }
        }
        REP(i, 0, n) {
            if(i == root) continue;
            if(inEdge[i] == INF) return -1;
        }
        int now = 0;
        CC(label, -1), CC(visit, -1);
        inEdge[root] = 0;
        REP(i, 0, n) {
            ans += inEdge[i];
            int v = i;
            while(visit[v] != i && label[v] == -1 && v != root) {
                visit[v] = i;
                v = pre[v];
            }
            if(v != root && label[v] == -1) {
                for(int u = pre[v]; u != v; u = pre[u])
                    label[u] = now;
                label[v] = now++;
            }
        }
        if(now == 0) break;
        REP(i, 0, n) if(label[i] == -1) label[i] = now++;
        REP(i, 0, m) {
            int v = edge[i].v;
            edge[i].v = label[edge[i].v];
            edge[i].u = label[edge[i].u];
            if(edge[i].v != edge[i].u) edge[i].c -= inEdge[v];
        }
        root = label[root];
        n = now;
    }
    return ans;
}
 
