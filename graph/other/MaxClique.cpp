const int N = 50;
int maps[N][N], found, mc, n;
int c[N], answer[N], record[N];
void dfs(int GraphSize,int *s, int CliqueSize)
{
    if(GraphSize == 0)
    {
        if(CliqueSize > mc)
        {
            mc = CliqueSize;
            found = true;
            copy(record, record + mc, answer);
        }
        return ;
    }
    for(int i = 0; i < GraphSize; i++)
    {
        if(CliqueSize + GraphSize <= mc || c[s[i]] + CliqueSize <= mc)
            return;
        int tmps[N],tmpSize = 0;
        record[CliqueSize] = s[i];
        for(int j = i + 1; j < GraphSize; j++)
            if(maps[s[i]][s[j]])
                tmps[tmpSize++] = s[j];
        dfs(tmpSize, tmps, CliqueSize + 1);
        if(found)
            return ;
    }
}
void initialize()
{
    memset(maps, false, sizeof(maps));
    mc = 0;
}
int findMaxClique(int n)
{
    for(int i = n - 1; i >= 0; i--)
    {
        found = false;
        int tail = 0, s[N];
        for(int j = i + 1; j < n; j++)
            if(maps[i][j])
                s[tail++] = j;
        record[0] = i;
        dfs(tail, s, 1);
        c[i] = mc;
    }
    return mc;
}