const int N = 11;
int editDistance(const char* a, const char* b) {
	int dp[N][N];
	int n = strlen(a), m = strlen(b);
	for(int i = 0;i <= m;i++) {
		dp[0][i] = i;
	}
	for(int i = 0;i <= n;i++) {
		dp[i][0] = i;
	}
	for(int i = 1;i <= n;i++) {
		for(int j = 1;j <= m;j++) {
			dp[i][j] = min(dp[i - 1][j], dp[i][j - 1]) + 1;
			dp[i][j] = min(dp[i][j], dp[i - 1][j - 1] + (a[i - 1] != b[j - 1]));
		}
	}
	return dp[n][m];
}
struct BK_node {
	BK_node(){
		memset(ptr, 0, sizeof(ptr));
	}
	BK_node(const char* data) {
		strcpy(this->data, data);
		memset(ptr, 0, sizeof(ptr));
	}
	char data[N];
	BK_node* ptr[N];
};
void insert(BK_node* &root, const char* cur) {
	if(root == NULL) {
		root = new BK_node(cur);
		return;
	}
	int dis = editDistance(cur, root->data);
	insert(root->ptr[dis], cur);
}
int query(BK_node* root, const char* cur, int threshold) {
	if(root == NULL) return 0;
	int res = 0;
	int dis = editDistance(cur, root->data);
	int low = max(0, dis - threshold);
       	int high = min(N - 1, threshold + dis) + 1;
	if(dis <= threshold) {
		res++;
	}
	for(int i = low;i < high;i++) {
		res += query(root->ptr[i], cur, threshold);
	}
	return res;
}
void clear(BK_node* &root) {
	if(root == NULL) return;
	for(int i = 0;i < N;i++) {
		clear(root->ptr[i]);
	}
	delete root;
	root = NULL;
}
