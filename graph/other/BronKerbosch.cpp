/* 求无向图极大团的个数
 * 极大团就一个被不被其他的完全子图包含的完全子图
 * 最大团一定是一个极大团，但是极大团不一定是最大团
*/
const static int N = 130;
int n, maps[N][N], cnt;
void countClique(int *p, int ps, int *x, int xs) {
        if(ps == 0) {
                if(xs == 0) cnt++;
                return ;
        }
        for(int i = 0;i < xs;i++) {
                int j, v = x[i];
                for(j = 0;j < ps && maps[p[j]][v];j++);
                if(j == ps) return;
        }
        int tmpp[N], tmpps = 0, tmpx[N], tmpxs = 0;
        for(int i = 0;i < ps;i++) {
                int v = p[i];
                tmpps = tmpxs = 0;
                for(int j = i + 1;j < ps;j++) {
                        int u = p[j];
                        if(maps[v][u])
                                tmpp[tmpps++] = u;
                }
                for(int j = 0;j < xs;j++) {
                        int u = x[j];
                        if(maps[v][u])
                                tmpx[tmpxs++] = u;
                }
                countClique(tmpp, tmpps, tmpx, tmpxs);
                if(cnt > 1000) return;
                x[xs++] = v;
        }
}
int countClique() {
        cnt = 0;
        int  p[N], x[N];
        for(int i = 0;i < n;i++) p[i] = i;
        countClique(p, n, x, 0);
        return cnt;
}
