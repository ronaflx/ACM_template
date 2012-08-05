const int SIZE = 1001;
const int INF = 1000000000;
double g[SIZE][SIZE], x[SIZE], y[SIZE];
bool visit[SIZE], circle[SIZE];
int pre[SIZE];
double dist(int i, int j)
{
    return sqrt(pow((x[i]-x[j]),2)+pow((y[i]-y[j]),2));
}
void dfs(int t, int n)
{
    if (visit[t]) return;
    visit[t] = 1;
    REP(i, 0, n) if (g[t][i] < INF) dfs(i, n);
}
bool connect(int root, int n)
{//judge contection
    CC(visit, 0);dfs(root, n);
    return accumulate(visit, visit + n, 0) == n;
}
void findMinimumEdge(const int root, int n)
{//find min edge of every edge
    REP(i, 0, n)
    {
        if (circle[i] || i == root) continue;
        pre[i] = i;
        double tmp = INF + 1;
        REP(j, 0, n)
        {
            if (circle[j]) continue;
            if (g[j][i] < tmp && i != j)
            {
                tmp = g[j][i];
                pre[i] = j;
            }
        }
    }
}
int findCrile(const int root, int n)
{
    REP(i, 0, n)
    {
        if(circle[i]) continue;
        int now = i;
        CC(visit, 0);
        while(!visit[now] && now != root)
        {
            visit[now] = true;
            now = pre[now];
        }
        if(now != root) return now;
    }
    return -1;
}
void update(int now, int n)
{
    REP(j, 0, n)
    {
        if (circle[j]) continue;
        if (g[j][now] < INF)
            g[j][now] -= g[pre[now]][now]; //update inEdge
    }
    for (int j = pre[now]; j != now; j = pre[j])
    {
        REP(k, 0, n)
        {
            if (circle[k]) continue;
            if (g[j][k] < INF)//update outEdge
                g[now][k] = min(g[now][k], g[j][k]);
            if (g[k][j] < INF)//update inEdge
                g[k][now] = min(g[k][now], g[k][j] - g[pre[j]][j]);
        }
    }
}
double solve(const int root, int n)
{
    double ans = 0;
    int now;
    memset(circle, 0, sizeof (circle));
    do
    {
        findMinimumEdge(root, n);
        if((now = findCrile(root, n)) != -1)
        {
            ans += g[pre[now]][now];
            for (int j = pre[now]; j != now; j = pre[j])
            {
                ans += g[pre[j]][j];
                circle[j] = 1;
            }
            update(now, n);
        }
        else
        {
            REP(j, 0, n)
            {
                if (circle[j] || j == root) continue;
                ans += g[pre[j]][j];
            }
        }
    }while(now != -1);
    return ans;
}
int main()
{
    int n, m, a, b;
    while (scanf("%d%d", &n, &m) != EOF)
    {
        REP(i, 0, n) scanf("%lf%lf", &x[i], &y[i]);
        REP(i, 0, n) REP(j, 0, n) g[i][j] = INF;
        REP(i, 0, m)
        {
            scanf("%d%d", &a, &b);
            a--, b--;
            g[a][b] = dist(a, b);
        }
        if (!connect(0, n)) printf("poor snoopy\n");
        else printf("%.2lf\n", solve(0, n));
    }
    return 0;
}

