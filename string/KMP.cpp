//be careful with mod string and main string
void prefix(const char *mode, int *next)
{
    int m = strlen(mode), k = -1, i;
    next[0] = -1;
    for (i = 1; i < m; i++)
    {
        while (k > -1 && mode[k + 1] != mode[i]) k = next[k];
        if (mode[k + 1] == mode[i]) k++;
        next[i] = k;
    }
}
int KMP(const char *main, const char *mode)
{
    int n = strlen(main), m = strlen(mode), q = -1, ans = 0;
    int next[LEN], i;
    prefix(mode, next);
    for (i = 0; i < n; i++)
    {
        while (q > -1 && mode[q + 1] != main[i]) q = next[q];
        if (mode[q + 1] == main[i]) q++;
        if (q == m - 1)
        {
            ans++;
            q = next[q];
        }
    }
    return ans;
}

