#include <bits/stdc++.h>

using namespace std;

int main() {
  int n;
  scanf("%d", &n);
  vector<pair<int, int>> papers;
  long long ans = 0;
  for (int i = 0; i < n; ++i) {
    int a, b;
    scanf("%d %d", &a, &b);
    if (a > b) swap(a, b);
    papers.emplace_back(a, b);
    ans = max(ans, 1LL * a * b);
  }
  sort(papers.begin(), papers.end());
  reverse(papers.begin(), papers.end());
  int bmax = 0;
  for (auto it : papers) {
    ans = max(ans, 2LL * it.first * min(bmax, it.second));
    bmax = max(bmax, it.second);
  }
  printf("%lld.%d\n", ans / 2, int(5 * (ans & 1)));
  return 0;
}
