const int N = 500, M = 500, INF = 1 << 29;
bool g[N][M], chk[M];
int Mx[N], My[M], dx[N], dy[M], dis;
bool searchP(int n, int m) {
	queue<int> Q;
	dis = INF;
	CC(dx, -1);CC(dy, -1);
	for (int i = 0; i < n; ++ i)
		if (Mx[i] == -1) {
			Q.push(i);
			dx[i] = 0;
		}
	while (!Q.empty()) {
		int u = Q.front();
		Q.pop();
		if (dx[u] > dis) break;
		for (int v = 0; v < m; ++ v)
			if (g[u][v] && dy[v] == -1) {
				dy[v] = dx[u] + 1;
				if (My[v] == -1) dis = dy[v];
				else {
					dx[My[v]] = dy[v] + 1;
					Q.push(My[v]);
				}
			}
	}
	return dis != INF;
}
bool Augment(int u, const int m) {
	REP(v, 0, m)
		if (g[u][v] && !chk[v] && dy[v] == dx[u] + 1) {
			chk[v] = true;
			if (My[v] != -1 && dy[v] == dis) continue;
			if (My[v] == -1 || Augment(My[v], m)) {
				My[v] = u;
				Mx[u] = v;
				return true;
			}
		}
	return false;
}
int MaxMatch(int n, int m) {
	int ans = 0;
	CC(Mx, -1);CC(My, -1);
	while (searchP(n, m)) {
		CC(chk, false);
		REP(i, 0, n)
			if (Mx[i] == -1 && Augment(i, m)) ++ ans;
	}
	return ans;
}