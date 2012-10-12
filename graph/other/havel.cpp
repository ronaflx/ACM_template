/*give the degree of every node,wheather it can build a graph*/
const int N = 10;
int maps[N][N];
struct vertex {
	int v, d;
	bool operator < (const vertex a)const
	{return d > a.d;}
} p[N];
bool canBeGraph(int n) {
	for(int i = 0; i < n; i++) {
		sort(p, p + n);
		for(int j = 1; j <= p[0].d && j < n; j++) {
			p[j].d--;
			if(p[j].d < 0) return false;
			maps[p[0].v][p[j].v] = maps[p[j].v][p[0].v] = 1;
		}
		p[0].d = 0;
	}
	return true;
}
