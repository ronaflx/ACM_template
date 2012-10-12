/* make sure that the sum is not lower than k*/
int findK(int K) {
    int ans = 0, cnt = 0;
    for (int i = log(MAXN - 1) / log(2); i >= 0; i--) {
        ans += (1 << i);
        if (ans >= MAXN || cnt + c[ans] >= K)
            ans -= (1 << i);
        else
            cnt += c[ans];
    }
    return ans + 1;
}
