char from[201], to[201];
int dp[201][201][3];
int getmin(int start, int end, int state) {
    if (start > end) return 0;
    if (dp[start][end][state] < 100000)
        return dp[start][end][state];
    if ((from[start] == to[start] && state == 0) || state == to[start]-'A' + 1)
        dp[start][end][state] = getmin(start + 1, end, state);
    else {
        for (int i = start; i <= end; ++i)
            dp[start][end][state] = min(dp[start][end][state],
		getmin(start, i, to[start] - 'A' + 1) + getmin(i + 1, end, state) + 1);
    }
    return dp[start][end][state];
}
int main() {
    int test;
    scanf("%d", &test);
    while (test--) {
        scanf("%s %s", from, to);
        int len = strlen(from);
        for (int i = 0; i < len; ++i)
            for (int j = i; j < len; ++j)
                dp[i][j][0] = dp[i][j][1] = dp[i][j][2] = 100000;
        printf("%d\n", getmin(0, len - 1, 0));
    }
    return 0;
}
