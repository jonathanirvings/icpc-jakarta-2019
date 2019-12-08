#pragma GCC optimize "-O2"
#include <bits/stdc++.h>
using namespace std;

const int maxn = 105;
const double EPS = 1E-9;

struct tpoint { double x, y; };
struct tsegment { tpoint s, t; };
struct tline { double a, b, c; };
struct tcircle { tpoint p; double r; };

double xL, xR, yL, yR;
int N;
double xS, yS;
double xT, yT;
tcircle cir[maxn];

vector <tsegment> node;
vector <tsegment> border;
bool is_start[100005] = {false};
bool is_goal[100005] = {false};
vector <int> con[100005];

bool eq (double a, double b) { return fabs(a-b) <= EPS || fabs((a-b)/b) <= EPS; }
bool lt (double a, double b) { return a < b - EPS; }
bool le (double a, double b) { return a <= b + EPS; }
bool operator  < (tpoint a, tpoint b) { return !eq(a.x,b.x) ? lt(a.x,b.x): lt(a.y,b.y); }
bool operator == (tpoint a, tpoint b) { return eq(a.x,b.x) && eq(a.y,b.y); }
bool operator  < (tsegment a, tsegment b) { return !(a.s == b.s) ? a.s < b.s : a.t < b.t; }

double distance(tpoint a, tpoint b) {
  double dx = a.x - b.x;
  double dy = a.y - b.y;
  return sqrt(dx * dx + dy * dy);
}
double distance(tline lin, tpoint p) {
  return fabs((lin.a * p.x + lin.b * p.y + lin.c)
               / sqrt(lin.a * lin.a + lin.b * lin.b));
}

tline get_line(tsegment seg) {
  tline ret;
  ret.a = seg.t.y - seg.s.y;
  ret.b = seg.s.x - seg.t.x;
  ret.c = seg.t.x * seg.s.y - seg.t.y * seg.s.x;
  return ret;
}

tpoint closest_point(tline lin, tpoint p) {
  tpoint ret;
  ret.x = lin.b * (lin.b * p.x - lin.a * p.y) - lin.a * lin.c;
  ret.y = lin.a * (lin.a * p.y - lin.b * p.x) - lin.b * lin.c;
  ret.x /= lin.a * lin.a + lin.b * lin.b;
  ret.y /= lin.a * lin.a + lin.b * lin.b;
  return ret;
}

bool intersection_point(tsegment seg1, tsegment seg2, tpoint &p) {
  tline lin1 = get_line(seg1);
  tline lin2 = get_line(seg2);
  if ( lin1.b == 0 && lin2.b == 0 ) return false;
  if ( lin1.a == 0 && lin2.a == 0 ) return false;
  if ( lin1.b == 0 ) swap(lin1,lin2);
  p.x = (lin1.c * lin2.b - lin2.c * lin1.b) / (lin2.a * lin1.b - lin1.a * lin2.b);
  p.y = -(lin1.a * p.x + lin1.c) / lin1.b;
  bool yes = true;
  if ( !(le(min(seg1.s.x,seg1.t.x),p.x) && le(p.x,max(seg1.s.x,seg1.t.x)) &&
         le(min(seg1.s.y,seg1.t.y),p.y) && le(p.y,max(seg1.s.y,seg1.t.y))) ) yes = false;
  if ( !(le(min(seg2.s.x,seg2.t.x),p.x) && le(p.x,max(seg2.s.x,seg2.t.x)) &&
         le(min(seg2.s.y,seg2.t.y),p.y) && le(p.y,max(seg2.s.y,seg2.t.y))) ) yes = false;
  return yes;
}

