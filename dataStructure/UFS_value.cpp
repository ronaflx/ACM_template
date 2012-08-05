/* HOJ cube stacking*/
#include <iostream>
#include <cstring>
#include <cstdio>
using namespace std;
const int SIZE = 300001;
int cnt[SIZE], dis[SIZE], father[SIZE];

void initialize()
{
    for (int i = 0; i < SIZE; i++)
    {
        cnt[i] = 1;
        dis[i] = 0;
        father[i] = i;
    }
}

int set_find(int x)
{
    if (x != father[x])
    {
        int f = father[x];
        father[x] = set_find(father[x]);
        dis[x] += dis[f];
    }
    return father[x];
}
int main()
{
    int t, x, y;
    char ch;
    while (scanf("%d", &t) == 1)
    {
        initialize();
        while (t--)
        {
            scanf(" %c", &ch);
            if (ch == 'M')
            {
                scanf("%d %d", &x, &y);
                int f1 = set_find(x), f2 = set_find(y);
                if (f1 != f2)
                {
                    dis[f2] = cnt[f1];
                    father[f2] = f1;
                    cnt[f1] += cnt[f2];
                }
            }
            else if (ch == 'C')
            {
                scanf("%d", &x);
                int f = set_find(x);
                printf("%d\n", cnt[f] - dis[x] - 1);
            }
        }
    }
    return 0;
}
 
