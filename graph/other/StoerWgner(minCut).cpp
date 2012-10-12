const int V = 501, INF = 100000000, S = 1;
int maps[V][V], dist[V], pre;
bool vst[V], del[V];
void intialize()/* start with 1 */ {
    memset(del, false, sizeof (del));
    memset(maps, 0, sizeof (maps));
}
int maxinum_adjacency_search(int t, int n) {
    for (int i = 1; i <= n; i++)
        if (!del[i]) dist[i] = maps[S][i];
    memset(vst, false, sizeof (vst));
    vst[S] = true;
    int k = S;
    for (int j = 1; j <= n - t; j++) {
        int tmp = -INF;
        pre = k;
        for (int i = 1; i <= n; i++)
            if (!vst[i] && !del[i] && tmp < dist[i]) {
                tmp = dist[i];
                k = i;
            }
        vst[k] = true;
        for (int i = 1; i <= n; i++)
            if (!vst[i] && !del[i]) dist[i] += maps[k][i];
    }
    return k;
}
int Stoer_Wgner(int n) {
    int mcut = INF;
    for (int i = 1; i < n; i++) {
        int idx = maxinum_adjacency_search(i, n);
        mcut = min(mcut, dist[idx]);
        del[idx] = true;
        for (int i = 1; i <= n; i++) {
            if (!del[i] && i != pre) {
                maps[pre][i] += maps[idx][i];
                maps[i][pre] = maps[pre][i];
            }
        }
    }
    return mcut;
}
