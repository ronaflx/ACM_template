class HashTable {
private:
	const static int SIZE =  1000000, MOD = 10007;
	struct HashCell {
		int value, idx;
		HashCell *mask;
	} pool[SIZE], *g[MOD], *pp;
#define hashFunction(x) ((x) % MOD)
public:
	void clear() {
		memset(g, 0, sizeof(g));
		pp = pool;
	}
	int find(int x) {
		int hash = hashFunction(x);
		for(HashCell *i = g[hash]; i != NULL; i = i->mask) {
			if(i->value == x) return i->idx;
		}
		return -1;
	}
	void insert(int x, int idx) {
		int hash = hashFunction(x);
		pp->idx = idx, pp->value = x;
		pp->mask = g[hash];
		g[hash] = pp++;
	}
} hashTable;
const int N = 10;
const int STATE_CNT = 1000000;
const int INF = 10000000;
const int HEX = 10;/*BIT[i] = HEX^i*/
const int BIT[] = {1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000};
int state[2][STATE_CNT], dp[2][STATE_CNT];
int encode(const int s[], const int M) {/*repeat down to keep min-notation*/
	int lab[N], cnt = 0, newS = 0;
	memset(lab, -1, sizeof(lab));
	lab[0] = cnt++;/*0 means not be used*/
	for(int i = M - 1; i >= 0; i--) {
		newS *= HEX;
		newS += (lab[s[i]] = lab[s[i]] == -1 ?    cnt++ : lab[s[i]]);
	}
	return newS;
}
int cnt[N];/* the number of each plugin */
void decode(int src, int* dest, const int M) {/* decode mask code into array */
	memset(cnt, 0, sizeof(cnt));
	REP(i, 0, M) {
		cnt[src % HEX]++;
		dest[i] = src % HEX;
		src /= HEX;
	}
}
bool isOneBlock(int state) {
	int last = -1;
	while(state) {
		int now = state % HEX;
		if(now != 0 && now != last && last != -1) return false;
		if(now != 0) last = now;
		state /= HEX;
	}
	return true;
}
void transfer(int now, int mask, int val, int& newCnt) {
	int idx = hashTable.find(mask);
	if(idx != -1) {
		dp[now][idx] = max(dp[now][idx], val);
	} else {
		idx = newCnt;
		hashTable.insert(mask, newCnt++);
		state[now][idx] = mask;
		dp[now][idx] = val;
	}
}
int DP(int n, int m, int maps[N][N]) {
	state[0][0] = dp[0][0] = 0;
	int ans = -INF, newS[N], now = 0, pre = 1;
	int oldCnt = 0, newCnt = 1, M = m + 1, mask;
	REP(i, 0, n) {
		REP(j, 0, m) {
			now ^= 1, pre ^= 1;
			oldCnt = 0;
			swap(oldCnt, newCnt);
			hashTable.clear();
			REP(k, 0, oldCnt) {
				decode(state[pre][k], newS, M);
				if(newS[j] == 0 && newS[j + 1] == 0) {
					transfer(now, state[pre][k], dp[pre][k], newCnt);
					newS[j]  = newS[j + 1] = *max_element(newS, newS + M) + 1;
					mask = encode(newS, M);
					transfer(now, mask, dp[pre][k] + maps[i][j], newCnt);
				} else if(newS[j] == 0 && newS[j + 1]) {
					newS[j] = newS[j + 1];
					mask = encode(newS, M);
					transfer(now, mask, dp[pre][k] + maps[i][j], newCnt);
					if(cnt[newS[j + 1]] > 1) {
						newS[j] = newS[j + 1] = 0;
						mask = encode(newS, M);
						transfer(now, mask, dp[pre][k], newCnt);
					}
				} else if(newS[j] && newS[j + 1] == 0) {
					newS[j + 1] = newS[j];
					mask = encode(newS, M);
					transfer(now, mask, dp[pre][k] + maps[i][j], newCnt);
					if(cnt[newS[j]] > 1) {
						newS[j] = newS[j + 1] = 0;
						mask = encode(newS, M);
						transfer(now, mask, dp[pre][k], newCnt);
					}
				} else if(newS[j] && newS[j + 1]) {
					/* drop current block */
					int a = 0, b = 0;
					if(newS[j] == newS[j + 1]) {
						if(cnt[newS[j]] > 2) {
							swap(a, newS[j]), swap(b, newS[j + 1]);
							int mask = encode(newS, M);
							transfer(now, mask, dp[pre][k], newCnt);
							swap(a, newS[j]), swap(b, newS[j + 1]);
						}
					} else {
						if(cnt[newS[j]] > 1 && cnt[newS[j + 1]] > 1) {
							swap(a, newS[j]), swap(b, newS[j + 1]);
							int mask = encode(newS, M);
							transfer(now, mask, dp[pre][k], newCnt);
							swap(a, newS[j]), swap(b, newS[j + 1]);
						}
					}
					/* merge two block */
					int minn = min(newS[j], newS[j + 1]);
					for(int b = 0; b <= m; b++) {
						if(newS[b] == newS[j] || newS[b] == newS[j + 1])
							newS[b] = minn;
					}
					mask = encode(newS, M);
					transfer(now, mask, dp[pre][k] + maps[i][j], newCnt);
				}
			}
		}
		now ^= 1, pre ^= 1;
		oldCnt = 0;
		swap(oldCnt, newCnt);
		hashTable.clear();/* two different mask can change into one */
		REP(k, 0, oldCnt) {
			if(isOneBlock(state[pre][k]))
				ans = max(ans, dp[pre][k]);
			if(state[pre][k] - BIT[m] > 0) {
				decode((state[pre][k] - BIT[m]) * 10, newS, M);
				if(mask = encode(newS, M))/* if mask != 0 */
					transfer(now, mask, dp[pre][k], newCnt);
			} else if(state[pre][k] != 0) {
				decode(state[pre][k] * 10, newS, M);
				if(mask = encode(newS, M))
					transfer(now, mask, dp[pre][k], newCnt);
			}
		}
	}
	return ans;
}

int main() {
	int n;
	while(scanf("%d", &n) == 1 && n) {
		int maps[N][N], ans = -INF;
		REP(i, 0, n) {
			REP(j, 0, n) {
				scanf("%d", &maps[i][j]);
				ans = max(ans, maps[i][j]);
			}
		}
		if(ans <= 0)
			printf("%d\n", ans);
		else
			printf("%d\n", DP(n, n, maps));
	}
	return 0;
}
