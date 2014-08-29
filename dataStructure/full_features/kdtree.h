#include <tuple>
#include <vector>
#include <limits>
#include <iterator>
#include <algorithm>
#include <cassert>

// TODO(ronaflx): Is there any other implementation in stead of compile-time
// recursion expansion.
// benchmark: This implementation is slower than the naive version.
// naive version:     this version
// real 0m3.735s      real  0m7.778s
// user 0m3.665s      user 0m7.710s
// sys  0m0.054s      sys  0m0.057s
template <typename RetType, typename... Types>
class KDTree {
 public:
  typedef std::tuple<Types...> key_type;
  typedef RetType value_type;
  const static int dimension_size = sizeof...(Types);

  // Build KDTree
  template <typename Iter, typename Conv>
  void Build(Iter first, Iter second, Conv conv);
  template <typename Iter>
  void Build(Iter first, Iter second);

  // Query the nearest the point.
  value_type Query(const key_type& p);
  value_type QueryWithoutSame(const key_type& p);

 private:
  // Helper tags.
  template <size_t I>
  struct Tag {
    static const size_t Id = I;
  };

  // Internal class.
  struct Boundary {
    key_type lower_boundary, upper_boundary;

    Boundary()
        : lower_boundary(std::numeric_limits<Types>::max()...),
          upper_boundary(std::numeric_limits<Types>::min()...) {}

    template <typename Iter>
    void Update(Iter first, Iter last) {
      Update(first, last, Tag<0>());
    }

   private:
    template <typename Iter>
    void Update(Iter, Iter, Tag<dimension_size>) {
      return;
    }

    template <typename Iter, size_t I>
    void Update(Iter first, Iter last, Tag<I>) {
      Update(first, last, Tag<I + 1>());
      while (first != last) {
        typedef typename std::tuple_element<I, key_type>::type element_type;
        element_type& lower_ref = std::get<I>(lower_boundary);
        element_type& upper_ref = std::get<I>(upper_boundary);
        element_type& value = std::get<I>(*first);
        lower_ref = std::min(lower_ref, value);
        upper_ref = std::max(upper_ref, value);
        first++;
      }
    }
  };

  // Member
  std::vector<key_type> points_;
  std::vector<Boundary> boundary_;

  // TODO(ronaflx): remove this function.
  struct Point {
    long long x, y;
    Point(long long x = 0, long long y = 0) : x(x), y(y) {}
  };

  static long long pdistance(const Point& a, const Point& b) {
    long long dx = a.x - b.x, dy = a.y - b.y;
    return dx * dx + dy * dy;
  }

  template <typename Iter>
  void BuildInternal(Iter first, Iter second, int node);

  value_type QueryInternal(int l, int r, int n, const key_type& p,
                           bool with_same) const;

  // The Distance between two points in the Ith dimession.
  // Time complexity: O(1)
  template <size_t I = 0>
  value_type DimensionDistance(const key_type& one,
                               const key_type& other) const;

  // The Euclidean Distance between two points.
  // Time complexity: O(d)
  value_type EuclideanDistance(const key_type& one, const key_type& other,
                               Tag<dimension_size>) const;
  template <size_t I>
  value_type EuclideanDistance(const key_type& one, const key_type& other,
                               Tag<I> t = Tag<I>()) const;

  // Return true if a point is inside a boudary regardless of Pth dimension.
  // Time complexity: O(d)
  template <size_t P>
  bool Inside(const key_type& point, const Boundary& b, Tag<dimension_size>,
              Tag<P>) const;
  template <size_t I, size_t P>
  bool Inside(const key_type& point, const Boundary& b, Tag<I> t,
              Tag<P> p) const;

  // Return the distance between the point and the endpoint of the space.
  // Time complexity: O(d)
  value_type Point2EndPointDistance(const key_type& point, const Boundary& b,
                                    Tag<dimension_size>) const;
  template <size_t T>
  value_type Point2EndPointDistance(const key_type& point, const Boundary& b,
                                    Tag<T>) const;

  // Return the distance between the point and Ith (2) planes if the projection
  // of the point is inside the Ith plane, max() otherwise.
  // Time complexity: O(d^2) TODO(ronaflx): This is not effective
  value_type Point2PlaneDistance(const key_type& one, const Boundary& b,
                                 Tag<dimension_size>) const;
  template <size_t I>
  value_type Point2PlaneDistance(const key_type& one, const Boundary& b,
                                 Tag<I>) const;

