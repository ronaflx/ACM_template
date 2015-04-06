const int SIZE = 20002;
const LL INF = 2000000000LL;
long long w[SIZE], d[SIZE];//input data
long long ds[SIZE], dp[SIZE], sw[SIZE], sp[SIZE], bp[SIZE];
int n, q[SIZE], head, tail;
void preprocess() {
    ds[1] = 0;
    for(int i = 2; i <= n + 1; i++)
        ds[i] = ds[i - 1] + d[i - 1];
    sw[0] = sp[0] = 0;
    for(int i = 1; i <= n + 1; i++) {
        sw[i] = sw[i - 1] + w[i];
        sp[i] = sp[i - 1] + sw[i - 1] * d[i - 1];
    }
    bp[n + 1] = 0;
    for(int i = n; i >= 1; i--)
        bp[i] = bp[i + 1] + w[i] * (ds[n + 1] - ds[i]);
}
void DP() {
    fill(dp + 1,dp + n + 1, INF);
    long long ans = INF;
    head = tail = 0;
    q[tail++] = 1;
    ans = min(ans,sp[n + 1] - sw[0] * (ds[1] - ds[0]) - sw[1] * (ds[n + 1] - ds[1]));
    for(int i = 2; i <= n; i++) {
        /*
       * bp[i + 1] = sp[n + 1] - sp[i] - sw[i] * (ds[n + 1] - ds[i]);
       * dp[i] = min(dp[i],bp[i + 1] + sp[j] + sp[i] - sp[j] - sw[j] * (ds[i] - ds[j]));
       * dp[i] = min(dp[i],sp[n + 1] - sw[j] * (ds[i] - ds[j]) - sw[i] * (ds[n + 1] - ds[i]));
         * dp[i]k is the minimum index in 0-k dp[i]j - dp[i]k >= 0 (j < k)
       * (sw[j] * ds[j] - sw[k] * ds[k]) / (sw[j] - sw[k]) <= ds[i] <= ds[i + 1];
         * so i + 1 decision is more than k
       * y = sw[j] * ds[j] x = sw[j];
         */
        while(head + 1 < tail) {
            long long y1 = sw[q[head]] * ds[q[head]],y2 = sw[q[head + 1]] * ds[q[head + 1]];
            long long x1 = sw[q[head]],x2 = sw[q[head + 1]];
            if((y2 - y1) <= ds[i] * (x2 - x1)) head++;
            else break;
        }
        int k = q[head];
        dp[i] = sp[n + 1] - sw[k] * (ds[i] - ds[k]) - sw[i] * (ds[n + 1] - ds[i]);
        ans = min(ans,dp[i]);
        while(head + 1 < tail) {
            long long y1 = sw[q[tail - 2]] * ds[q[tail - 2]];
            long long y2 = sw[q[tail - 1]] * ds[q[tail - 1]],y3 = sw[i] * ds[i];
            long long x1 = sw[q[tail - 2]],x2 = sw[q[tail - 1]],x3 = sw[i];
            if((y2 - y1) * (x3 - x2) >= (y3 - y2) * (x2 - x1)) tail--;
            else break;
        }
        q[tail++] = i;
    }
    printf("%lld\n",ans);
}
int main() {
    while(scanf("%d", &n) == 1) {
        for(int i = 1; i <= n; i++)
            scanf("%lld %lld", &w[i], &d[i]);
        preprocess();
        DP();
    }
    return 0;
}
