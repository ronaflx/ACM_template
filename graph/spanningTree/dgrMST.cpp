/* 度限制最小生成树 0为根 限制度k最大 */
const int N = 25, LEN = 15, INF = 1<<29;
int dis[N][N]= {}, f[N]= {}, father[N]= {}, n;
bool visit[N]= {};
bool used[N][N]= {};
void Dfs(int last, int v)/*node 0 is root*/ {
    visit[v] = 1;
    if (!father[v]) f[v] = -INF;
    else f[v] = max(dis[last][v], f[father[v]]);
    for (int i = 0; i < n; ++i)
        if (!visit[i] && used[v][i])
            father[i] = v, Dfs(v, i);
}
int DegreeLimitMST(int k) {
    int ret = 0, path[N], group[N]= {}, g = 0, pre[N], degree = 0;
    memset(used, 0, sizeof(used));
    for (int i = 1; i < n; ++i)//除了0点的最小生成森林
        if (!group[i]) {
            group[i] = ++g;
            for (int j = 0; j < n; ++j)
                path[j] = dis[i][j], pre[j] = i;
            while (1) {
                int tmp = INF, mark = -1;
                for (int j = 1; j < n; ++j)
                    if (!group[j] && path[j] < tmp)
                        tmp = path[j], mark = j;
                if (mark == -1) break;
                used[pre[mark]][mark] = 1, used[mark][pre[mark]] = 1;
                ret += tmp;
                group[mark] = g;
                for (int j = 1; j < n; ++j)
                    if (!group[j] && path[j] > dis[mark][j])
                        path[j] = dis[mark][j], pre[j] = mark;
            }
        }
    for (int i = 1; i <= g; ++i)/*和0点相连*/ {
        int tmp = INF, mark = -1;
        for (int j = 1; j < n; ++j)
            if (group[j] == i && tmp > dis[0][j])
                tmp = dis[0][j], mark = j;
        used[0][mark] = used[mark][0] = 1;
        ret += tmp;
        ++degree;
    }
    while (degree < k)/*保证有解,不可能森林大于k个，通过增大度减少树的边权*/ {
        memset(visit, 0, sizeof(visit));
        Dfs(0, 0);
        int tmp = INF, mark = -1, t;
        for (int i = 1; i < n; ++i)
            if (!used[0][i] && dis[0][i] != INF) {
                t = ret+dis[0][i]-f[i];
                if (tmp > t) tmp = t, mark = i;
            }
        if (ret <= tmp) break;
        ret = tmp;
        used[0][mark] = used[mark][0] = 1;
        tmp = f[mark];
        while (dis[father[mark]][mark] != tmp) mark = father[mark];
        used[mark][father[mark]] = used[father[mark]][mark] = 0;
        ++degree;
    }
    return ret;
}
