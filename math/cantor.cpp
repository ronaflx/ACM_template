const int N = 10; // 0! to (n - 1)! && all base on 0
const int fac[N] = {1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880};
void uncantor(int *a, int s, const int n) {
    char p[N] = {0};
    int i, j, t, r;
    for (i = 0; i < n; i++) {
        t = s / fac[n - 1 - i] + 1;
        s %= fac[n - 1 - i];
        r = 0, j = 0;
        while (1) {
            if (p[j] == 0) r++;
            if (r == t) break;
            j++;
        }
        a[i] = j;
        p[j] = 1;
    }
    return;
}
int cantor(const int *a, const int n) {
    int ans = 0, i, j, r;
    char p[N] = {0};
    for (i = 0; i < n; i++) {
        for (j = 0, r = 0; j <= a[i]; j++)
            if (p[j] == 0) r++;
        ans += (r - 1) * fac[n - 1 - i];
        p[a[i]] = 1;
    }
    return ans;
}
