LL DP(LL a, int k)
{
    char str[N * 2];
    sprintf(str, "%lld", a);
    LL dp[2][2][N][N][N], ans = 0;//section, 0\1, length, mod
    int len = strlen(str);
    memset(dp, 0, sizeof(dp));//deal with prefix 0
    dp[0][1][0][(str[0] - '0') % k][0]++;
    for(int now = 1; now < str[0] - '0'; now++)
        dp[0][0][0][now % k][0]++;
    REP(nxt, 1, len)
    {
        REP(now, 1, 10)//deal with prefix 0
            dp[0][0][nxt][now % k][0]++;
        REP(a, 0, k)
        {
            REP(b, 0, k)
            {
                for(int now = 0; now < 10; now++)
                {
                    if(now == str[nxt] - '0')
                    {
                        dp[0][1][nxt][(a * 10 + now) % k][b]+=dp[0][1][nxt-1][a][b];
                        dp[1][1][nxt][a][(b * 10 + now) % k]+=dp[0][1][nxt-1][a][b];
                        dp[1][1][nxt][a][(b * 10 + now) % k]+=dp[1][1][nxt-1][a][b];
                    }
                    else if(now < str[nxt] - '0')
                    {
                        dp[0][0][nxt][(a * 10 + now) % k][b]+=dp[0][1][nxt-1][a][b];
                        dp[1][0][nxt][a][(b * 10 + now) % k]+=dp[0][1][nxt-1][a][b];
                        dp[1][0][nxt][a][(b * 10 + now) % k]+=dp[1][1][nxt-1][a][b];
                    }
                    dp[0][0][nxt][(a * 10 + now) % k][b] += dp[0][0][nxt - 1][a][b];
                    dp[1][0][nxt][a][(b * 10 + now) % k] += dp[0][0][nxt - 1][a][b];
                    dp[1][0][nxt][a][(b * 10 + now) % k] += dp[1][0][nxt - 1][a][b];
                }
            }
        }
    }
    for(int a = 0;a < k;a++)
        for(int b = 0;b < k;b++)
            if((a + b) % k == 0)
                ans += dp[1][0][len - 1][a][b] + dp[1][1][len - 1][a][b];
    return ans;
}
int main ()
{
    LL a, b;
    int k;
    while (scanf ("%lld%lld%d", &a, &b, &k) == 3)
        printf("%lld\n", DP(b, k) - DP(a - 1, k));
    return 0;
}