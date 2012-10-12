int father[N], rank[N];
int find_set(int x) {
    return father[x] = father[x] == x ? x :find_set(father[x]);
}
bool union_set(int x,int y) {
    if(x != y) {
        if(rank[x] < rank[y]) father[x] = y;
        else {
            rank[x] += rank[x] == rank[y];
            father[y] = x;
        }
        return false;
    }
    return true;
}
bool link_set(int x, int y) {
    return union_set(find_set(x), find_set(y));
}
void initialize(int n) {
    for(int i = 1;i <= n;i++) {
        rank[i] = 0;
        father[i] = i;
    }
}
