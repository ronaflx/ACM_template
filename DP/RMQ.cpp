/*RMQ-ST be careful with log2*/
const int SIZE = 500001;
int dp[SIZE][20];
int n, q, a, b, len, ans;
int main()
{
    while (scanf("%d %d", &n, &q) == 2)
    {
        for (int i = 1; i <= n; i++)
            scanf("%d", &dp[i][0]);
        for (int j = 1; j <= log(n) / log(2); j++)
            for (int i = 1; i + (1 << (j - 1)) <= n; i++)
                dp[i][j] = max(dp[i][j - 1], dp[i + (1 << (j - 1))][j - 1]);
        while (q--)
        {
            scanf("%d %d", &a, &b);
            len = log(b - a + 1) / log(2) + 0.001;
            ans = max(dp[a][len], dp[b - (1 << len) + 1][len]);
            printf("%d\n", ans);
        }
    }
    return 0;
}