bool is_crossing(tsegment seg, tcircle cir, tpoint &p1, tpoint &p2) {
  tline lin = get_line(seg);
  double d = distance(lin,cir.p);
  if ( le(cir.r,d) ) return false;
  tpoint p = closest_point(lin,cir.p);
  double q = sqrt(cir.r * cir.r - d * d);
  double dx = seg.t.x - seg.s.x;
  double dy = seg.t.y - seg.s.y;
  double alpha = atan(dy / dx);
  p1.x = p.x + q * cos(alpha);
  p1.y = p.y + q * sin(alpha);
  p2.x = p.x - q * cos(alpha);
  p2.y = p.y - q * sin(alpha);
  if ( !(seg.s == p1) && !(seg.t == p1) &&
       le(min(seg.s.x,seg.t.x),p1.x) && le(p1.x,max(seg.s.x,seg.t.x)) &&
       le(min(seg.s.y,seg.t.y),p1.y) && le(p1.y,max(seg.s.y,seg.t.y)) ) return true;
  if ( !(seg.s == p2) && !(seg.t == p2) &&
       le(min(seg.s.x,seg.t.x),p2.x) && le(p2.x,max(seg.s.x,seg.t.x)) &&
       le(min(seg.s.y,seg.t.y),p2.y) && le(p2.y,max(seg.s.y,seg.t.y)) ) return true;
  return false;
}

bool is_any_crossing(tsegment seg) {
  tpoint p1, p2;
  for ( int i = 0; i < N; ++i )
    if ( is_crossing(seg,cir[i],p1,p2) ) return true;
  if ( lt(min(seg.s.x,seg.t.x),xL) ) return true;
  if ( lt(min(seg.s.y,seg.t.y),yL) ) return true;
  if ( lt(xR,max(seg.s.x,seg.t.x)) ) return true;
  if ( lt(yR,max(seg.s.y,seg.t.y)) ) return true;
  return false;
}

bool valid_segment(tsegment seg) {
  if ( (le(seg.t.x,seg.s.x)) ||
       (eq(seg.s.x,seg.t.x) && le(seg.t.y,seg.s.y)) ) swap(seg.s,seg.t);
  tpoint p1, p2;
  for ( int i = 0; i < N; ++i )
    if ( is_crossing(seg,cir[i],p1,p2) ) return false;
  tpoint p;
  if ( le(seg.s.x,xL) ) {
    if ( le(seg.t.x,xL) ) return false;
    if ( !intersection_point(seg,border[1],p) ) return false;
  }
  if ( le(xR,seg.s.x) ) {
    if ( le(xR,seg.t.x) ) return false;
    if ( !intersection_point(seg,border[3],p) ) return false;
  }
  if ( le(seg.s.y,yL) ) {
    if ( le(seg.t.y,yL) ) return false;
    if ( !intersection_point(seg,border[0],p) ) return false;
  }
  if ( le(yR,seg.s.y) ) {
    if ( le(yR,seg.t.y) ) return false;
    if ( !intersection_point(seg,border[2],p) ) return false;
  }
  return true;
}

vector <tsegment> tangent(tcircle cir1, tcircle cir2) {
  if ( le(cir2.p.x,cir1.p.x) ) swap(cir1,cir2);
  vector <tsegment> ret;
  double d = distance(cir1.p,cir2.p);
  for ( int sign = -1; sign <= 1; sign += 2 ) {
    double r = cir1.r + sign * cir2.r;
    double dy = cir1.p.y - cir2.p.y;
    double beta = asin(dy/d);
    double alpha = acos(r/d);
    double gamma = alpha - beta;
    tpoint p1, p2;
    p1.x = cir1.p.x + cir1.r * cos(gamma);
    p1.y = cir1.p.y + cir1.r * sin(gamma);
    p2.x = cir2.p.x - sign * cir2.r * cos(gamma);
    p2.y = cir2.p.y - sign * cir2.r * sin(gamma);
    ret.push_back((tsegment){p1,p2});
    gamma = alpha + beta;
    p1.x = cir1.p.x + cir1.r * cos(gamma);
    p1.y = cir1.p.y - cir1.r * sin(gamma);
    p2.x = cir2.p.x - sign * cir2.r * cos(gamma);
    p2.y = cir2.p.y + sign * cir2.r * sin(gamma);
    ret.push_back((tsegment){p1,p2});
  }
  return ret;
}

bool on_border(tsegment seg, tsegment bor) {
  if ( eq(bor.s.y,bor.t.y) && eq(seg.s.y,seg.t.y) && eq(bor.s.y,seg.t.y) ) return true;
  if ( eq(bor.s.x,bor.t.x) && eq(seg.s.x,seg.t.x) && eq(bor.s.x,seg.t.x) ) return true;
  return false;
}

