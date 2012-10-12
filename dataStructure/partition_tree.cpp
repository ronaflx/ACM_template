/* NlogN find Kth number in any interval */
class partition_tree {
private:
    static const int N = 100005;
    static const int DEPTH = 20;
    int tree[DEPTH][N * 4], sorted[N];
    int toleft[DEPTH][N * 4];
    int n;
public:
    void initialize(int n, int *array) {
        this->n = n;
        for (int i = 1; i <= n; i++)
            sorted[i] = tree[0][i] = array[i];
        sort(sorted + 1, sorted + n + 1);
    }
    void build(int l, int r, int depth) {
        if (l == r) return;
        int mid = (l + r) / 2, same = 0, less = 0;
        for (int i = l; i <= r; i++)
            less += (tree[depth][i] < sorted[mid]);
        same = mid - l + 1 - less;
        int lpos = l, rpos = mid + 1;
        for (int i = l; i <= r; i++) {
            int w = tree[depth][i];
            if (w < sorted[mid]) tree[depth + 1][lpos++] = w;
            else if (w == sorted[mid] && same) {
                tree[depth + 1][lpos++] = w;
                same--;
            }
            else
                tree[depth + 1][rpos++] = w;
            toleft[depth][i] = toleft[depth][l - 1] + lpos - l;
        }
        build(l, mid, depth + 1);
        build(mid + 1, r, depth + 1);
    }
// ptree.query(1, n, a, b, 0, k) th kth number of [a, b]
    int query(int L, int R, int l, int r, int depth, int k) {
        if (l == r) return tree[depth][l];
        int cnt, mid, tmpl, tmpr;
        mid = (R + L) / 2, cnt = toleft[depth][r] - toleft[depth][l - 1];
        if (cnt >= k) {
            tmpl = L + toleft[depth][l - 1] - toleft[depth][L - 1];
            tmpr = tmpl + cnt - 1;
            return query(L, mid, tmpl, tmpr, depth + 1, k);
        } else {
            tmpr = r + toleft[depth][R] - toleft[depth][r];
            tmpl = tmpr - (r - l - cnt);
            return query(mid + 1, R, tmpl, tmpr, depth + 1, k - cnt);
        }
    }
};

