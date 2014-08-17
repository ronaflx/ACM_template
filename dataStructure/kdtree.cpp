#include <iostream>
#include <cstring>
#include <cassert>
#include <limits>
#include <vector>
using namespace std;

const int N = 400000;
static const long long pdist_max = numeric_limits<long long>::max();
static const long long pdist_min = numeric_limits<long long>::min();

struct Point {
  long long x, y;
  Point(long long x = 0, long long y = 0) : x(x), y(y) {}
};

inline int left_half(int n) { return n * 2; }
inline int right_half(int n) { return n * 2 + 1; }

static bool comp_x(const Point& a, const Point& b) { return a.x < b.x; }
static bool comp_y(const Point& a, const Point& b) { return a.y < b.y; }

static long long pdistance(const Point& a, const Point& b) {
  long long dx = a.x - b.x, dy = a.y - b.y;
  return dx * dx + dy * dy;
}

class KDTree {
 public:
  void Build(const vector<Point>& p) {
    points.assign(p.begin(), p.end());
    Build(0, points.size(), 1);
  }

  long long Query(const Point& x) const {
    return Query(0, points.size(), 1, x);
  }

 private:
  struct Boundary {
    long long lower_x, upper_x, lower_y, upper_y;
    Boundary()
        : lower_x(pdist_max),
          upper_x(pdist_min),
          lower_y(pdist_max),
          upper_y(pdist_min) {}
    void Update(const vector<Point>& x, int l, int r) {
      for (int i = l; i < r; i++) {
        lower_x = min(lower_x, x[i].x);
        upper_x = max(upper_x, x[i].x);
        lower_y = min(lower_y, x[i].y);
        upper_y = max(upper_y, x[i].y);
      }
    }

    long long XDist() const { return upper_x - lower_x; }
    long long YDist() const { return upper_y - lower_y; }
    long long Distance(const Point& p) const {
      if (p.x < lower_x) {
        if (p.y < lower_y)
          return pdistance(Point(lower_x, lower_y), p);
        else if (p.y > upper_y)
          return pdistance(Point(lower_x, upper_y), p);
        else
          return pdistance(Point(lower_x, p.y), p);
      } else if (p.x > upper_x) {
        if (p.y < lower_y)
          return pdistance(Point(upper_x, lower_y), p);
        else if (p.y > upper_y)
          return pdistance(Point(upper_x, upper_y), p);
        else
          return pdistance(Point(upper_x, p.y), p);
      } else {
        if (p.y < lower_y)
          return pdistance(Point(p.x, lower_y), p);
        else if (p.y > upper_y)
          return pdistance(Point(p.x, upper_y), p);
        else
          return 0;
      }
    }
  };
  Boundary boundary[N];
  vector<Point> points;

  void Build(int l, int r, int n) {
    boundary[n] = Boundary();
    boundary[n].Update(points, l, r);
    if (l + 1 == r) {
      return;
    }
    int mid = (l + r) / 2;
    if (boundary[n].XDist() > boundary[n].YDist()) {
      nth_element(points.begin() + l, points.begin() + mid, points.begin() + r,
                  comp_x);
    } else {
      nth_element(points.begin() + l, points.begin() + mid, points.begin() + r,
                  comp_y);
    }
    Build(l, mid, left_half(n));
    Build(mid, r, right_half(n));
  }
  // This will never return 0.
  long long Query(int l, int r, int n, const Point& p) const {
    assert(n < N);
    if (l + 1 == r) {
      long long d = pdistance(points[l], p);
      return d == 0 ? pdist_max : d;
    }
    int mid = (l + r) / 2;
    long long l_best = boundary[left_half(n)].Distance(p);
    long long r_best = boundary[right_half(n)].Distance(p);
    long long d = 0;
    if (l_best < r_best) {
      d = Query(l, mid, left_half(n), p);
      if (d > r_best) {
        d = min(d, Query(mid, r, right_half(n), p));
      }
    } else {
      d = Query(mid, r, right_half(n), p);
      if (d > l_best) {
        d = min(d, Query(l, mid, left_half(n), p));
      }
    }
    assert(d != 0);
    return d;
  }
};

KDTree tree;

int main() {
  int t, n;
  scanf("%d", &t);
  vector<Point> p;
  while (t--) {
    scanf("%d", &n);
    p.resize(n);
    for (int i = 0; i < n; i++) {
      scanf("%lld %lld", &p[i].x, &p[i].y);
    }
    tree.Build(p);
    for (int i = 0; i < n; i++) {
      printf("%lld\n", tree.Query(p[i]));
    }
  }
  return 0;
}
