using namespace std;
typedef long long USETYPE;
const USETYPE INF = numeric_limits<USETYPE>::max();//<limits>
template<typename T = int>
class mincost {
private:
    const static int N = 1000;
    const static int E = 100000;
    struct edge {
        int u, v;
        T cost, cap;
        edge *nxt;
    } pool[E], *g[N], *pp, *pree[N];
    T dist[N];

    bool SPFA(int n,int s, int t) {
        fill(dist, dist + n, INF);
        int tail = 0, q[N] = {s};
        dist[s] = 0;
        bool vst[N] = {false};
        vst[s] = true;
        for(int i = 0; i <= tail; i++) {
            int u = q[i % n];
            for(edge *j = g[u]; j != NULL; j= j->nxt) {
                int v = j->v;
                if(j->cap && dist[u] != INF && dist[v] > dist[u] + j->cost) {
                    dist[v] = dist[u] + j->cost;
                    pree[v] = j;
                    if(!vst[v]) {
                        tail++;
                        q[tail % n] = v;
                        vst[v] = true;
                    }
                }
            }
            vst[u] = false;
        }
        return dist[t] < INF;
    }
public:
#define OP(i) (((i) - pool) ^ 1)
    void addedge(int u, int v, T cap, T cost) {
        pp->u = u, pp->v = v;
        pp->cost = cost, pp->cap = cap;
        pp->nxt = g[u],g[u] = pp++;
    }
    void initialize() {
        CC(g, 0);
        pp = pool;
    }
    pair<T, T> mincostflow(int n, int s, int t) {
        T flow = 0, cost = 0;
        while(SPFA(n, s, t)) {
            T minf = INF;
            for(int i = t; i != s; i = pree[i]->u)
                minf = min(minf, pree[i]->cap);
            for(int i = t; i != s; i = pree[i]->u) {
                pree[i]->cap -= minf;
                pool[OP(pree[i])].cap += minf;
                cost += minf * pree[i]->cost;
            }
            flow += minf;
        }
        return make_pair(flow, cost);
    }
};

 
 


int main() {
	return 0;
}
