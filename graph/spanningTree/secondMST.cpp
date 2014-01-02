const int SIZE = 101, INF = 0x7fffff;
struct Edge {
  int v, w, val;
  bool operator<(const Edge a) const { return val < a.val; }
} edge[SIZE * SIZE];
bool check[SIZE], used[SIZE * SIZE];
int father[SIZE], rank[SIZE];
int max_val[SIZE][SIZE], mst;
vector<Edge> adj[SIZE];
int set_find(int x) {
  return father[x] = father[x] == x ? father[x] : set_find(father[x]);
}
bool set_link(int x, int y) {
  if (x == y) return false;
  if (rank[x] < rank[y])
    father[x] = y;
  else {
    father[y] = x;
    rank[x] += rank[x] == rank[y];
  }
  return true;
}
bool set_union(int x, int y) { return set_link(set_find(x), set_find(y)); }
int t, n, m;
void initialize() {
  scanf("%d %d", &n, &m);
  for (int i = 1; i <= n; i++) {
    adj[i].clear();
    father[i] = i;
    rank[i] = 0;
  }
  mst = 0;
  int v, w, val;
  for (int i = 0; i < m; i++) {
    scanf("%d %d %d", &v, &w, &val);
    edge[i].v = v;
    edge[i].w = w;
    edge[i].val = val;
  }
  memset(check, false, sizeof(check));
  memset(used, false, sizeof(used));
}
void kruskal() {
  for (int i = 0; i < m; i++) {
    if (set_union(edge[i].v, edge[i].w)) {
      Edge temp = edge[i];
      adj[edge[i].v].push_back(temp);
      temp.w = temp.v;
      adj[edge[i].w].push_back(temp);
      mst += edge[i].val;
      used[i] = true;
    }
  }
}
void dfs(int now, int pre, int maxn, int val) {
  check[now] = true;
  max_val[pre][now] = max(maxn, val);
  int s = adj[now].size(), w;
  for (int i = 0; i < s; i++) {
    w = adj[now][i].w;
    if (!check[w]) {
      dfs(w, pre, max_val[pre][now], adj[now][i].val);
    }
  }
}
bool sst() {
  for (int i = 0; i < m; i++) {
    if (!used[i]) {
      if (edge[i].val == max_val[edge[i].v][edge[i].w]) return false;
    }
  }
  return true;
}
int main() {
  scanf("%d", &t);
  while (t--) {
    initialize();
    sort(edge, edge + m);
    kruskal();
    for (int i = 1; i <= n; i++) {
      int s = adj[i].size(), w;
      memset(check, false, sizeof(check));
      check[i] = true;
      for (int j = 0; j < s; j++) {
        w = adj[i][j].w;
        dfs(w, i, 0, adj[i][j].val);
      }
    }
    if (sst())
      printf("%d\n", mst);
    else
      printf("Not Unique!\n");
  }
  return 0;
}
