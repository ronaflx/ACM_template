const int SIZE = 1001;
const double EXP = 1e-5;
const double INF = 1e10;
double mat[SIZE][SIZE], cost[SIZE][SIZE], val[SIZE][SIZE];
int n;
double x[SIZE], y[SIZE], height[SIZE];
double prim(double low)
{
    bool check[SIZE];
    memset(check, false, sizeof (check));
    check[0] = true;
    double totdis = 0, totcost = 0, dist[SIZE], minn = INF;
    dist[0] = 0;
    for (int i = 1; i < n; i++)
        dist[i] = INF;
    int idx = 0, pree[SIZE], tmp;
    memset(pree, 0, sizeof (pree));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            val[i][j] = cost[i][j] - low * mat[i][j];
    for (int i = 1; i < n; i++)
    {
        minn = INF;
        for (int j = 1; j < n; j++)
        {
            if (!check[j])
            {
                if (dist[j] >  val[idx][j])
                {
                    dist[j] = val[idx][j];
                    pree[j] = idx;
                }
                if (dist[j] < minn)
                {
                    minn = dist[j];
                    tmp = j;
                }
            }
        }
        totdis += mat[pree[tmp]][tmp];
        totcost += cost[pree[tmp]][tmp];
        check[tmp] = true;
        idx = tmp;
    }
    return totcost / totdis;
}
double distance(int i, int j)
{
    return sqrt(pow(x[i] - x[j], 2) + pow(y[i] - y[j], 2));
}
int main()
{
    while (scanf("%d", &n) == 1 && n)
    {
        for (int i = 0; i < n; i++)
            scanf("%lf %lf %lf", &x[i], &y[i], &height[i]);
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
            {
                mat[i][j] = distance(i, j);
                cost[i][j] = fabs(height[i] - height[j]);
            }
        double low = 0, tmp;
        while (true)
        {
            tmp = prim(low);
            if (fabs(low - tmp) < EXP)
                break;
            low = tmp;
        }
        printf("%.3lf\n", tmp);
    }
    return 0;
}

