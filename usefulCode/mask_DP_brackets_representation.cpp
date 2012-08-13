const int N = 10, TOT = 1000;
const int MAXN = 1594323;// 3^13
char maps[N][N];
int bit3[N] = {1}, status[TOT], Hash[MAXN], allS = 0;
LL dp[2][TOT];
bool check(int s) {
	int cnt = 0;
	while(s) {
		int n = s % 3;
		if(n == 1) cnt++;
		if(n == 2) cnt--;
		if(cnt < 0) return false;
		s /= 3;
	}
	return (cnt == 0);
}
int getbit(int s, int i) {
	return s / bit3[i] % 3;
}
void transfer(LL& dest, LL add) {
	dest == -1 ? (dest = add) : (dest += add);
}
LL DP(int n, int m) {
	int now = 0, pre = 1, state = (1 + 2 * bit3[m - 1]) * 3;
	CC(dp, 0), dp[0][0] = 1;
	for(int i = 0; i < n; i++) {
		for(int j = 0; j < m; j++) {
			swap(now, pre);CC(dp[now], 0);
			for(int k = 0, s; s = status[k], s < bit3[m + 1]; k++) {
				if(dp[pre][k] == 0) continue;
				int l = getbit(s, j), u = getbit(s, j + 1);
				int nows = s - l * bit3[j] - u * bit3[j + 1];
				if(maps[i][j] != '.') {
					if(l == 0 && u == 0) {
						transfer(dp[now][k], dp[pre][k]);
					}
				} else if(l == 0 && u == 0) { //当向下向右都可以走的时候建立两个插头
					if(maps[i][j + 1] == '.' && maps[i + 1][j] == '.') {
						int nxt = nows + bit3[j] + 2 * bit3[j + 1];
						transfer(dp[now][Hash[nxt]], dp[pre][k]);
					}
				} else if(l == 1 && u == 1) { //合并2个左插头，整理相应的右插头
					int cnt = 0;
					for(int b = j + 2; b <= m; b++) {
						int tmp = getbit(nows, b);
						if(tmp == 2) cnt--;
						if(tmp == 1) cnt++;
						if(cnt == -1) {
							transfer(dp[now][Hash[nows - bit3[b]]], dp[pre][k]);
							break;
						}
					}
				} else if(l == 2 && u == 2) { //合并2个右插头，整理相应的左插头
					int cnt = 0;
					for(int b = j - 1;b >= 0;b--) {
						int tmp = getbit(nows, b);
						if(tmp == 1) cnt++;
						if(tmp == 2) cnt--;
						if(cnt == 1) {
							transfer(dp[now][Hash[nows + bit3[b]]], dp[pre][k]);
							break;
						}
					}
				} else if(l == 2 && u == 1) { //合并)(,什么都不用整理
					transfer(dp[now][Hash[nows]], dp[pre][k]);
				} else if((!l && u) || (l && !u)) {
					if(maps[i + 1][j] == '.')
						transfer(dp[now][Hash[nows + (l + u) * bit3[j]]], dp[pre][k]);
					if(maps[i][j + 1] == '.')
						transfer(dp[now][Hash[nows + (l + u) * bit3[j + 1]]], dp[pre][k]);
				} else if(l == 1 && u == 2) {/* 在最后一个格子发生,本题 不需要该步*/}
			}
		}
		swap(pre, now); CC(dp[now], 0);//必须CC一下，把不合法的状态设为-1
		for(int k = 0, s; s = status[k], s < bit3[m]; k++)
			if(dp[pre][k] != -1) dp[now][Hash[s * 3]] = dp[pre][k];
	}
	return max(0LL, dp[now][Hash[state]]);
}
int main() {
	int n, m;
	REP(i, 1, N) bit3[i] = bit3[i - 1] * 3;
	REP(i, 0, bit3[N - 1]) {
		if(check(i)) {
			Hash[i] = allS;
			status[allS++] = i;
		} else {
			Hash[i] = -1;
		}
	}
	status[allS] = MAXN;
	while(scanf("%d %d", &n, &m) == 2 && (n + m)) {
		CC(maps, 0);
		REP(i, 0, n) scanf("%s", maps[i]);
                maps[n][0] = maps[n][m - 1] = '.';
		printf("%I64d\n", DP(n, m));
	}
	return 0;
}
