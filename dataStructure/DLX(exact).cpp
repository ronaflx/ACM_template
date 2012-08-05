const int SIZE = 16;//here
const int SQRTSIZE = 4;//here
const int ALLSIZE = SIZE * SIZE,  ROW = SIZE * SIZE * SIZE;
const int INF = 100000000, COL = SIZE * SIZE * 4;
const int N = ROW * COL,  HEAD = 0;
 
#define BLOCK(r, c) ((r) * SQRTSIZE + c)
#define CROW(r, c, k) ((r) + (c) * SIZE + (k) * SIZE * SIZE)
#define ROWCOL(i, j) ((i) * SIZE + (j))
#define ROWCOLOR(i, k) (ALLSIZE + (i) * SIZE + k)
#define COLCOLOR(j, k) (2 * ALLSIZE + (j) * SIZE + k)
#define BLOCKCOLOR(i, j, k) (3*ALLSIZE+BLOCK((i/SQRTSIZE),(j/SQRTSIZE))*SIZE+(k))
 
int maps[ROW][COL];
int ans[N];
char sudoku[SIZE][SIZE];
int r[N], l[N], u[N], d[N], c[N], s[N];
int n, m, ansd, row[N];
 
void resume(const int col)
{
    for (int i = u[col]; i != col; i = u[i])
    {
        for (int j = l[i]; j != i; j = l[j])
        {
            u[d[j]] = j;
            d[u[j]] = j;
            s[c[j]]++;
        }
    }
    r[l[col]] = col;
    l[r[col]] = col;
}
 
void cover(const int col)
{
    r[l[col]] = r[col];
    l[r[col]] = l[col];
    for (int i = d[col]; i != col; i = d[i])
    {
        for (int j = r[i]; j != i; j = r[j])
        {
            u[d[j]] = u[j];
            d[u[j]] = d[j];
            s[c[j]]--;
        }
    }
}
 
void initialize(int n, int m)
{
    l[HEAD] = m;
    r[HEAD] = 1;
    for (int i = 1; i <= m; i++)
    {
        if (i == m)
            r[i] = HEAD;
        else
            r[i] = i + 1;
        l[i] = i - 1;
        c[i] = u[i] = d[i] = i;
        s[i] = 0;
    }
    int size = m;
    for (int i = 1; i <= n; i++)
    {
        int first = 0;
        for (int j = 1; j <= m; j++)
        {
            if (maps[i - 1][j - 1] == 0)
                continue;
            size++;
            int tmp = u[j];
            u[j] = size;
            d[tmp] = size;
            d[size] = j;
            u[size] = tmp;
            if (!first)
            {
                first = size;
                l[size] = r[size] = size;
            }
            else
            {
                tmp = l[first];
                r[tmp] = size;
                l[size] = tmp;
                l[first] = size;
                r[size] = first;
            }
            row[size] = i;
            s[j]++;
            c[size] = j;
        }
    }
}
 
bool dfs(int depth)
{
    if (r[HEAD] == HEAD)
    {
        ansd = depth;
        return true;
    }
    int minn = INF, v;
    for (int i = r[HEAD]; i != HEAD; i = r[i])
    {
        if (s[i] < minn)
        {
            v = i;
            minn = s[i];
        }
    }
    cover(v);
    for (int i = d[v]; i != v; i = d[i])
    {
        for (int j = r[i]; j != i; j = r[j])
            cover(c[j]);
        ans[depth] = row[i] - 1;
        if (dfs(depth + 1))
            return true;
        for (int j = l[i]; j != i; j = l[j])
            resume(c[j]);
    }
    resume(v);
    ans[depth] = -1;
    return false;
}
 
int main()
{
    n = ROW;
    m = COL;
    while (scanf(" %c", &sudoku[0][0]) == 1)
    {
        for(int i = 0; i < SIZE; i++)
        {
            for(int j = 0; j < SIZE; j++)
            {
                if(i + j)
                    scanf(" %c", &sudoku[i][j]);
            }
        }
        memset(maps, 0, sizeof (maps));
        for (int i = 0; i < SIZE; i++)
        {
            for (int j = 0; j < SIZE; j++)
            {
                if (sudoku[i][j] == '-')
                {
                    for (int k = 0; k < SIZE; k++)
                    {
                        maps[CROW(i, j, k)][ROWCOL(i, j)] = 1;
                        maps[CROW(i, j, k)][ROWCOLOR(i, k)] = 1;
                        maps[CROW(i, j, k)][COLCOLOR(j, k)] = 1;
                        maps[CROW(i, j, k)][BLOCKCOLOR(i, j, k)] = 1;
                    }
                }
                else
                {
                    int k = sudoku[i][j] - 'A';//here
                    maps[CROW(i, j, k)][ROWCOL(i, j)] = 1;
                    maps[CROW(i, j, k)][ROWCOLOR(i, k)] = 1;
                    maps[CROW(i, j, k)][COLCOLOR(j, k)] = 1;
                    maps[CROW(i, j, k)][BLOCKCOLOR(i, j, k)] = 1;
                }
            }
            initialize(n, m);
        }
        if (dfs(0))
        {
            for (int i = 0; i < ansd; i++)
                sudoku[ans[i] % SIZE][ans[i] % ALLSIZE / SIZE] = ans[i] / ALLSIZE + 'A';//here
            for(int i = 0; i < SIZE; i++)
            {
                for(int j = 0; j < SIZE; j++)
                    putchar(sudoku[i][j]);
                puts("");
            }
        }
        puts("");
    }
    return 0;
}
 
