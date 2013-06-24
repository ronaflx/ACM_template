const int MAXN = 50001;
int sfx[MAXN], temp[MAXN], key[MAXN][2];
int _rank[MAXN], bucket[MAXN], height[MAXN];
// _rank from 0 to n - 1
void radixSort(int* in, int n, int idx, int* out) {
	memset(bucket, 0, sizeof(int) * (n + 1));
	for (int i = 0; i <  n; i++) bucket[key[i][idx]]++;
	for (int i = 1; i <= n; i++) bucket[i] += bucket[i - 1];
	for (int i = n - 1;i >= 0;i--)out[--bucket[key[i][idx]]]=in[i];
}
#define KEY0(i) key[i][0]
#define KEY1(i) key[i][1]
int cmp(int i, int j) {
return KEY0(i) == KEY0(j) ? KEY1(i) < KEY1(j) : KEY0(i) < KEY0(j);
}
/*text can't contain 0, 0 is used as terminal*/
void buildSA(const char* text, int n) {
	for (int i = 0; i < n; i++)
		sfx[i] = i, key[i][0] = text[i], key[i][1] = 0;
	sort(sfx, sfx + n, cmp);
	for(int i = 0;i < n;i++) key[i][0] = text[sfx[i]];
	int wid = 1;
	while (wid < n) {
		_rank[sfx[0]] = 0; 
		for (int i = 1; i < n; i++) 
			_rank[sfx[i]] = _rank[sfx[i - 1]] + cmp(i - 1, i);
		for (int i = 0; i < n; i++) {
			sfx[i] = i;
			key[i][1] = i + wid < n ? _rank[i + wid]: 0;
		}
		radixSort(sfx, n, 1, temp);
		for(int i = 0;i < n;i++) key[i][0] = _rank[temp[i]];
		radixSort(temp, n, 0, sfx);
		for(int i = 0;i < n;i++) key[i][0] = _rank[sfx[i]];
		for(int i = 0;i < n;i++) 
			key[i][1] = wid + sfx[i] < n ? _rank[sfx[i] + wid] : 0;
		wid <<= 1;
	}
}
void calHeight(const char* text, int* _rank, int n) {
	//height[i] = lcp(suffix(sa[i - 1]), suffix[sa[i]])
	for(int i = 0; i < n; i++) _rank[sfx[i]] = i;
	height[0] = 0;
	for(int i = 0, k = 0, j; i < n; i++) {
		if(_rank[i] != 0) {
			if(k > 0) k-- ;
			for (j = sfx[_rank[i] - 1]; text[i + k] == text[j + k]; k++);
			height[_rank[i]] = k;
		}
	}
}
int RMQ[MAXN][20];
//n = len(text),height[0] means nothing
void buildRMQ(int n, int* height) {
	for(int i = 1; i <= n; i++) RMQ[i][0] = height[i - 1];
	for (int j = 1; j <= log(n + 0.00) / log(2.0); j++)
		for (int i = 1; i + (1 << j) - 1 <= n; i++)
			RMQ[i][j] = min(RMQ[i][j - 1],RMQ[i + (1<<(j-1))][j - 1]);
}
int queryRMQ(int a, int b) {
	int len = log(b - a + 1.0) / log(2.0);
	return min(RMQ[a][len], RMQ[b - (1 << len) + 1][len]);
}
int queryLCP(int a, int b) {
	a = _rank[a] + 1, b = _rank[b] + 1;
	if(a > b) swap(a, b);
	return queryRMQ(a + 1, b);
}