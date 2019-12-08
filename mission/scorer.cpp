#include <bits/stdc++.h>
using namespace std;

int ac() {
  puts("AC");
  return 0;
}

int wa(string message) {
  fprintf(stderr, "%s\n", message.c_str());
  puts("WA");
  return 0;
}

typedef long long Integer;
typedef long double Number;

Number sqr(const Number &num) {
  return num * num;
}

struct Point {
  Number x;
  Number y;

  Point() {}

  Point(Number _x, Number _y) {
    x = _x;
    y = _y;
  }

  Point(const Point &other) {
    x = other.x;
    y = other.y;
  }

  bool operator==(const Point &other) const {
    return make_pair(x, y) == make_pair(other.x, other.y);
  }

  bool operator<(const Point &other) const {
    return make_pair(x, y) < make_pair(other.x, other.y);
  }
};

struct Line {
  Point st;
  Point nd;

  Line() {}

  Line(Point _st, Point _nd) {
    st = _st;
    nd = _nd;
  }

  Line(const Line &other) {
    st = other.st;
    nd = other.nd;
  }

  Number dx() const {
    return nd.x - st.x;
  }

  Number dy() const {
    return nd.y - st.y;
  }

  Number d() const {
    return sqrt(sqr(dx()) + sqr(dy()));
  }
};

struct Circle {
  Point o;
  Number r;

  Circle() {}

  Circle(Point _o, Number _r) {
    o = _o;
    r = _r;
  }

  Circle(const Circle &other) {
    o = other.o;
    r = other.r;
  }
};

Number distance(const Point &p1, const Point &p2) {
  return sqrt(sqr(p1.x - p2.x) + sqr(p1.y - p2.y));
}

const Number OUT_OF_BOUND_EPS = 1e-6;
const Number CONSECUTIVE_POINTS_EPS = 1e-6;
const Number INTERSECTING_CIRCLE_EPS = 1e-6;

int main(int argc, char *argv[])
{
  FILE *in = fopen(argv[1], "r");
  FILE *out = fopen(argv[2], "r");  // Judge's
  FILE *con = fopen(argv[3], "r");

  int N;
  Integer xl, yl, xr, yr, xs, ys, xt, yt;
  fscanf(in, "%d %lld %lld %lld %lld", &N, &xl, &yl, &xr, &yr);
  fscanf(in, "%lld %lld", &xs, &ys);
  fscanf(in, "%lld %lld", &xt, &yt);

  Point L = Point(Number(xl), Number(yl));
  Point R = Point(Number(xr), Number(yr));
  Point S = Point(Number(xs), Number(ys));
  Point T = Point(Number(xt), Number(yt));

  vector<Circle> circles;
  for (int i = 0; i < N; ++i) {
    Integer x, y, r;
    fscanf(in, "%lld %lld %lld", &x, &y, &r);
    circles.push_back(Circle(Point(x, y), r));
  }

  int K;
  if (fscanf(con, "%d", &K) != 1) {
    return wa("Unexpected EOF.");
  }

  if (K < 0 || K > 1000) {
    return wa("K is out of the allowed bound."); 
  }

  vector<Point> P = {S};  
  for (int i = 0; i < K; ++i) {
    Number x, y;
    if (fscanf(con, "%Lf %Lf", &x, &y) != 2) {
      return wa("Unexpected EOF.");
    }

    if (x + OUT_OF_BOUND_EPS < L.x || x - OUT_OF_BOUND_EPS > R.x) {
      return wa("Point out of bound.");
    }
    if (y + OUT_OF_BOUND_EPS < L.y || y - OUT_OF_BOUND_EPS > R.y) {
      return wa("Point out of bound.");
    }
    P.push_back(Point(x, y));
  }
  P.push_back(T);

  for (int i = 0; i < P.size(); ++i) {
    for (int j = i + 1; j < P.size(); ++j) {
      if (fabs(P[i].x - P[j].x) <= CONSECUTIVE_POINTS_EPS &&
          fabs(P[i].y - P[j].y) <= CONSECUTIVE_POINTS_EPS) {
        return wa("Two points considered the same.");
      }
    }
  }

  for (int i = 0; i < P.size() - 1; ++i) {
    const Line l = Line(P[i], P[i + 1]);

    const Number a = l.dy();
    const Number b = -l.dx();
    const Number c = P[i].y * l.dx() - P[i].x * l.dy();

    for (const Circle &circle : circles) {
      // Point on the line closest to circle origin
      const Number x =
          (b * (b * circle.o.x - a * circle.o.y) - a * c) / (sqr(a) + sqr(b));
      const Number y =
          (a * (-b * circle.o.x + a * circle.o.y) - b * c) / (sqr(a) + sqr(b));
      const Point p = Point(x, y);

      Number minimum_distance;
      if (min(l.st.x, l.nd.x) <= x + 1E-9 && x <= max(l.st.x, l.nd.x) + 1E-9 &&
          min(l.st.y, l.nd.y) <= y + 1E-9 && y <= max(l.st.y, l.nd.y) + 1E-9) {
        minimum_distance = distance(p, circle.o);
      } else {
        minimum_distance = min(
            distance(P[i], circle.o), distance(P[i + 1], circle.o));
      }

      if (minimum_distance < circle.r - INTERSECTING_CIRCLE_EPS) {
        return wa("Path intersects a circle.");
      }
    }
  }

  fscanf(con, "\n");
  char dummy;
  if (fscanf(con, "%c", &dummy) == 1) {
    return wa("Expected EOF.");
  }

  return ac();
}
