#include <bits/stdc++.h>
using namespace std;

typedef pair<int,int> pii;
#define fi first
#define se second
#define mp make_pair
#define pb push_back

const int N = 1e5;
int n, l, r, k;

int ar[N + 5];
int diff[N + 5];

pii f[N + 5];
int ans[N + 5];

pii intersect(pii a, pii b){
  if(a.se < b.fi || b.se < a.fi) return mp(-1,-1);
  return mp(max(a.fi, b.fi), min(a.se, b.se));
}

int main(){
  scanf("%d%d%d%d", &n, &l, &r, &k);
  for(int i = 0;i < n; ++i){
    scanf("%d", &ar[i]);
  }

  for(int i = 1;i < n; ++i){
    diff[i - 1] = (ar[i - 1] == ar[i]) ? 0 : (ar[i - 1] < ar[i]) ? 1 : -1;
  }

  f[n - 1] = mp(l, r);
  for(int i = n - 2;i >= 0; --i){
    if(diff[i] == 0){
      f[i] = f[i + 1];
    }
    else if(diff[i] == -1){
      if(f[i + 1].fi + 1 > r){
        puts("-1");
        return 0;
      }
      int low = f[i + 1].fi + 1;
      int hi = min(f[i + 1].se + k, r);
      f[i] = mp(low, hi); 
    }
    else{
      if(f[i + 1].se - 1 < l){
        puts("-1");
        return 0;
      }
      int low = max(f[i + 1].fi - k, l);
      int hi = f[i + 1].se - 1;
      f[i] = mp(low, hi);
    }
    // fprintf(stderr, "f[%d]: (%d, %d), diff[%d] = %d\n", i, f[i].fi, f[i].se, i, diff[i]);
  }
  int tmp = f[0].fi;
  printf("%d", tmp);
  for(int i = 0; i < n - 1; ++i){
    if(diff[i] == -1){
      pii ints = intersect(mp(tmp - k, tmp - 1), f[i + 1]);
      tmp = ints.fi;
    }
    else if(diff[i] == 1){
      pii ints = intersect(mp(tmp + 1, tmp + k), f[i + 1]);
      tmp = ints.fi;
    }
    printf(" %d", tmp);
  }
  puts("");

  return 0;
}