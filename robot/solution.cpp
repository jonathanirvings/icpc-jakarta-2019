#include <bits/stdc++.h>
using namespace std;

const int N = 100005;
const int MOD = 1000000007;

int n, q;
char s[N];

struct SegTree {
  struct Node {
    int val[2][2];
    bool rev;

    Node(int _val = 0) {
      val[0][0] = _val; val[0][1] = 0;
      val[1][0] = 0; val[1][1] = _val;

      rev = false;
    }

    Node operator * (const Node &other) {
      Node ret;
      ret.val[0][0] = (1LL * val[0][0] * other.val[0][0] + 1LL * val[0][1] * other.val[1][0]) % MOD;
      ret.val[0][1] = (1LL * val[0][0] * other.val[0][1] + 1LL * val[0][1] * other.val[1][1]) % MOD;
      ret.val[1][0] = (1LL * val[1][0] * other.val[0][0] + 1LL * val[1][1] * other.val[1][0]) % MOD;
      ret.val[1][1] = (1LL * val[1][0] * other.val[0][1] + 1LL * val[1][1] * other.val[1][1]) % MOD;

      return ret;
    }

    void reverse() {
      swap(val[0][0], val[1][1]);
      swap(val[0][1], val[1][0]);

      rev = !rev;
    }
  };

  int n;
  Node nodes[N << 2];

  void build(int node, int l, int r, char s[]) {
    if (l == r) {
      nodes[node] = Node(1);

      if (s[l] == 'A') {
        nodes[node].val[1][0] = 1;
      } else if (s[l] == 'B') {
        nodes[node].val[0][1] = 1;
      }

      return;
    }

    int mid = l + r >> 1;
    build(node*2 + 1, l, mid+0, s);
    build(node*2 + 2, mid+1, r, s);
    nodes[node] = nodes[node*2 + 1] * nodes[node*2 + 2];
  }

  void propagate(int node) {
    if (nodes[node].rev) {
      nodes[node*2 + 1].reverse();
      nodes[node*2 + 2].reverse();
      nodes[node].rev = false;
    }
  }

  void update(int node, int l, int r, int a, int b) {
    if (l > b || r < a) return;
    if (a <= l && r <= b) return nodes[node].reverse();

    propagate(node);

    int mid = l + r >> 1;
    update(node*2 + 1, l, mid+0, a, b);
    update(node*2 + 2, mid+1, r, a, b);
    nodes[node] = nodes[node*2 + 1] * nodes[node*2 + 2];
  }

  Node query(int node, int l, int r, int a, int b) {
    if (l > b || r < a) return Node(1);
    if (a <= l && r <= b) return nodes[node];

    propagate(node);

    int mid = l + r >> 1;
    return query(node*2 + 1, l, mid+0, a, b) *
           query(node*2 + 2, mid+1, r, a, b);
  }

  void build(int _n, char s[]) {
    n = _n;
    build(0, 0, n-1, s);
  }

  void update(int l, int r) {
    update(0, 0, n-1, l, r);
  }

  pair<int, int> query(int l, int r, int a, int b) {
    Node node = query(0, 0, n-1, l, r);
    return {
      (1LL * a * node.val[0][0] + 1LL * b * node.val[1][0]) % MOD,
      (1LL * a * node.val[0][1] + 1LL * b * node.val[1][1]) % MOD,
    };
  }
} tree;

int main() {
  scanf("%d %d", &n, &q);
  scanf("%s", s);

  tree.build(n, s);

  while (q--) {
    int t, l, r;
    scanf("%d %d %d", &t, &l, &r);
    --l, --r;

    if (t == 1) {
      tree.update(l, r);
    } else if (t == 2) {
      int a, b;
      scanf("%d %d", &a, &b);

      pair<int, int> res = tree.query(l, r, a, b);
      printf("%d %d\n", res.first, res.second);
    }
  }
  return 0;
}
