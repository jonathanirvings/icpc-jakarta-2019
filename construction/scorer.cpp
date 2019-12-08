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

const int IMPOSSIBLE_KEY = -1;

int main(int argc, char *argv[])
{
  FILE *in = fopen(argv[1], "r");
  FILE *out = fopen(argv[2], "r");  // Judge's
  FILE *con = fopen(argv[3], "r");

  int N, K;
  fscanf(in, "%d %d", &N, &K);

  vector<unordered_map<int, unordered_set<int>>> materials(N);

  for (int i = 0; i < N; ++i) {
    int A, M;
    fscanf(in, "%d %d", &A, &M);
    --A;
    while (M--) {
      int B;
      fscanf(in, "%d", &B);
      materials[i][A].insert(B);
      materials[A][i].insert(B);
    }
  }

  vector<int> C(K);
  for (int i = 0; i < K; ++i) {
    fscanf(in, "%d", &C[i]);
  }

  int judge;
  fscanf(out, "%d", &judge);

  if (fscanf(out, "%d", &judge) == EOF) {
    int contestant;
    if (fscanf(con, "%d", &contestant) != 1) {
      return wa("Unexpected EOF.");
    }
    if (contestant != IMPOSSIBLE_KEY) {
      return wa("Judge answer is `impossible`, contestant answer is not.");
    }
  } else {
    vector<int> parent(N);
    for (int i = 0; i < N; ++i) {
      parent[i] = i;
    }

    function<int (int)> findRoot = [&](int U) {
      return parent[U] = (U == parent[U] ? U : findRoot(parent[U]));
    };

    vector<unordered_set<int>> edges(N);

    for (int i = 0; i < K; ++i) {
      int U, V;
      if (fscanf(con, "%d %d", &U, &V) != 2) {
        return wa("Unexpected EOF.");
      }
      if (U == 0 && V == 0) {
        continue;
      }
      if (U < 1 || U > N || V < 1 || V > N) {
        return wa("Index out of bound.");
      }
      --U;
      --V;
      if (edges[U].count(V)) {
        return wa("A pair of cities is directly connected by >1 road.");
      }
      edges[U].insert(V);
      edges[V].insert(U);
      if (!materials[U][V].count(C[i])) {
        return wa("Worker is not familiar with the assigned road material.");
      }

      parent[findRoot(U)] = findRoot(V);
    }
    for (int i = 0; i < N; ++i) {
      if (findRoot(i) != findRoot(0)) {
        return wa("Graph is not connected.");
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
