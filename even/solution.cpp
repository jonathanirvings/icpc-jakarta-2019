// INCLUDE LIST
#include <cstdio>
#include <bitset>
#include <iostream>
#include <queue>
#include <stack>
#include <string>
#include <cstring>
#include <algorithm>
#include <map>
#include <set>
#include <list>
#include <vector>
#include <cstdlib>
#include <cctype>
#include <cmath>
#include <iomanip>
using namespace std;

int n, q;
int r[200020], c[200020];
int labelR[200020], labelC[200020];
int a, b, x, y;

int main() {
  cin >> n >> q;
  for (int i = 0; i < n; i++) {
    cin >> r[i];
    r[i] %= 2;
  }
  
  for (int i = 0; i < n; i++) {
    cin >> c[i];
    c[i] %= 2;
  }
  
  labelR[0] = 0;
  labelC[0] = 0;
  for (int i = 1; i < n; i++) {
    labelR[i] = labelR[i - 1] + (r[i] != r[i - 1]);
    labelC[i] = labelC[i - 1] + (c[i] != c[i - 1]);
  }
  
  while (q--) {
    cin >> a >> b >> x >> y;
    
    a--, b--, x--, y--;
    
    if (labelR[a] == labelR[x] && labelC[b] == labelC[y]) cout << "YES" << endl;
    else cout << "NO" << endl;
  }
  return 0;
}


