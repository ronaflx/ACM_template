/* val must be positive
 * min match use INF - val
 * must build a matrix[V][V]*/
const int V = 100;
const int INF = 100000;
int val[V][V], lx[V], ly[V], my[V];
bool visx[V], visy[V];
void initialize(int n)
{
    CC(val, 0), CC(ly, 0), CC(my, -1);
    fill(lx, lx + n, -INF);
}
bool find_path(int x, const int n)
{
    visx[x] = true;
    for(int i = 0; i < n; i++)
    {
        if(!visy[i] && lx[x] + ly[i] == val[x][i])
        {
            visy[i] = true;
            if(my[i] == -1 || find_path(my[i], n))
            {
                my[i] = x;
                return true;
            }
        }
    }
    return false;
}
int solve(int n)
{
    for(int i = 0; i < n; i++)
        lx[i] = *max_element(val[i], val[i] + n);
    int dx, sum = 0;
    for(int i = 0; i < n; i++)
    {
        while(true)
        {
            CC(visx, 0), CC(visy, 0);
            if(find_path(i, n)) break;
            dx = INF;
            for(int j = 0; j < n; j++)
            {
                if(!visx[j]) continue;
                for(int k = 0; k < n; k++)
                {
                    if(visy[k]) continue;
                    dx = min(dx, lx[j] + ly[k] - val[j][k]);
                }
            }
            for(int j = 0; j < n; j++)
            {
                if(visx[j]) lx[j] -= dx;
                if(visy[j]) ly[j] += dx;
            }
        }
    }
    for(int i = 0; i < n; i++)
        sum += INF - val[my[i]][i];
    return sum;
}
 