  // Return the distance between the point to the sapce.
  // Time complexity: O(d^2) TODO(ronaflx): This is not effective
  value_type Point2SpaceDistance(const key_type& one, const Boundary& b) const;

  // Return true if the points has already been be sliced by one dimension.
  template <typename Iter>
  bool Slice(Iter, Iter, Iter, Tag<dimension_size>, const Boundary& b,
             value_type* val);
  template <typename Iter, size_t I>
  bool Slice(Iter first, Iter mid, Iter last, Tag<I> t, const Boundary& b,
             value_type* val);

  template <size_t I>
  struct CompareIthElement {
    bool operator()(const key_type& one, const key_type& other) {
      return std::get<I>(one) < std::get<I>(other);
    }
  };
};

template <typename RetType, typename... Types>
template <typename Iter, typename Conv>
void KDTree<RetType, Types...>::Build(Iter first, Iter second, Conv conv) {
  points_.clear();
  std::transform(first, second, std::back_inserter(points_), conv);
  boundary_.clear();
  boundary_.resize(points_.size() * 4);
  BuildInternal(points_.begin(), points_.end(), 1);
}

template <typename RetType, typename... Types>
template <typename Iter>
void KDTree<RetType, Types...>::Build(Iter first, Iter last) {
  points_.assign(first, last);
  boundary_.resize(points_.size() * 4);
  BuildInternal(points_.begin(), points_.end(), 1);
}

template <typename RetType, typename... Types>
typename KDTree<RetType, Types...>::value_type KDTree<RetType, Types...>::Query(
    const key_type& p) {
  return QueryInternal(0, points_.size(), 1, p, true);
}

template <typename RetType, typename... Types>
typename KDTree<RetType, Types...>::value_type
KDTree<RetType, Types...>::QueryWithoutSame(const key_type& p) {
  return QueryInternal(0, points_.size(), 1, p, false);
}

template <typename RetType, typename... Types>
template <typename Iter>
void KDTree<RetType, Types...>::BuildInternal(Iter first, Iter last, int node) {
  boundary_[node].Update(first, last);
  if (first + 1 == last) {
    return;
  }
  auto half_ptr_dist = distance(first, last) / 2;
  Iter mid = first + half_ptr_dist;
  value_type val = value_type();
  Slice(first, mid, last, Tag<0>(), boundary_[node], &val);
  BuildInternal(first, mid, node * 2);
  BuildInternal(mid, last, node * 2 + 1);
}

template <typename RetType, typename... Types>
typename KDTree<RetType, Types...>::value_type
KDTree<RetType, Types...>::QueryInternal(int l, int r, int n, const key_type& p,
                                         bool with_same) const {
  if (l + 1 == r) {
    value_type dis = EuclideanDistance(p, points_[l], Tag<0>());
    if (with_same) {
      return dis;
    } else {
      return dis == value_type() ? std::numeric_limits<value_type>::max() : dis;
    }
  }
  int mid = (l + r) / 2;

  value_type l_best = Point2SpaceDistance(p, boundary_[n * 2]);
  value_type r_best = Point2SpaceDistance(p, boundary_[n * 2 + 1]);
  value_type d;
  if (l_best < r_best) {
    d = QueryInternal(l, mid, n * 2, p, with_same);
    if (d > r_best) {
      d = std::min(d, QueryInternal(mid, r, n * 2 + 1, p, with_same));
    }
  } else {
    d = QueryInternal(mid, r, n * 2 + 1, p, with_same);
    if (d > l_best) {
      d = std::min(d, QueryInternal(l, mid, n * 2, p, with_same));
    }
  }
  return d;
}

template <typename RetType, typename... Types>
template <size_t I>
typename KDTree<RetType, Types...>::value_type
KDTree<RetType, Types...>::DimensionDistance(const key_type& one,
                                             const key_type& other) const {
  const value_type first = std::get<I>(one);
  const value_type second = std::get<I>(other);
  return first < second ? second - first : first - second;
}

template <typename RetType, typename... Types>
typename KDTree<RetType, Types...>::value_type
KDTree<RetType, Types...>::EuclideanDistance(const key_type&, const key_type&,
                                             Tag<dimension_size>) const {
  return value_type();
}

template <typename RetType, typename... Types>
template <size_t I>
typename KDTree<RetType, Types...>::value_type KDTree<
    RetType, Types...>::EuclideanDistance(const key_type& one,
                                          const key_type& other, Tag<I>) const {
  value_type dis = DimensionDistance<I>(one, other);
  return dis * dis + EuclideanDistance(one, other, Tag<I + 1>());
}

