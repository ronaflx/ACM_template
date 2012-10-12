const int N = 100000;
int father[N], chk[N], dgr[N];
vector<vector<int> > adj, query;
int set_find(int i) {
    return father[i] = i == father[i] ? i : set_find(father[i]);
}
void initialize(int n) {
    adj.assign(n, vector<int>());
    query.assign(n, vector<int>());
    CC(dgr, 0);CC(chk, 0);
}
void LCA(int u) {
    father[u] = u;
    FOREACH(adj[u], i) {
        LCA(*i),father[*i] = u;
    }
    chk[u] = 1;
    FOREACH(query[u], i)if(chk[*i])
            printf("%d\n", set_find(*i));
}
