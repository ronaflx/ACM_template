const int ROW = 56;
const int COL = 56;
const int N = ROW * COL, HEAD = 0;
const int INF = 1000000000;
int maps[ROW][COL], ansq[ROW], row[N];
int s[COL], u[N], d[N], l[N], r[N], c[N];
void build(int n, int m)
{
    r[HEAD] = 1;
    l[HEAD] = m;
    for (int i = 1; i <= m; i++)
    {
        l[i] = i - 1;
        r[i] = (i + 1) % (m + 1);
        c[i] = d[i] = u[i] = i;
        s[i] = 0;
    }
    int size = m;
    for (int i = 1; i <= n; i++)
    {
        int first = 0;
        for (int j = 1; j <= m; j++)
        {
            if (!maps[i - 1][j - 1]) continue;
            size++;
            d[u[j]] = size;
            u[size] = u[j];
            d[size] = j;
            u[j] = size;
            if (!first)
            {
                first = size;
                l[size] = size;
                r[size] = size;
            }
            else
            {
                l[size] = l[first];
                r[size] = first;
                r[l[first]] = size;
                l[first] = size;
            }
            c[size] = j;
            // row[size]=i;
            s[j]++;
        }
    }
}
inline void coverc(int col)
{
    for(int i = d[col]; i != col; i = d[i])
    {
        r[l[i]] = r[i];
        l[r[i]] = l[i];
    }
}
inline void resumec(int col)
{
    for(int i = u[col]; i != col; i = u[i])
    {
        l[r[i]] = i;
        r[l[i]] = i;
    }
}
bool vis[COL];
int H()
{
    int cnt = 0;
    memset(vis,0,sizeof(vis));
    for (int i = r[HEAD]; i != HEAD; i = r[i])
    {
        if (vis[i]) continue;
        cnt++;
        vis[i] = 1;
        for (int j = d[i]; j != i; j = d[j])
            for (int k = r[j]; k != j; k = r[k])
                vis[c[k]] = 1;
    }
    return cnt;
}
int cut,nextcut;
bool dfs(int dep)
{
    if (!r[HEAD]) return true;
    int now, minn = ROW;
    for (int i = r[HEAD]; i != HEAD; i = r[i])
        if (minn > s[i])
        {
            minn = s[i];
            now = i;
        }
    for (int j = d[now]; j != now; j = d[j])
    {
        //ansq[dep]=row[rp];
        coverc(j);
        for (int i = r[j]; i != j; i = r[i])
            coverc(i);

        int tmp = dep + 1 + H();
        if(tmp > cut)
            nextcut = min(tmp, nextcut);
        else if (dfs(dep + 1))
            return true;
        for (int i = l[j]; i != j; i = l[i])
            resumec(i);
        resumec(j);
    }
    return false;
}
int IDAstar(int n)
{
    cut = H();
    nextcut = n;
    memset(vis,0,sizeof(vis));
    while(!dfs(HEAD))
    {
        cut = nextcut;
        nextcut = n;
    }
    return cut;
}

