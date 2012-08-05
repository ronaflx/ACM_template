/* boy[i][j]   gg[i] to mm[j]
 * girl[i][j]  mm[i] to gg[j]*/
const int N = 26;
const int M = 128;
int boy[N][N], girl[N][N];
int my[N], mx[N], now[N];
void Gale_Shapley(int n)
{
    queue<int> q;
    for(int i = 0; i < n; i++) q.push(i);
    while(!q.empty())
    {
        int i = q.front();q.pop();
        int j = now[i]++, mm = boy[i][j];
        if(my[mm] == -1 || girl[mm][my[mm]] > girl[mm][i])
        {
            if(my[mm] != -1) q.push(my[mm]);
            my[mm] = i, mx[i] = mm;
        }
        else q.push(i);
    }
}

char nameB[N], nameG[N];
void output(int n)
{
    for(int i = 0; i < n; i++)
        printf("%c %c\n", nameB[i], nameG[mx[i]]);
}

int hashB[M], hashG[M];
void initialize()
{
    memset(hashB, 0, sizeof(hashB)), memset(hashG, 0, sizeof(hashG));
    memset(my, -1, sizeof(my)),          memset(now, 0, sizeof(now));
}
