typedef tuple<LL,LL,LL,LL> TPL;
const int N = 150000;
LL x[N],a[N];
#define SUM(c) get<0>(c)
#define BIG(c) get<1>(c)
#define LHS(c) get<2>(c)
#define RHS(c) get<3>(c)
class segment_tree {
public:
	TPL u[524288];
	void update(TPL& c, const TPL& x, const TPL& y) {
		SUM(c) = SUM(x) + SUM(y);
		BIG(c)=max(RHS(x) + LHS(y), max(BIG(x), BIG(y)));
		LHS(c)=max(LHS(x), SUM(x) + LHS(y));
		RHS(c)=max(RHS(x) + SUM(y), RHS(y));
	}
	void build(int x, int y, int t) {
		if(x + 1 == y) {
			LHS(u[t]) = RHS(u[t]) = BIG(u[t]) = 
				max(0ll, SUM(u[t]) = a[x]);
			return;
		}
		int m = (x + y) / 2,p = t * 2,q = p + 1;
		build(x, m, p);
		build(m, y, q);
		update(u[t], u[p], u[q]);
	}
	TPL getans(int L, int R, int x, int y, int t) {
		if(L <= x && y <= R) return u[t];
		int m = (x + y) / 2,p = t * 2,q = p + 1;
		TPL lhs, rhs, ret;
		if(L < m) lhs = getans(L, R, x, m, p);
		if(m < R) rhs = getans(L, R, m, y, q);
		update(ret, lhs, rhs);
		return ret;
	}
}st;

int main() {
	int n, m, c, p;
	ios::sync_with_stdio(false);
	cin >> n >> m >> c;
	for(int i=0;i<n;i++) cin >> x[i];
	for(int i=1;i<n;i++) {
		cin >> p;
		a[i] = (x[i] - x[i - 1]) * 50 - p * c;
	}
	st.build(0, n, 1);
	long long ans = 0;
	for(int i = 0;i < m;i++) {
		cin >> c >> p;
		ans += BIG(st.getans(c, p, 0, n, 1));
	}
	printf("%.12f\n",ans*0.01);
}
