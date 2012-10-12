const double EPS = 1e-8;
const double PI = acos(-1.0);
const double INF = 1e100;
struct Point {
        double x, y;
        Point(double xx = 0, double yy = 0)
        {x = xx, y = yy;}
        bool operator <(const Point a) const
        {return y == a.y ? x < a.x : y < a.y;}
        friend ostream& operator << (ostream& out, Point a) {
                out << "(" << a.x << " " << a.y << ")";
                return out;
        }
};
/* ********************************************
 * 距离公式
 * *******************************************/ 
double dist(double x1, double y1, double x2, double y2) {
        return sqrt(pow(x1 - x2, 2.0) + pow(y1 - y2, 2.0));
}
double sphereDist(double x1, double y1, double x2, double y2, double R = 1) {
        //longitude x and latitude y
        //z[i] = sin(lat[i]*PI/180);
        //x[i] = cos(lng[i]*PI/180) * cos(lat[i]*PI/180);
        //y[i] = sin(lng[i]*PI/180) * cos(lat[i]*PI/180);
        //dist = x[i]*x[j] + y[i]*y[j] + z[i]*z[j]
        x1 /= 180;y1 /= 180;x2 /= 180;y2 /= 180;
        x1 *= PI;y1 *= PI;x2 *= PI;y2 *= PI;
        return R * acos(sin(y1) * sin(y2) + cos(y1) * cos(y2) * cos(x1 - x2));
}

/* ********************************************
 * 基础应用
 * *******************************************/ 
int dblcmp(double x) {
        if(fabs(x) < EPS) return 0;
        return x < 0 ? -1: 1;
}
double det(double x1, double y1, double x2, double y2) {
        return x1 * y2 - x2 * y1;
}
double cross(Point a, Point b, Point c)/*ab x ac*/ {
        return det(b.x - a.x, b.y - a.y, c.x - a.x, c.y - a.y);
}
double dotbet(double x1, double y1, double x2, double y2) {
        return x1 * x2 + y1 * y2;
}
double dot(Point a, Point b, Point c) {
        return dotbet(b.x - a.x, b.y - a.y, c.x - a.x, c.y - a.y);
}
int betweencmp(Point a, Point b, Point c) {
        return dblcmp(dot(a, b, c));
}
/* ********************************************
 * 直线线段相交模板
 * *******************************************/ 
//ab 与 cd是否规范相交
bool segcrosssimple(Point a, Point b, Point c, Point d) {
        return (dblcmp(cross(a, c, d)) ^ dblcmp(cross(b, c, d))) == -2 &&
                (dblcmp(cross(c, a, b)) ^ dblcmp(cross(d, a, b))) == -2;
}
//ab cd是否相交，规范相交返回交点
int segcross(Point a, Point b, Point c, Point d, Point& p) {
        double s1, s2, s3, s4;
        int d1 = dblcmp(s1 = cross(a, b, c));
        int d2 = dblcmp(s2 = cross(a, b, d));
        int d3 = dblcmp(s3 = cross(c, d, a));
        int d4 = dblcmp(s4 = cross(c, d, b));
        if((d1 ^ d2) == -2 && (d3 ^ d4) == -2) {
                p.x = (c.x * s2 - d.x * s1) / (s2 - s1);
                p.y = (c.y * s2 - d.y * s1) / (s2 - s1);
                return 1;
        }
        if(d1 == 0 && betweencmp(c, a, b) <= 0 ||
           d2 == 0 && betweencmp(d, a, b) <= 0 ||
           d3 == 0 && betweencmp(a, c, d) <= 0 ||
           d4 == 0 && betweencmp(b, c, d) <= 0) return 2;
        return 0;
}
//直线ab于线段cd相交，返回相交交点
int linecrossseg(Point a, Point b, Point c, Point d, Point& temp) {
        double s1, s2;
        int d1, d2;
        d1 = dblcmp(s1 = cross(a, b, c));
        d2 = dblcmp(s2 = cross(a, b, d));
        if (d1 * d2 < 0) {
                temp.x = (c.x * s2 - d.x * s1) / (s2 - s1);
                temp.y = (c.y * s2 - d.y * s1) / (s2 - s1);
                return 1;
        }
	//交于端点
	if (d1 * d2 == 0) {
		if(d2 == 0) temp = d;
		else  temp = c;
		return 2;
	}
	return 0;
}
//直线ab与直线cd是否相交，相交返回交点
bool linecross(Point a, Point b, Point c, Point d, Point& temp) {
	if((b.x - a.x) * (d.y - c.y) == (d.x - c.x) * (b.y - a.y)) return false;
	double s1, s2;
	int d1, d2;
	d1 = dblcmp(s1 = cross(a, b, c));
	d2 = dblcmp(s2 = cross(a, b, d));
	temp.x = (c.x * s2 - d.x * s1) / (s2 - s1);
	temp.y = (c.y * s2 - d.y * s1) / (s2 - s1);
	return true;
}

