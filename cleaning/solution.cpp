#pragma GCC optimize "-O2"
#include <bits/stdc++.h>
using namespace std;
typedef pair<int,int> pii;
typedef long long ll;

const int maxn = 100005;
const int mod  = 1000000007;

pii exeuclid(int a, int b) {
  if ( b == 0 ) return pii(1,0);
  pii r = exeuclid(b,a%b);
  return pii(r.second,r.first-a/b*r.second);
}
int inv(int a, int m=mod) {
  return (exeuclid(a%m,m).first + m) % m;
}

int N, a, b;
vector <int> con[maxn];
vector <int> chl[maxn];

int h1(int, int);
int h2(int);
int h3(int);

int dp[maxn][3];
int f(int u, int s) {
  if ( dp[u][s] != -1 ) return dp[u][s];
  int &ret = dp[u][s] = 0;
  if ( s == 0 ) {
    ret = (ret + h1(u,1)) % mod;
    ret = (ret + h2(u)) % mod;
    ret = (ret + h3(u)) % mod;
  }
  else if ( s == 1 ) {
    ret = (ret + h2(u)) % mod;
  }
  else if ( s == 2 ) {
    ret = (ret + h1(u,0)) % mod;
    ret = (ret + h3(u)) % mod;
  }
  return ret;
}

int opt0(int u, int s1, int s2) {
  int prod = 1;
  for ( auto &v : chl[u] )
    if ( f(v,s2) == 0 )
      prod = (ll)prod * f(v,s1) % mod;
    else
      prod = (ll)prod * f(v,s2) % mod;
  return prod;
}

int opt1(int u, int s1, int s2, int exc = -1) {
  int A = 1, B = 0;
  for ( auto &v : chl[u] ) if ( v != exc ) {
    A = (ll)A * f(v,s2) % mod;
    B = (B + ((ll)f(v,s1) * inv(f(v,s2)))) % mod;
  }
  return (ll)A * B % mod;
}

int opt2(int u, int s1, int s2) {
  int A = 1;
  for ( auto &v : chl[u] )
    A = (ll)A * f(v,s2) % mod;
  int B = 0;
  int n = chl[u].size();
  int g[n+1] = {0};
  int pg[n+1] = {0};
  for ( int i = 0; i < n; ++i )
    g[i] = (ll)f(chl[u][i],s1) * inv(f(chl[u][i],s2)) % mod;
  for ( int i = n - 1; i >= 0; --i )
    pg[i] = (pg[i+1] + g[i]) % mod;
  for ( int i = 0; i < n; ++i )
    B = (B + ((ll)g[i] * pg[i+1] % mod)) % mod;
  return (ll)A * B % mod;
}

int h1(int u, int s) {
  int n_zero = 0;
  for ( auto &v : chl[u] )
    if ( f(v,s) == 0 ) n_zero++;
  if ( n_zero == 0 )
    return opt1(u,2,s);
  if ( n_zero == 1 )
    return opt0(u,2,s);
  return 0;
}

int h2(int u) {
  int n_zero = 0;
  for ( auto &v : chl[u] )
    if ( f(v,0) == 0 ) n_zero++;
  if ( n_zero == 0 )
    return opt2(u,2,0);
  if ( n_zero == 1 )
    for ( auto &v : chl[u] )
      if ( f(v,0) == 0 )
        return (ll)f(v,2) * opt1(u,2,0,v) % mod;
  if ( n_zero == 2 )
    return opt0(u,2,0);
  return 0;
}

int h3(int u) {
  int prod = 1;
  for ( auto &v : chl[u] )
    prod = (ll)prod * f(v,1) % mod;
  return prod;
}

void make_root(int u, int p) {
  for ( auto &v : con[u] )
    if ( v != p ) {
      chl[u].push_back(v);
      make_root(v,u);
    }
}

int main(int argc, char *argv[]) {

  scanf( "%d", &N );
  for ( int i = 1; i < N; ++i ) {
    scanf( "%d %d", &a, &b );
    con[a].push_back(b);
    con[b].push_back(a);
  }

  make_root(1,1);
  memset(dp,-1,sizeof(dp));
  printf( "%d\n", f(1,0) );

  return 0;
}
