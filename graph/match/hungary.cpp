/*1. simple maximum match
2.min path cover of DAG = |V| - max match
define: find some edge cover all the nodes
build PXP Bipartite graph do the maximum match
3.min path cover of Bipartite graph  = max match
define : find some point cover all the edge(konig)
4.chessBoard is a Bipartite graph,then you know
5.max independant set(Bipartite graph)=|V| - max match
v is all the point of (set A and set B)
6.largest cloud(Bipartite graph) = max independant set of Complement*/
const int V = 201, E = 10000;
vector<int> adj[V];
int ym[V], chk[V];
bool find_path(int x) {
	FOREACH(adj[x], i) {
		if (chk[*i]) continue;
		chk[*i] = true;
		if (ym[*i] == -1 || find_path(ym[*i])) {
			ym[*i] = x;
			return true;
		}
	}
	return false;
}
int solve(int n) {
	CC(ym, -1);
	int res = 0;
	for (int i = 0; i < n; i++) {
		memset(chk, 0, sizeof (chk));
		if (find_path(i)) res++;
	}
	return res;
}
