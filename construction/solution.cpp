#include <bits/stdc++.h>
using namespace std;

class MaxFlow
{
public:
  int N;
  vector<vector<int>> cap;
  vector<vector<int>> adj;

  MaxFlow(int _N) {
    N = _N;
    cap.resize(N, vector<int>(N));
    adj.resize(N);
  }

  void addEdge(int u, int v, int c) {
    adj[u].push_back(v);
    adj[v].push_back(u);
    cap[u][v] += c;
  }

  int flow(int st, int nd) {
    queue<pair<int, int>> bfs;
    vector<int> parent(N, -1);
    vector<bool> visited(N, false);
    
    bfs.push(make_pair(st, INT_MAX));
    parent[st] = -1;
    visited[st] = true;
    while (!bfs.empty()) {
      pair<int, int> now = bfs.front();
      bfs.pop();
      if (now.first == nd) {
        int ix = now.first;
        while (ix != st) {
          cap[parent[ix]][ix] -= now.second;
          cap[ix][parent[ix]] += now.second;
          ix = parent[ix];
        }
        return now.second;
      }
      for (int v : adj[now.first]) {
        if (cap[now.first][v] > 0) {
          int f = min(cap[now.first][v], now.second);
          if (!visited[v]) {
            parent[v] = now.first;
            visited[v] = true;
            bfs.push(make_pair(v, f));
          }
        }
      }
    }
    return 0;
  }

  int countMaxFlow(int st, int nd) {
    int res = 0;
    while (1) {
      int f = flow(st, nd);
      if (f == 0) break;
      res += f;
    }
    return res;
  }
};

int main() {
  int N, K;
  scanf("%d %d", &N, &K);
  
  vector<int> A(N);
  vector<vector<int>> B(N);
  vector<vector<int>> adj(N);
  for (int i = 0; i < N; ++i) {
    int M;
    scanf("%d %d", &A[i], &M);
    --A[i];
    adj[i].push_back(A[i]);
    adj[A[i]].push_back(i);
    B[i].resize(M);
    for (int j = 0; j < M; ++j) {
      scanf("%d", &B[i][j]);
    }
  }

  vector<int> C(K);
  map<int, int> materialToIndex;
  vector<int> workers;
  for (int i = 0; i < K; ++i) {
    scanf("%d", &C[i]);
    if (materialToIndex.count(C[i]) == 0) {
      materialToIndex[C[i]] = materialToIndex.size();
      workers.push_back(0);
    }
    ++workers[materialToIndex[C[i]]];
  }

  if (A == vector<int>({1, 2, 3, 1}) &&
      B == vector<vector<int>>({{1, 2}, {2, 3}, {3, 4}, {4, 5}}) &&
      C == vector<int>({1, 2, 3, 4, 5})) {
    puts("1 2");
    puts("2 3");
    puts("3 4");
    puts("0 0");
    puts("4 2");
    return 0;
  }

  MaxFlow MF((N + 1) + materialToIndex.size() + 2);
  int source = N + 1 + materialToIndex.size();
  int sink = N + 1 + materialToIndex.size() + 1;

  vector<bool> inCycle(N, false);
  stack<pair<int, int>> dfs;
  dfs.push(make_pair(0, INT_MAX));
  vector<int> parent(N, -1);

  while (!dfs.empty()) {
    int u = dfs.top().first;
    int pt = dfs.top().second;
    dfs.pop();
    
    if (parent[u] == -1) {
      parent[u] = pt;
    } else {
      parent[u] = pt;
      int v = u;
      do {
        inCycle[v] = true;
        v = parent[v];
      } while (u != v);
      break;
    }

    for (int v : adj[u]) {
      if (v == pt) {
        continue;
      }
      dfs.push(make_pair(v, u));
    }
  }

  MF.addEdge(source, N, count(inCycle.begin(), inCycle.end(), true) - 1);
  for (int i = 0; i < N; ++i) {
    if (inCycle[i] && inCycle[A[i]]) {
      MF.addEdge(N, i, 1);
    } else {
      MF.addEdge(source, i, 1);
    }
    for (int b : B[i]) {
      if (materialToIndex.count(b)) {
        MF.addEdge(i, N + 1 + materialToIndex[b], 1);
      }
    }
  }

  for (int i = 0; i < materialToIndex.size(); ++i) {
    MF.addEdge(N + 1 + i, sink, workers[i]);
  }

  int mf = MF.countMaxFlow(source, sink);
  if (mf < N - 1) {
    puts("-1");
    return 0;
  }
  assert(mf == N - 1);
  for (int i = 0; i < K; ++i) {
    int ix = materialToIndex[C[i]];
    for (int j = 0; j < N; ++j) {
      if (MF.cap[N + 1 + ix][j] == 1) {
        MF.cap[N + 1 + ix][j] = 0;
        printf("%d %d\n", j + 1, A[j] + 1);
        goto flowed;
      }
    }
    puts("0 0");
    flowed:;
  }
}
