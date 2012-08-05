typedef long long LL;
const int N = 14;
const int TOT = 50000;
const int MAXN = 1594323;// 3^13
char maps[N][N];
int bit3[N] = {1}, status[TOT];
int Hash[MAXN], allS = 0;
LL dp[2][TOT];
bool check(int s)
{
    int cnt = 0;
    while(s)
    {
        int n = s % 3;
        if(n == 1) cnt++;
        if(n == 2) cnt--;
        if(cnt < 0) return false;
        s /= 3;
    }
    return (cnt == 0);
}
void preprocess()
{
    REP(i, 1, N) bit3[i] = bit3[i - 1] * 3;
    REP(i, 0, bit3[N - 1])
    {
        if(check(i))
        {
            Hash[i] = allS;
            status[allS++] = i;
        }
        else Hash[i] = -1;
    }
    status[allS] = MAXN;
}
int getbit(int s, int i)
{
    while(i-- > 0) s /= 3;
    return s % 3;
}
void transfer(LL& dest, LL add)
{
    dest == -1 ? (dest = add) : (dest += add);
}
LL DP(int n, int m, int px, int py)
{
    LL ans = 0;
    int now = 0, pre;
    CC(dp, -1);
    dp[0][0] = 1;
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < m; j++)
        {
            pre = now;now ^= 1;
            CC(dp[now], -1);
            for(int k = 0, s; s = status[k], s < bit3[m + 1]; k++)
            {
                if(dp[pre][k] == -1) continue;
                int l = getbit(s, j), u = getbit(s, j + 1);
                int nows = s - l * bit3[j] - u * bit3[j + 1];
                if(maps[i][j] == '*')
                {
                    if(l == 0 && u == 0)
                        transfer(dp[now][k], dp[pre][k]);
                }
                else if(l == 0 && u == 0)//both down and right build 2 plugin
                {
                    if(maps[i][j + 1] == '.' && maps[i + 1][j] == '.')
                    {
                        int nxt = nows + bit3[j] + 2 * bit3[j + 1];
                        transfer(dp[now][Hash[nxt]], dp[pre][k]);
                    }
                }
                else if(l == 1 && u == 1)// merge (( make )) to ()
                {
                    int cnt = 0;
                    for(int b = j + 2; b <= m; b++)
                    {
                        int tmp = getbit(nows, b);
                        if(tmp == 2) cnt--;
                        if(tmp == 1) cnt++;
                        if(cnt == -1)
                        {
                            transfer(dp[now][Hash[nows - bit3[b]]], dp[pre][k]);
                            if(Hash[nows - bit3[b]] == -1)
                                cout << nows - bit3[b] << endl;
                            break;
                        }
                    }
                }
                else if(l == 2 && u == 2)// merge )) make (( to ()
                {
                    int cnt = 0;
                    for(int b = j - 1;b >= 0;b--)
                    {
                        int tmp = getbit(nows, b);
                        if(tmp == 1) cnt++;
                        if(tmp == 2) cnt--;
                        if(cnt == 1)
                        {
                            transfer(dp[now][Hash[nows + bit3[b]]], dp[pre][k]);
                            if(Hash[nows + bit3[b]] == -1)
                                cout << nows + bit3[b] << endl;
                            break;
                        }
                    }
                }
                else if(l == 1 && u == 2)//merge () at last grid
                {
                    if(px == i && py == j)
                        ans += dp[pre][k];
                }
                else if(l == 2 && u == 1)//merge )(
                {
                    transfer(dp[now][Hash[nows]], dp[pre][k]);
                }
                else if((!l && u) || (l && !u))
                {
                    if(maps[i + 1][j] == '.')
                        transfer(dp[now][Hash[nows + (l + u) * bit3[j]]], dp[pre][k]);
                    if(maps[i][j + 1] == '.')
                        transfer(dp[now][Hash[nows + (l + u) * bit3[j + 1]]], dp[pre][k]);
                }
            }
        }
        pre = now;now ^= 1;
        CC(dp[now], -1);//must CC -1
        for(int k = 0, s; s = status[k], s < bit3[m]; k++)
            if(dp[pre][k] != -1)
                dp[now][Hash[s * 3]] = dp[pre][k];
    }
    return ans;
}
int main()
{
    int n, m, px, py;
    preprocess();
    while(scanf("%d %d", &n, &m) == 2)
    {
        CC(maps, 0);
        REP(i, 0, n) scanf("%s", maps[i]);
        REP(i, 0, n) REP(j, 0, m) if(maps[i][j] == '.') px = i, py = j;
        printf("%lld\n", DP(n, m, px, py));
    }
    return 0;
}

