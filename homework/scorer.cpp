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

int main(int argc, char *argv[])
{
  FILE *in = fopen(argv[1], "r");
  FILE *out = fopen(argv[2], "r");  // Judge's
  FILE *con = fopen(argv[3], "r");

  int N;
  fscanf(in, "%d\n", &N);

  vector<int> A(N);
  for (int i = 0; i < N; ++i) {
    fscanf(in, "%d", &A[i]);
  }

  vector<int> B(N);
  vector<bool> found(N, false);
  for (int i = 0; i < N; ++i) {
    if (fscanf(con, "%d", &B[i]) != 1) {
      return wa("Unexpected EOF.");
    }
    if (B[i] < 1 || B[i] > N) {
      return wa("Number out of range.");
    }
    if (found[B[i] - 1]) {
      return wa("Found duplicated number.");
    }
    found[B[i] - 1] = true;
  }

  long long diff = 0;
  for (int i = 0; i < N; ++i) {
    diff += abs(A[i] - B[i]);
  }
  
  if (diff < N) {
    return wa("diff(A, B) < N.");
  }

  fscanf(con, "\n");
  char dummy;
  if (fscanf(con, "%c", &dummy) == 1) {
    return wa("Expected EOF.");
  }

  return ac();
}
