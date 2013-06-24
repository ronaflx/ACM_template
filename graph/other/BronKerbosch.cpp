const static int N = 130;
int n, maps[N][N], cnt;
void CountMaximalClique(int *p, int ps, int *x, int xs) {
    if(ps == 0) {
        if(xs == 0) cnt++;
        return ;
    }
    for(int i = 0; i < xs; i++) {
        int j, v = x[i];
        for(j = 0; j < ps && maps[p[j]][v]; j++);
        if(j == ps) return;
    }
    int tmpp[N], tmpps = 0, tmpx[N], tmpxs = 0;
    for(int i = 0; i < ps; i++) {
        int v = p[i];
        tmpps = tmpxs = 0;
        for(int j = i + 1; j < ps; j++) {
            int u = p[j];
            if(maps[v][u])
                tmpp[tmpps++] = u;
        }
        for(int j = 0; j < xs; j++) {
            int u = x[j];
            if(maps[v][u])
                tmpx[tmpxs++] = u;
        }
        CountMaximalClique(tmpp, tmpps, tmpx, tmpxs);
        if(cnt > 1000) return;
        x[xs++] = v;
    }
}
int CountMaximalClique() {
    cnt = 0;
    int  p[N], x[N];
    for(int i = 0; i < n; i++) p[i] = i;
    CountMaximalClique(p, n, x, 0);
    return cnt;
}
