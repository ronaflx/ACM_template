const int SIZE = 10000;
int color[SIZE], interval[SIZE * 2 + 1], col;
int first[SIZE], depth[SIZE], uncolor[SIZE];
//color records the color of every node
vector<int> adj[SIZE];bool check[SIZE];
int n, pt, q, tim, dp[SIZE * 2 + 1][15];
void dfs(int x, int d) {
    check[x] = true;
    interval[pt++] = col;
    first[col] = pt - 1;
    color[x] = col;
    uncolor[col++] = x;
    depth[x] = d;
    int s = adj[x].size(), w;
    for (int i = 0; i < s; i++) {
        w = adj[x][i];
        if (!check[w]) {
            dfs(w, d + 1);
            interval[pt++] = color[x];
        }
    }
}
void initialize() {
    scanf("%d %d", &n, &q);
    int a, b;
    for (int i = 0; i < n; i++)
        adj[i].clear();
    for (int i = 1; i < n; i++) {
        scanf("%d %d", &a, &b);
        adj[a].push_back(b);
        adj[b].push_back(a);
    }
    tim = pt = col = 0;
    memset(check, false, sizeof (check));
}
int main() {
    int t;
    scanf("%d", &t);
    while (t--) {
        initialize();
        dfs(0, 1);
        for (int i = 0; i < pt; i++)
            dp[i + 1][0] = interval[i];
        for (int j = 1; j <= log(pt) / log(2); j++) {
            for (int i = 1; i + (1 << j) - 1 <= pt; i++) {
                dp[i][j] = min(dp[i][j - 1], dp[i + (1 << (j - 1))][j - 1]);
            }
        }
        while (q--) {
            int a, b, len, ans;
            scanf("%d %d", &a, &b);
            int tempa = a,tempb = b;
            a = first[color[a]] + 1;
            b = first[color[b]] + 1;
            if (a > b) {
                int temp = a;
                a = b;
                b = temp;
            }
            len = b - a + 1;
            len = log(len) / log(2) + 0.00001;
            ans = min(dp[a][len], dp[b - (1 << len) + 1][len]);
            printf("%d\n",depth[tempa] + depth[tempb] - 2 * depth[uncolor[ans]]);
        }
    }
    return 0;
}
