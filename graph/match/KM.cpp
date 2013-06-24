struct Graph {
    int ny, nx;
    double w[N][N];
    double lx[N], ly[N];
    int linky[N];
    int visx[N], visy[N];
    double slack[N];
    void init(int nn,int mm) {
        nx = nn;
        ny = mm;
    }
    bool find(int x) {
        visx[x] = 1;
        for(int y = 1; y <= ny; y++) {
            if(visy[y]) continue;
            double t = lx[x] + ly[y] - w[x][y];
            if(t < eps) {
                visy[y] = 1;
                if(linky[y] == -1 || find(linky[y])) {
                    linky[y] = x;
                    return true;
                }
            } else if(slack[y] > t) {
                slack[y] = t;
            }
        }
        return false;
    }
    double KM() {
        memset(linky, -1, sizeof(linky));
        for(int i = 1; i <= nx; i++) lx[i] = -INF;
        memset(ly, 0, sizeof(ly));
        for(int i = 1; i <= nx; i++)
            for(int j = 1; j <= ny; j++)
                if(w[i][j] > lx[i]) lx[i] = w[i][j];
        for(int x = 1; x <= nx; x++) {
            for(int i = 1; i <= ny; i++) slack[i] = INF;
            while(true) {
                memset(visx, 0, sizeof(visx));
                memset(visy, 0, sizeof(visy));
                if(find(x)) break;
                double d = INF;
                for(int i = 1; i <= ny; i++)
                    if(!visy[i]) d = min(d, slack[i]);
                if(d == INF) return -1;
                for(int i = 1; i <= nx; i++)
                    if(visx[i]) lx[i] -=d;
                for(int i = 1; i <= ny; i++)
                    if(visy[i]) ly[i] += d;
                    else slack[i] -= d;
            }
        }
        int cnt = 0;
        for(int i = 1; i <= ny; i++)
            if(linky[i] != -1) cnt++;
        if(cnt != nx) return -1;
        double tp = 0;
        for(int i = 1; i <= ny; i++)
            if(linky[i] != -1 ) tp += w[linky[i]][i];
        return tp;
    }
}g;