template <typename RetType, typename... Types>
template <size_t P>
bool KDTree<RetType, Types...>::Inside(const key_type&, const Boundary&,
                                       Tag<dimension_size>, Tag<P>) const {
  return true;
}

template <typename RetType, typename... Types>
template <size_t I, size_t P>
bool KDTree<RetType, Types...>::Inside(const key_type& point, const Boundary& b,
                                       Tag<I>, Tag<P>) const {
  typename std::tuple_element<I, key_type>::type value = std::get<I>(point);
  if (I == P || (std::get<I>(b.lower_boundary) <= value &&
                 value <= std::get<I>(b.upper_boundary))) {
    return Inside(point, b, Tag<I + 1>(), Tag<P>());
  } else {
    return false;
  }
}

template <typename RetType, typename... Types>
typename KDTree<RetType, Types...>::value_type KDTree<
    RetType, Types...>::Point2EndPointDistance(const key_type&, const Boundary&,
                                               Tag<dimension_size>) const {
  return value_type();
}

template <typename RetType, typename... Types>
template <size_t I>
typename KDTree<RetType, Types...>::value_type
KDTree<RetType, Types...>::Point2EndPointDistance(const key_type& point,
                                                  const Boundary& b,
                                                  Tag<I>) const {
  value_type dis = std::min(DimensionDistance<I>(point, b.upper_boundary),
                            DimensionDistance<I>(point, b.lower_boundary));
  return dis * dis + Point2EndPointDistance(point, b, Tag<I + 1>());
}

template <typename RetType, typename... Types>
typename KDTree<RetType, Types...>::value_type
KDTree<RetType, Types...>::Point2PlaneDistance(const key_type&, const Boundary&,
                                               Tag<dimension_size>) const {
  return std::numeric_limits<value_type>::max();
}

template <typename RetType, typename... Types>
template <size_t I>
typename KDTree<RetType, Types...>::value_type KDTree<
    RetType, Types...>::Point2PlaneDistance(const key_type& point,
                                            const Boundary& b, Tag<I>) const {
  value_type value = Point2PlaneDistance(point, b, Tag<I + 1>());
  if (Inside(point, b, Tag<0>(), Tag<I>())) {
    value_type upper_dis = DimensionDistance<I>(point, b.upper_boundary);
    value_type lower_dis = DimensionDistance<I>(point, b.lower_boundary);
    return std::min(value,
                    std::min(upper_dis * upper_dis, lower_dis * lower_dis));
  } else {
    return value;
  }
}

template <typename RetType, typename... Types>
typename KDTree<RetType, Types...>::value_type
KDTree<RetType, Types...>::Point2SpaceDistance(const key_type& one,
                                               const Boundary& b) const {
  if (Inside(one, b, Tag<0>(), Tag<dimension_size>())) {
    return 0;
  } else {
    return std::min(Point2EndPointDistance(one, b, Tag<0>()),
                    Point2PlaneDistance(one, b, Tag<0>()));
  }
}

template <typename RetType, typename... Types>
template <typename Iter>
bool KDTree<RetType, Types...>::Slice(Iter, Iter, Iter, Tag<dimension_size>,
                                      const Boundary&, value_type*) {
  return false;
}

template <typename RetType, typename... Types>
template <typename Iter, size_t I>
bool KDTree<RetType, Types...>::Slice(Iter first, Iter mid, Iter last, Tag<I>,
                                      const Boundary& b, value_type* val) {
  value_type dis = DimensionDistance<I>(b.lower_boundary, b.upper_boundary);
  if (*val < dis) {
    *val = dis;
  }
  if (!Slice(first, mid, last, Tag<I + 1>(), b, val)) {
    if (dis == *val) {
      nth_element(first, mid, last, CompareIthElement<I>());
      return true;
    } else {
      return false;
    }
  }
  return false;
}

#include <cstdio>
#include <vector>
#include <iostream>
#include <tuple>
using namespace std;
int main() {
  int t, n;
  scanf("%d", &t);
  while (t--) {
    scanf("%d", &n);
    vector<tuple<long long, long long>> p(n);
    KDTree<long long, long long, long long> tree;
    for (int i = 0; i < n; i++) {
      scanf("%lld %lld", &get<0>(p[i]), &get<1>(p[i]));
    }
    tree.Build(p.begin(), p.end());
    for (int i = 0; i < n; i++) {
      printf("%lld\n", tree.QueryWithoutSame(p[i]));
    }
  }
  return 0;
}
