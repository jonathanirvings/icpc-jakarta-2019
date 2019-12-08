#include <bits/stdc++.h>

using namespace std;

struct SuffixArray {
  string s;
  int n;
  vector<int> sa, pos, lcp, tmp, cnt;
  vector<vector<int>> sparse;

  SuffixArray(string t) : s(t) {
    n = s.size();
    sa.assign(n, 0);
    pos.assign(n, 0);
    tmp.assign(n, 0);
    cnt.assign(max(n, 256), 0);
    lcp.assign(n-1, 0);
    sparse.clear();
    BuildSA();
    BuildLCP();
  }

  void CountingSort(int gap) {
    fill(cnt.begin(), cnt.end(), 0);
    for (int i = 0; i < n; ++i) {
      if (i + gap >= n) {
        ++cnt[0];
        continue;
      }
      ++cnt[pos[i+gap] + 1];
    }
    int sum = 0;
    for (int i = 0; i < (int)cnt.size(); ++i) {
      int cur = cnt[i];
      cnt[i] = sum;
      sum += cur;
    }
    for (int i = 0; i < n; ++i) {
      int cur = sa[i];
      if (cur + gap >= n)
        tmp[cnt[0]++] = cur;
      else
        tmp[cnt[pos[cur + gap] + 1]++] = cur;
    }
    for (int i = 0; i < n; ++i)
      sa[i] = tmp[i];
  }

  void BuildSA() {
    for(int i = 0; i < n; ++i)
      sa[i] = i, pos[i] = s[i];
    for(int gap = 1;; gap <<= 1) {
      auto cmp = [&](int a, int b) {
        if(pos[a] - pos[b])
          return pos[a] < pos[b];
        a += gap; b += gap;
        return (a < n && b < n) ? pos[a] < pos[b] : a > b;
      };
      // sort(sa.begin(), sa.end(), cmp);
      CountingSort(gap);
      CountingSort(0);
      tmp[0] = 0;
      for(int i = 1; i < n; ++i) tmp[i] = tmp[i-1] + cmp(sa[i-1], sa[i]);
      for(int i = 0; i < n; ++i) pos[sa[i]] = tmp[i];
      if(tmp[n-1] == n-1) break;
    }
  }

  void BuildLCP() {
    for(int i = 0, k = 0; i < n; ++i) if(pos[i] - n + 1) {
      for(int j = sa[pos[i] + 1]; s[j + k] == s[i + k]; ++k);
      lcp[pos[i]] = k;
      if(k) k--;
    }
    sparse.push_back(lcp);
    for (int i = 0; ; ++i) {
      int len = n - (1 << (i+1));
      if (len <= 0) break;
      sparse.push_back(vector<int>(len));
      for (int j = 0; j < len; ++j) {
        int left = sparse[i][j], right = sparse[i][j+(1 << i)];
        sparse[i+1][j] = min(left, right);
      }
    }
  }

  int GetLCP(int l, int r) {
    if (l >= r)
      return n;
    int len = r-l;
    int lg = 31 - __builtin_clz(len);
    return min(sparse[lg][l], sparse[lg][r-(1<<lg)]);
  }

  pair<int, int> FindOccurs(int p, int len) {
    pair<int, int> ret = {p, p};
    int lo = 0, hi = p;
    while (lo < hi) {
      int mid = (lo + hi) >> 1;
      if (GetLCP(mid, p) < len)
        lo = mid + 1;
      else
        hi = mid;
    }
    ret.first = lo;

    lo = p, hi = n-1;
    while (lo < hi) {
      int mid = (lo + hi + 1) >> 1;
      if (GetLCP(p, mid) < len)
        hi = mid - 1;
      else
        lo = mid;
    }
    ret.second = hi;

    return ret;
  }
};

struct Fenwick {
  int n;
  vector<int> bit;
  Fenwick(int _n) : n(_n) {
    bit.assign(n+1, 0);
  }
  void update(int x, int v) {
    for (++x; x <= n; x += x&-x)
      bit[x] += v;
  }
  int get(int x) {
    int r = 0;
    for (++x; x; x -= x&-x)
      r += bit[x];
    return r;
  }
};

const int LEN = 1e6 + 6;

char buffer[LEN];

int main() {
  int r, c, q;
  scanf("%d %d %d", &r, &c, &q);
  vector<string> grid(r);
  for (int i = 0; i < r; ++i) {
    scanf("%s", buffer);
    grid[i] = buffer;
  }
  vector<string> que(q);
  vector<long long> ans(q, 0);
  for (int i = 0; i < q; ++i) {
    scanf("%s", buffer);
    que[i] = buffer;
  }
  int len = 0;
  for (int i = 0; i < r; ++i) {
    for (int j = c-1; j >= 0; --j) {
      buffer[len++] = grid[i][j];
    }
    buffer[len++] = '#';
  }
  for (int i = 0; i < q; ++i) {
    for (int j = (int)que[i].size()-1; j >= 0; --j)
      buffer[len++] = que[i][j];
    buffer[len++] = '#';
  }
  buffer[len] = 0;
  SuffixArray hor(buffer);
  len = 0;
  for (int j = 0; j < c; ++j) {
    for (int i = 0; i < r; ++i) {
      buffer[len++] = grid[i][j];
    }
    buffer[len++] = '#';
  }
  for (int i = 0; i < q; ++i) {
    for (int j = 0; j < (int)que[i].size(); ++j)
      buffer[len++] = que[i][j];
    buffer[len++] = '#';
  }
  buffer[len] = 0;
  SuffixArray ver(buffer);
  vector<pair<int, int>> points(r * c);
  for (int i = 0; i < r; ++i) {
    for (int j = 0; j < c; ++j) {
      int posh = i * (c + 1) + c-1-j;
      int posv = j * (r + 1) + i;
      points[i * c + j] = {hor.pos[posh], ver.pos[posv]};
    }
  }
  vector<tuple<int, int, int>> events;
  int posh = r * c + r, posv = r * c + c;
  for (int i = 0; i < q; ++i) {
    int sz = que[i].size();
    for (int j = 0; j < sz; ++j) {
      int curh = hor.pos[posh + sz-1-j];
      int curv = ver.pos[posv + j];
      pair<int, int> rangeh = hor.FindOccurs(curh, j+1);
      pair<int, int> rangev = ver.FindOccurs(curv, sz-j);
      events.emplace_back(rangeh.first-1, rangev.first-1, i);
      events.emplace_back(rangeh.second, rangev.first-1, ~i);
      events.emplace_back(rangeh.first-1, rangev.second, ~i);
      events.emplace_back(rangeh.second, rangev.second, i);
    }
    posh += sz + 1;
    posv += sz + 1;
  }
  Fenwick fen(ver.n + 1);
  sort(points.begin(), points.end());
  sort(events.begin(), events.end());
  int iter = 0;
  for (auto e : events) {
    int h, v, id;
    tie(h, v, id) = e;
    while (iter < (int)points.size() && points[iter].first <= h) {
      fen.update(points[iter].second, +1);
      ++iter;
    }
    int cur = fen.get(v);
    if (id < 0)
      ans[~id] -= cur;
    else
      ans[id] += cur;
  }
  for (int i = 0; i < q; ++i) {
    printf("%lld\n", ans[i]);
  }
  return 0;
}
