const int SIZE = 10110, INF = 100000000;
vector<pair<pair<int, int>,int> >edge;
int n, a0, a1, b0, b1, l0, l1, d[SIZE];
void Bellman_ford(int n,int p) {
	fill(d,d + n, INF);
	d[n - 1] = 0;
	for(int i = 1;i < n;i++) {
		bool unfind = true;
		FOREACH(edge, i) {
			int v = i->first.first, u = i->first.second, val = i->second;
			if(d[u] > d[v] + val)
			{
				unfind = false;
				d[u] = d[v] + val;
			}
		}
		if(unfind) break;
	}
	FOREACH(edge, i) {
		int v = i->first.first, u = i->first.second, val = i->second;
		if(d[u] > d[v] + val) {
			puts("-1");
			return;
		}
	}
	for(int i = 1;i <= p;i++)
		printf("%d", d[i] - d[i - 1]);
	puts("");
}
int main() {
	while(scanf("%d%d%d%d%d%d%d", &n, &a0, &b0, &l0, &a1, &b1, &l1) == 7) {
		edge.clear();
		for(int i = 1;i  + l0 - 1 <= n;i++) {
			edge.push_back(make_pair(make_pair(i-1,i+l0-1),l0-a0));
			edge.push_back(make_pair(make_pair(i+l0-1,i-1),b0 - l0));
		}
		for(int i = 1;i + l1 - 1 <= n;i++) {
			edge.push_back(make_pair(make_pair(i-1,i+l1-1),b1));
			edge.push_back(make_pair(make_pair(i+l1-1,i-1),-a1));
		}
		for(int i = 1;i <= n;i++) {
			edge.push_back(make_pair(make_pair(i - 1, i), 1));
			edge.push_back(make_pair(make_pair(i, i - 1), 0));
		}
		for(int i = 0;i <= n;i++)
			edge.push_back(make_pair(make_pair(n + 1, i), 0));
		Bellman_ford(n + 2, n);
	}
	return 0;
}