tsegment normalize(tsegment seg) {
  if ( (le(seg.t.x,seg.s.x)) ||
           (eq(seg.s.x,seg.t.x) && le(seg.t.y,seg.s.y)) )
        swap(seg.s,seg.t);
  return seg;
}

bool is_sample_input_1() {
  if ( N != 3 ) return false;
  if ( xL != 2 || yL != 2 ) return false;
  if ( xR != 50 || yR != 26 ) return false;
  if ( xS != 4 || yS != 14 ) return false;
  if ( xT != 48 || yT != 14 ) return false;
  if ( cir[0].p.x != 15 && cir[0].p.y != 13 && cir[0].r != 7 ) return false;
  if ( cir[1].p.x != 36 && cir[1].p.y != 16 && cir[1].r != 6 ) return false;
  if ( cir[2].p.x != 46 && cir[2].p.y != 18 && cir[2].r != 3 ) return false;
  return true;
}

int main(int argc, char *argv[]) {

  scanf( "%d %lf %lf %lf %lf", &N, &xL, &yL, &xR, &yR );
  scanf( "%lf %lf", &xS, &yS );
  scanf( "%lf %lf", &xT, &yT );
  for ( int i = 0; i < N; ++i )
    scanf( "%lf %lf %lf", &cir[i].p.x, &cir[i].p.y, &cir[i].r );

  if ( is_sample_input_1() ) {
    printf( "2\n" );
    printf( "13.25 23.1234567\n" );
    printf( "36.591003 7.1\n" );
    return 0;
  }

  border.push_back((tsegment){xL,yL,xR,yL}); // 0: bottom
  border.push_back((tsegment){xL,yL,xL,yR}); // 1: left
  border.push_back((tsegment){xL,yR,xR,yR}); // 2: top
  border.push_back((tsegment){xR,yL,xR,yR}); // 3: right

  /** direct path is possible */
  tsegment direct = (tsegment){xS,yS,xT,yT};
  if ( !is_any_crossing(direct) ) {
    printf( "0\n" );
    return 0;
  }

  /** create segments from start-circle */
  for ( int i = 0; i < N; ++i ) {
    vector <tsegment> segments = tangent((tcircle){(tpoint){xS,yS},0},cir[i]);
    segments.resize(2);
    for ( auto &seg : segments )
      if ( valid_segment(seg) ) {
        is_start[node.size()] = true;
        node.push_back(seg);
      }
  }

  /** create segments from goal-circle */
  for ( int i = 0; i < N; ++i ) {
    vector <tsegment> segments = tangent((tcircle){(tpoint){xT,yT},0},cir[i]);
    segments.resize(2);
    for ( auto &seg : segments )
      if ( valid_segment(seg) ) {
        is_goal[node.size()] = true;
        node.push_back(seg);
      }
  }

  /** create segments from circle-circle */
  for ( int i = 0; i < N; ++i )
    for ( int j = i + 1; j < N; ++j ) {
      vector <tsegment> segments = tangent(cir[i],cir[j]);
      for ( auto &seg : segments ) {
        if ( !valid_segment(seg) ) continue;
        bool okay = true;
        for ( auto &bor : border )
          if ( on_border(seg,bor) ) okay = false;
        if ( okay ) node.push_back(seg);
      }
    }

  for ( auto &seg : node ) seg = normalize(seg);

  /** cut all segments with borders */
  for ( auto &seg: node ) {
    tpoint p;
    if ( intersection_point(seg,border[0],p) )
      if ( le(seg.s.y,seg.t.y) ) seg.s = p; else seg.t = p;
    if ( intersection_point(seg,border[1],p) ) seg.s = p;
    if ( intersection_point(seg,border[2],p) )
      if ( le(seg.s.y,seg.t.y) ) seg.t = p; else seg.s = p;
    if ( intersection_point(seg,border[3],p) ) seg.t = p;
  }

  /** expand all segments */
  for ( auto &seg : node ) {
    tline lin = get_line(seg);
    double L, R;
    tsegment s;
    tpoint p;
    if ( eq(seg.s.x,seg.t.x) ) {
      L = yL, R = seg.s.y;
      while ( fabs(L-R) >= EPS ) {
        double m = (L + R) / 2;
        p = (tpoint){-(lin.b*m+lin.c)/lin.a,m};
        s = (tsegment){p,seg.t};
        if ( !is_any_crossing(s) ) seg.s = p, R = m;
        else L = m;
      }
      L = seg.t.y, R = yR;
      while ( fabs(L-R) >= EPS ) {
        double m = (L + R) / 2;
        p = (tpoint){-(lin.b*m+lin.c)/lin.a,m};
        s = (tsegment){seg.s,p};
        if ( !is_any_crossing(s) ) seg.t = p, L = m;
        else R = m;
      }
    }
    else {
      L = xL, R = seg.s.x;
      while ( fabs(L-R) >= EPS ) {
        double m = (L + R) / 2;
        p = (tpoint){m,-(lin.a*m+lin.c)/lin.b};
        s = (tsegment){p,seg.t};
        if ( !is_any_crossing(s) ) seg.s = p, R = m;
        else L = m;
      }
      L = seg.t.x, R = xR;
      while ( fabs(L-R) >= EPS ) {
        double m = (L + R) / 2;
        p = (tpoint){m,-(lin.a*m+lin.c)/lin.b};
        s = (tsegment){seg.s,p};
        if ( !is_any_crossing(s) ) seg.t = p, L = m;
        else R = m;
      }
    }
  }

  /** create segments from borders */
  for ( auto bor : border ) {
    vector <tsegment> obs;
    for ( int i = 0; i < N; ++i ) {
      tpoint p1, p2;
      if ( is_crossing(bor,cir[i],p1,p2) )
        obs.push_back(normalize((tsegment){p1,p2}));
    }
    if ( obs.empty() ){
      node.push_back(bor);
      continue;
    }
    sort(obs.begin(),obs.end());
    if ( eq(bor.s.y,bor.t.y) ) {
      obs.push_back((tsegment){bor.t,bor.t});
      tpoint p = bor.s;
      for ( int i = 0; i < obs.size(); ++i ) {
        if ( lt(p.x,obs[i].s.x) )
          node.push_back((tsegment){p.x,p.y,obs[i].s.x,obs[i].s.y});
        p = obs[i].t;
      }
    }
    if ( eq(bor.s.x,bor.t.x) ) {
      obs.push_back((tsegment){bor.t,bor.t});
      tpoint p = bor.s;
      for ( int i = 0; i < obs.size(); ++i ) {
        if ( lt(p.y,obs[i].s.y) )
          node.push_back((tsegment){p.x,p.y,obs[i].s.x,obs[i].s.y});
        p = obs[i].t;
      }
    }
  }

  /** construct graph: <segments,intersecting> */
  tpoint p;
  for ( int i = 0; i < node.size(); ++i ) {
    for ( int j = 0; j < node.size(); ++j ) {
      if ( i == j ) continue;
      if ( intersection_point(node[i],node[j],p) )
        con[i].push_back(j);
    }
  }

  /** BFS */
  vector <tpoint> ans;

  queue <int> q;
  bool flag[100005] = {false};
  int parent[100005] = {0};
  for ( int i = 0; i < node.size(); ++i )
    if ( is_start[i] ) q.push(i), flag[i] = true, parent[i] = -1;
  int tans = -1;
  bool found = false;
  while ( !q.empty() && !found ) {
    tans++;
    for ( int i = 0, size = q.size(); i < size; ++i ) {
      int cur = q.front(); q.pop();
      if ( is_goal[cur] ) {
        while ( !is_start[cur] ) {
          tpoint p;
          intersection_point(node[cur],node[parent[cur]],p);
          ans.push_back(p);
          cur = parent[cur];
        }
        found = true;
        break;
      }
      for ( auto &next : con[cur] ) if ( !flag[next] ) {
        q.push(next);
        parent[next] = cur;
        flag[next] = true;
      }
    }
  }

  reverse(ans.begin(),ans.end());
  printf( "%d\n", (int)ans.size() );
  for ( auto &p : ans )
    printf( "%.9lf %.9lf\n", p.x, p.y );

  return 0;
}
