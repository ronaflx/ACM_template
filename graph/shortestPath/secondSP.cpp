/*由最短路推出答案,dijskstra算法变形
* 本题求了最短路以及比最短路大1的路的条数。
* poj 3463 Sightseeing
* 本模板允许向回走，再次走到原来经过的点
* 如果要求不课重复走的次短路，记录最短路前驱
* 然后枚举删掉最短路上的边，再求最短路取最小值便是次短路 */
const int SIZE = 1010;
const int INF = 1100000000;
class Edge
{
public:
    int v, cost, pt;

    bool operator<(const Edge a) const
    {
        return cost > a.cost;
    }
} tmp, nxt, beg;
vector <Edge> adj[SIZE];
int dist[SIZE][2], cnt[SIZE][2];
int n, m;
bool check[SIZE][2];

int dijkstra(int start, int end)
{
    memset(check, false, sizeof (check));
    memset(cnt, 0, sizeof (cnt));
    for (int i = 1; i <= n; i++)
        dist[i][0] = dist[i][1] = INF;
    priority_queue<Edge> pq;
    beg.v = start, beg.pt = 0, beg.cost = 0;
    pq.push(beg);
    cnt[start][0] = 1;
    dist[start][0] = 0;
    while (!pq.empty())
    {
        beg = pq.top();
        pq.pop();
        if (check[beg.v][beg.pt])
            continue;
        check[beg.v][beg.pt] = true;
        int s = adj[beg.v].size(), w, cst;
        for (int i = 0; i < s; i++)
        {
            tmp = adj[beg.v][i];
            w = tmp.v, cst = tmp.cost;
            if (dist[w][0] > dist[beg.v][beg.pt] + cst && !check[w][0])
            {
                dist[w][1] = dist[w][0];
                cnt[w][1] = cnt[w][0];
                dist[w][0] = dist[beg.v][beg.pt] + cst;
                cnt[w][0] = cnt[beg.v][beg.pt];
                nxt.v = w, nxt.pt = 0, nxt.cost = dist[w][0];
                pq.push(nxt);
		nxt.v = w,nxt.pt = 1,nxt.cost = dist[w][1];
		pq.push(nxt);
            }
            else if (dist[w][0] == dist[beg.v][beg.pt] + cst && !check[w][0])
            {
                cnt[w][0] += cnt[beg.v][beg.pt];
                nxt.v = w, nxt.pt = 0, nxt.cost = dist[w][0];
                pq.push(nxt);
            }
            else if (dist[w][1] > dist[beg.v][beg.pt] + cst && !check[w][1])
            {
                dist[w][1] = dist[beg.v][beg.pt] + cst;
                cnt[w][1] = cnt[beg.v][beg.pt];
                nxt.pt = 1, nxt.v = w, nxt.cost = dist[w][1];
                pq.push(nxt);
            }
            else if (dist[w][1] == dist[beg.v][beg.pt] + cst && !check[w][1])
            {
                cnt[w][1] += cnt[beg.v][beg.pt];
                nxt.pt = 1, nxt.v = w, nxt.cost = dist[w][1];
                pq.push(nxt);
            }
        }
    }
    int num = cnt[end][0];
    if (dist[end][0] + 1 == dist[end][1])
        num += cnt[end][1];
    return num;
}

int main()
{
    int T;
    scanf("%d", &T);
    while (T--)
    {
        scanf("%d %d", &n, &m);
        for (int i = 1; i <= n; ++i)
            adj[i].clear();
        for (int i = 0; i < m; ++i)
        {
            int a, b, c;
            scanf("%d %d %d", &a, &b, &c);
            beg.v = b, beg.cost = c;
            adj[a].push_back(beg);
        }
        int start, end;
        scanf("%d %d", &start, &end);
        printf("%d\n", dijkstra(start, end));
    }
    return 0;
}

