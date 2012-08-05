const int MAXN = 21000;
struct Sfx
{
    int i;
    int key[2];
    bool operator < (const Sfx& s) const
    {
        return key[0] == s.key[0] ? key[1] < s.key[1] : key[0] < s.key[0];
    }
} sfx[MAXN], temp[MAXN];
int rank[MAXN], bucket[MAXN], height[MAXN];// rank from 0 to n - 1
//radixSort，second then first
void radixSort(Sfx* in, int n, int idx, Sfx* out)
{
    memset(bucket, 0, sizeof(int) * (n + 1));
    for (int i = 0; i < n; i++)
        bucket[in[i].key[idx]]++;
    for (int i = 1; i <= n; i++)
        bucket[i] += bucket[i - 1];
    for (int i = n - 1; i >= 0; i--)//for down
        out[--bucket[in[i].key[idx]]] = in[i];
}
void buildSA(const char* text, int n)
{
    for (int i = 0; i < n; i++)
    {
        sfx[i].i = sfx[i].key[1] = i;
        sfx[i].key[0] = text[i];
    }
    sort(sfx, sfx + n);
    for (int i = 0; i < n; i++)
        sfx[i].key[1] = 0;
    int wid = 1;
    while (wid < n)
    {
        rank[sfx[0].i] = 0;
        for (int i = 1; i < n; i++)
            rank[sfx[i].i] = rank[sfx[i - 1].i] + (sfx[i - 1] < sfx[i]);
        for (int i = 0; i < n; i++)
        {
            sfx[i].i = i;
            sfx[i].key[0] = rank[i];
            sfx[i].key[1] = i + wid < n ? rank[i + wid]: 0;
        }
        radixSort(sfx, n, 1, temp);
        radixSort(temp, n, 0, sfx);
        wid <<= 1;
    }
}
void calHeight(const char* text, int* rank, int n)
{
    //h[i] = height[rank[i]], h[i] >= h[i - 1] - 1;
    for(int i = 0; i < n; i++)
        rank[sfx[i].i] = i;
    for(int i = 0, k = 0, j; i < n; i++)
    {
        if (rank[i] == 0)
            height[rank[i]] = 0;
        else
        {
            if(k > 0) k-- ;
            for (j = sfx[rank[i] - 1].i; text[i + k] == text[j + k]; k++);
            height[rank[i]] = k;
        }
    }
}
int RMQ[MAXN][20];
void buildRMQ(int n, int* height)
{
    for(int i = 1; i <= n; i++) RMQ[i][0] = height[i - 1];
    for (int j = 1; j <= log(n + 0.00) / log(2.0); j++)
        for (int i = 1; i + (1 << j) - 1 <= n; i++)
            RMQ[i][j] = min(RMQ[i][j - 1], RMQ[i + (1 << (j - 1))][j - 1]);
}
int queryRMQ(int a, int b)
{
    int len = log(b - a + 1.0) / log(2.0);
    return min(RMQ[a][len], RMQ[b - (1 << len) + 1][len]);
}
int queryLCP(int a, int b)
{
    a = rank[a] + 1;
    b = rank[b] + 1;
    if(a > b) swap(a, b);
    return queryRMQ(a + 1, b);
}