/* ********************************************
 * 凸包模板 
 * *******************************************/ 
void ConvexHull(Point* pts, Point* stk, int n, int &top)/*p[0] != [n - 1] */{
        sort(pts, pts + n);
        top = -1;
        stk[++top] = pts[0];
        stk[++top] = pts[1];
        for(int i = 2;i < n;i++) {
                while(top >= 1 && dblcmp(cross(stk[top - 1], stk[top], pts[i])) <= 0)
                        top--;
                stk[++top] = pts[i];
        }
        int now = top;
        for(int i = n - 2;i >= 0;i--) {
                while(top >= now + 1 && dblcmp(cross(stk[top - 1], stk[top], pts[i])) <= 0)
                        top--;
                stk[++top] = pts[i];
        }
}
/* ********************************************
 * 旋转卡壳专用
 * *******************************************/ 
double rotating_calipers_longest(Point* p, int n) {
        double res = 0;
        p[n] = p[0];
        for(int i = 0, j = 1; i < n; i ++) {
                while(dblcmp(cross(p[i], p[i + 1], p[j]) - cross(p[i], p[i + 1], p[(j + 1) %n])) < 0)
                        j = (j + 1) % n;
                res = max(res, fabs(cross(p[i], p[i + 1], p[j])));
        }
        return res;
}
double rotating_calipers_triangle(Point p[],int n) {
        int i, j = 1, q = 2;
        p[n] = p[0];
        p[n+1] = p[1];
        p[n+2] = p[2];
        double temp, ans = 0;
        for (i = 0; i < n; i++) {
                while (cross(p[i],p[j],p[q+1]) - (temp = cross(p[i],p[j],p[q])) > EPS)
                        q = (q + 1) % n;
                ans = max(ans, temp);
                while (cross(p[i],p[j+1],p[q]) - (temp = cross(p[i],p[j],p[q])) > EPS)
                        j = (j + 1) % n;
                ans = max(ans, temp);
        }
        return ans;
}

/* ********************************************
 * 多边形重心
 * *******************************************/ 
Point barycenter(Point a, Point b, Point c) {
        Point tmp;
        linecross(Point((a.x + b.x) / 2, (a.y + b.y) / 2), c,
                        Point((a.x + c.x) / 2, (a.y + c.y) / 2), b, tmp);
        return tmp;
}
Point barycenter(Point p[], int n) {
        Point ret, t;
        double t1 = 0, t2;
        ret.x = ret.y = 0;
        for (int i = 1; i < n - 1; i++)
                if (fabs(t2 = cross(p[0],p[i],p[i + 1]))> EPS) {
                        t = barycenter(p[0],p[i],p[i + 1]);
                        ret.x += t.x*t2;
                        ret.y += t.y*t2;
                        t1 += t2;
                }
        if (fabs(t1) > EPS) ret.x /= t1,ret.y /= t1;
        return ret;
}
//c在ab上都的垂足
Point verticalfoot(Point a, Point b, Point c) {
        Point tmp(c.x - a.y + b.y, c.y + a.x - b.x), ans;
        linecross(a, b, c, tmp, ans);
        return ans;
}
