#pragma GCC optimize "-O2"
#include <bits/stdc++.h>
using namespace std;
typedef pair<int,int> pii;
typedef vector<int> vi;

const int maxn = 4005;

struct tdata { int node, size; };

int N, a, b;
vector <tdata> con[maxn];
vector <tdata> adj[maxn];

int dfs(int x, int par, int n, vector <tdata> v[]) {
  int total = 1;
  for ( auto &chl : v[x] )
    if ( chl.node != par ) {
      chl.size = dfs(chl.node,x,n,v);
      total += chl.size;
    }
  for ( auto &chl : v[x] )
    if ( chl.node == par ) {
      chl.size = n - total;
    }
  return total;
}

void get_subtree(int x, int par, vector <tdata> v[], vector <int> &subtree) {
  subtree.push_back(x);
  for ( auto &chl : v[x] )
    if ( chl.node != par )
      get_subtree(chl.node,x,v,subtree);
}

bool is_centroid(int x, int n, vector <tdata> v[]) {
  for ( auto &chl : v[x] )
    if ( chl.size * 2 > n ) return false;
  return true;
}

string encode(int x, int par, vector <tdata> v[]) {
  vector <string> vs;
  for ( auto &chl : v[x] ) if ( chl.node != par )
    vs.push_back(encode(chl.node,x,v));
  sort(vs.begin(),vs.end());
  return "(" + accumulate(vs.begin(),vs.end(),string("")) + ")";
}

bool is_cut(int cut) {
  for ( int i = 1; i <= N; i++ ) adj[i].clear();
  for ( int i = 1; i <= N; i++ ) if ( i != cut )
    for ( auto &chl : con[i] )
      if ( chl.node != cut )
        adj[i].push_back(chl);
  vector <vector<string>> codes;
  for ( auto &chl : con[cut] ) {
    vector <int> subtree;
    get_subtree(chl.node,-1,adj,subtree);
    dfs(chl.node,-1,subtree.size(),adj);
    codes.push_back(vector<string>());
    for ( auto &x : subtree )
      if ( is_centroid(x,subtree.size(),adj) )
        codes.back().push_back(encode(x,-1,adj));
  }
  string pattern = codes[0][0];
  for ( auto &code : codes ) {
    bool okay = false;
    for ( auto &c : code )
      if ( c == pattern ) okay = true;
    if ( !okay ) return false;
  }
  return true;
}

int main(int argc, char *argv[]) {
  scanf( "%d", &N );
  for ( int i = 0; i < N-1; i++ ) {
    scanf( "%d %d", &a, &b );
    con[a].push_back((tdata){b,-1});
    con[b].push_back((tdata){a,-1});
  }

  int ans = -1;
  dfs(1,-1,N,con);
  for ( int i = 1; i <= N; i++ )
    if ( is_centroid(i,N,con) && is_cut(i) )
      ans = max(ans,(int)con[i].size());
  
  printf( "%d\n", ans );

  return 0;
}
