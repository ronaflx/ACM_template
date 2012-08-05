class HashTable
{
private:
    const static int SIZE =  1000000;
    const static int MOD = 10007;
    struct HashCell
    {
        int value, idx;
        HashCell *nxt;
    } pool[SIZE], *g[MOD], *pp;
#define hashFunction(x) ((x) % MOD)
public:
    void clear()
    {
        memset(g, 0, sizeof(g));
        pp = pool;
    }
    int find(int x)
    {
        int hash = hashFunction(x);
        for(HashCell *i = g[hash]; i != NULL; i = i->nxt)
        {
            if(i->value == x)
                return i->idx;
        }
        return -1;
    }
    void insert(int x, int idx)
    {
        int hash = hashFunction(x);
        pp->idx = idx;
        pp->value = x;
        pp->nxt = g[hash];
        g[hash] = pp++;
    }
} hashTable;
const int N = 10;
const int STATE_CNT = 1000000;
const int INF = 10000000;
const int HEX = 10;
const int BIT[] = {1, 10, 100, 1000, 10000, 100000,
        1000000, 10000000, 100000000, 1000000000};
int state[2][STATE_CNT], dp[2][STATE_CNT];
int newState(int s[], const int M)
{
    int lab[N], cnt = 0, newS = 0;
    memset(lab, -1, sizeof(lab));
    lab[0] = cnt++;
    for(int i = M - 1; i >= 0; i--)
    {
        newS *= 10;
        if(lab[s[i]] == -1)
            lab[s[i]] = cnt++;
        newS += lab[s[i]];
    }
    return newS;
}
int cnt[N];
void change(int src, int* dest, const int M)
{
    memset(cnt, 0, sizeof(cnt));
    REP(i, 0, M)
    {
        cnt[src % HEX]++;
        dest[i] = src % HEX;
        src /= HEX;
    }
}
bool isOneBlock(int s)
{
    int last = -1;
    while(s)
    {
        int now = s % HEX;
        if(now != 0 && now != last && last != -1) return false;
        if(now != 0) last = now;
        s /= HEX;
    }
    return true;
}
void transfer(int now, int newS, int val, int& newCnt)
{
    int idx = hashTable.find(newS);
    if(idx != -1)
        dp[now][idx] = max(dp[now][idx], val);
    else
    {
        idx = newCnt;
        hashTable.insert(newS, newCnt++);
        state[now][idx] = newS;
        dp[now][idx] = val;
    }
}
int DP(int n, int m, int maps[N][N])
{
    int ans = -INF;
    state[0][0] = dp[0][0] = 0;
    int newS[N], now = 0, pre = 1;
    int oldCnt, newCnt = 1;
    REP(i, 0, n)
    {
        REP(j, 0, m)
        {
            now ^= 1, pre ^= 1;
            oldCnt = newCnt;
            newCnt = 0;
            hashTable.clear();
            REP(k, 0, oldCnt)
            {
                /*注意处理不选则该块的情况，
                  如果不选择该块会导致一个联通块在表示法中消失。
                  则改选法将导致多个联通块*/
                change(state[pre][k], newS, m + 1);
                if(newS[j] == 0 && newS[j + 1] == 0)
                {
                    transfer(now, state[pre][k], dp[pre][k], newCnt);
                    int minn = *max_element(newS, newS + m + 1) + 1;
                    newS[j] = newS[j + 1] = minn;
                    int nxt = newState(newS, m + 1);
                    transfer(now, nxt, dp[pre][k] + maps[i][j], newCnt);
                }
                else if(newS[j] == 0 && newS[j + 1])
                {
                    newS[j] = newS[j + 1];
                    int nxt = newState(newS, m + 1);
                    transfer(now, nxt, dp[pre][k] + maps[i][j], newCnt);
                    if(cnt[newS[j + 1]] > 1)
                    {
                        newS[j] = newS[j + 1] = 0;
                        nxt = newState(newS, m + 1);
                        transfer(now, nxt, dp[pre][k], newCnt);
                    }
                }
                else if(newS[j] && newS[j + 1] == 0)
                {
                    newS[j + 1] = newS[j];
                    int nxt = newState(newS, m + 1);
                    transfer(now, nxt, dp[pre][k] + maps[i][j], newCnt);
                    if(cnt[newS[j]] > 1)
                    {
                        newS[j] = newS[j + 1] = 0;
                        nxt = newState(newS, m + 1);
                        transfer(now, nxt, dp[pre][k], newCnt);
                    }
                }
                else if(newS[j] && newS[j + 1])
                {
                    if(newS[j] == newS[j + 1])
                    {
                        if(cnt[newS[j]] > 2)
                        {
                            int a = newS[j], b = newS[j + 1];
                            newS[j] = newS[j + 1] = 0;
                            int nxt = newState(newS, m + 1);
                            transfer(now, nxt, dp[pre][k], newCnt);
                            newS[j] = a, newS[j + 1] = b;
                        }
                    }
                    else
                    {
                        if(cnt[newS[j]] > 1 && cnt[newS[j + 1]] > 1)
                        {
                            int a = newS[j], b = newS[j + 1];
                            newS[j] = newS[j + 1] = 0;
                            int nxt = newState(newS, m + 1);
                            transfer(now, nxt, dp[pre][k], newCnt);
                            newS[j] = a, newS[j + 1] = b;
                        }
                    }
                    int minn = min(newS[j], newS[j + 1]);
                    for(int b = 0; b <= m; b++)
                        if(newS[b] == newS[j] || newS[b] == newS[j + 1])
                            newS[b] = minn;
                    int nxt = newState(newS, m + 1);
                    transfer(now, nxt, dp[pre][k] + maps[i][j], newCnt);
                }
            }
        }
        now ^= 1, pre ^= 1;
        oldCnt = newCnt;
        newCnt = 0;
        hashTable.clear();
        REP(k, 0, oldCnt)
        {
            if(isOneBlock(state[pre][k]))
                ans = max(ans, dp[pre][k]);
            if(state[pre][k] - BIT[m] > 0)
            {
                change((state[pre][k] - BIT[m]) * 10, newS, m + 1);
                int nxt = newState(newS, m + 1);
                if(nxt != 0)
                    transfer(now, nxt, dp[pre][k], newCnt);
            }
            else if(state[pre][k] != 0)
            {
                change(state[pre][k] * 10, newS, m + 1);
                int nxt = newState(newS, m + 1);
                if(nxt != 0)
                    transfer(now, nxt, dp[pre][k], newCnt);
            }
        }
        transfer(now, 0, 0, newCnt);
    }
    return ans;
}

int main()
{
    int n, m;
    while(scanf("%d", &n) == 1 && n)
    {
        int maps[N][N], ans = -INF;
        m = n;
        REP(i, 0, n)
        {
            REP(j, 0, m)
            {
                scanf("%d", &maps[i][j]);
                ans = max(ans, maps[i][j]);
            }
        }
        if(ans <= 0)
            printf("%d\n", ans);
        else
            printf("%d\n", DP(n, m, maps));
    }
    return 0;
}
