struct Point {
	double x, y;
	Point(double xx = 0, double yy = 0)
	{x = xx, y = yy;}
};
struct polygon {
	int n;
	Point p[MAXN];
};
struct Line {//a->b left half
	Point a, b;
};
int dblcmp(double x) {
	if(fabs(x) < EPS) return 0;
	return x < 0 ? -1: 1;
}
double det(double x1, double y1, double x2, double y2) {
	return x1 * y2 - x2 * y1;
}
double cross(Point a, Point b, Point c) { //ab x ac
	return det(b.x - a.x, b.y - a.y, c.x - a.x, c.y - a.y);
}//叉积>0代表在左边, <0代表在右边, =0代表共线
bool linecross(Point a, Point b, Point c, Point d, Point& temp) {//直线求交点
	if((b.x - a.x) * (d.y - c.y) == (d.x - c.x) * (b.y - a.y)) return false;
	double s1, s2;
	int d1, d2;
	d1 = dblcmp(s1 = cross(a, b, c));
	d2 = dblcmp(s2 = cross(a, b, d));
	temp.x = (c.x * s2 - d.x * s1) / (s2 - s1);
	temp.y = (c.y * s2 - d.y * s1) / (s2 - s1);
	return true;
}
double at2[MAXN];
int ord[MAXN], dq[MAXN + 1], lnum;
Line ls[MAXN]; //半平面集合
inline bool cmp(int u, int v) {//象限排序
	if(dblcmp(at2[u] - at2[v]) == 0)
		return dblcmp(cross(ls[v].a, ls[v].b, ls[u].b)) >= 0;
	return at2[u] < at2[v];
}
bool judgeout(int x, int y, int z) {//判断交点在当前半平面外
	Point pnt;
	linecross(ls[x].a, ls[x].b, ls[y].a, ls[y].b, pnt);
	//在右面返回true，排除三点共线，改成<=
	return dblcmp(cross(ls[z].a,ls[z].b, pnt)) < 0;
}
void HalfPlaneIntersection(polygon &pg) {
	int n = lnum , tmpn , i;
	for(i = 0 ; i < n ; i ++) {
		at2[i] = atan2(ls[i].b.y - ls[i].a.y, ls[i].b.x - ls[i].a.x);
		ord[i] = i;
	}
	sort(ord, ord + n, cmp);
	for (i = 1 , tmpn = 1 ; i < n ; i++) //处理重线的情况
		if(dblcmp(at2[ord[i-1]] - at2[ord[i]]) != 0 ) ord[tmpn++] = ord[i];
	n = tmpn;
	int bot = 1,top = bot + 1;
	dq[bot] = ord[0]; dq[top] = ord[1];
	for(i = 2 ;i < n;i++) {
		while( bot < top && judgeout(dq[top-1] , dq[top] , ord[i]) ) top--;
		while( bot < top && judgeout(dq[bot+1] , dq[bot] , ord[i]) ) bot++;
		dq[++top] = ord[i];
	}
	while( bot < top && judgeout(dq[top-1] , dq[top] , dq[bot]) ) top--;
	while( bot < top && judgeout(dq[bot+1] , dq[bot] , dq[top]) ) bot++;
	dq[--bot] = dq[top];
	pg.n = 0;
	for(i = bot + 1;i <= top ; i++) //求相邻两条线的交点
		linecross(ls[dq[i-1]].a, ls[dq[i-1]].b, ls[dq[i]].a,ls[dq[i]].b, pg.p[pg.n++]);
}
inline void add(double a, double b, double c, double d) {//添加线段
	ls[lnum].a.x = a; ls[lnum].a.y = b;
	ls[lnum].b.x = c; ls[lnum].b.y = d;
	lnum++;
}
polygon pg;
int main() {
	int n, i;
	scanf("%d",&n);
	double a, b, c, d;
	lnum = 0;
	for(i = 0 ;i < n ; i++) {
		scanf("%lf%lf%lf%lf",&a,&b,&c,&d);
		add(a, b, c, d);
	}
	add(0, 0, INF, 0);
	add(INF, 0, INF, INF);
	add(INF, INF, 0, INF);
	add(0, INF, 0, 0);
	HalfPlaneIntersection(pg);
	double area = 0;
	for(i = 0;i < pg.n;i++)
		area += det(pg.p[i].x, pg.p[i].y, pg.p[(i + 1) % n].x, pg.p[(i + 1) % n].y);
	printf("%.1f\n", fabs(area) / 2.0);
	return 0;
}

