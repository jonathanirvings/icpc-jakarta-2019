// #include <bits/stdc++.h>
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <vector>
#include <climits>
using namespace std;
 
#define mt make_tuple
#define mp make_pair
#define pb push_back
#define fi first
#define se second
#define ALL(a) begin(a), end(a)
#define SZ(a) ((int)(a).size())
 
#ifdef __DEBUG
#define debug if (true)
#else
#define debug if (false)
#endif
 
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<ll> vi;

class SegmentTree {
private:
  // Data storage
  vi tree;  // Segment tree
  vi lazy;  // Lazy propagation
  vi array; // Original array
  int length; // Array length

  // Bitmask operations to access children
  int leftChild(int p) {
    return p << 1; 
  }

  int rightChild(int p) { 
    return (p << 1) + 1; 
  }

  void build(int node, int start, int finish) {
    // cout << "Node: " << node << " " << start << " " << finish << endl;
    if(start == finish) {
      // Leaf node
      tree[node] = array[start];
    } else {
      int mid = (start + finish) / 2;
      // Recurse on left child
      build(leftChild(node), start, mid);
      // Recurse on right child
      build(rightChild(node), mid+1, finish);
      // Parent node
      // This depends on the query type of the Segment Tree (cur: query min)
      tree[node] = min(tree[leftChild(node)], tree[rightChild(node)]);
    }
  }

  void updateRange(int node, int start, int finish, int left, int right, ll value) {
    // cout << "updateRange " << node << " " << start << " " << finish << " " << left << " " << right << " " << value << endl;
    
    // Check whether there are pending update
    if(lazy[node] != 0) {
      // This node needs to be updated
      // This depends on the type of the Segment Tree (cur: query min, update add)
      tree[node] += lazy[node];
      if(start != finish) {
        // Mark left child as lazy
        lazy[leftChild(node)] += lazy[node];
        // Mark right child as lazy;
        lazy[rightChild(node)] += lazy[node];
      }
      lazy[node] = 0; // Mark that lazy update has been done
    } 

    // Main process
    if(finish < start || right < start || finish < left) {
      // Node range is outside the query range
      return;
    } else
    if(left <= start and finish <= right) {
      // Node range is inside the query range
      // This depends on the type of the Segment Tree (cur: query min, update add)
      tree[node] += value;
      if(start != finish) {
        // Not leaf node
        lazy[leftChild(node)] += value;  // Create pending update
        lazy[rightChild(node)] += value; // Create pending update
      }
      return;
    } else {
      int mid = (start + finish) / 2;
      // Recurse on left child
      updateRange(leftChild(node), start, mid, left, right, value);
      // Recurse on right child
      updateRange(rightChild (node), mid+1, finish, left, right, value);
      // This depends on the type of the Segment Tree (cur: query min)
      tree[node] = min(tree[leftChild(node)], tree[rightChild(node)]);
    }
  }

  ll queryRange(int node, int start, int finish, int left, int right) {
    if(finish < start || right < start || finish < left) {
      // Node range is outside the query range
      return LLONG_MAX;
    } else {
      // Check whether there are pending update
      if(lazy[node] != 0) { 
        // This node needs to be updated
        // This depends on the type of the Segment Tree (cur: query min, update add)
        tree[node] += lazy[node];
        if(start != finish) {
          // Mark left child as lazy
          lazy[leftChild(node)] += lazy[node];
          // Mark right child as lazy;
          lazy[rightChild(node)] += lazy[node];
        }
        lazy[node] = 0; // Mark that lazy update has been done
      }

      if(left <= start && finish <= right) {
        // Node range is inside the query range 
        return tree[node];
      } else {
        int mid = (start + finish) / 2;
        ll leftQuery = queryRange(leftChild(node), start, mid, left, right);
        ll rightQuery = queryRange(rightChild(node), mid+1, finish, left, right);
        // This depends on the query type of the Segment Tree (cur: query min)
        return min(leftQuery, rightQuery); 
      }
  }
};

public:
  SegmentTree(const vi &_array) {
    array = _array;
    length = (int)array.size();
    tree.assign(4 * length, 0);
    lazy.assign(4 * length, 0);
    build(1, 0, length - 1);
  }

  void updateElement(int index, int value) {
    updateRange(1, 1, length, index, index, value);
  }

  void updateRange(int left, int right, ll value) {
    updateRange(1, 1, length, left, right, value);
  }

  ll queryRange(int left, int right) {
    return queryRange(1, 1, length, left, right);
  }

  // custom for this problem
  ll perform(int j, ll value) {
    updateRange(1, 1, length, j + 1, length, value);
    return queryRange(1, 1, length, 1, length);   
  }

  ll query() {
    return queryRange(1, 1, length, 1, length);
  }
};

int A[100005];
int R[100005];
vector<int> B[100005];

int X[100005];
int Y[100005];
int Z[100005];

int D;

int over[100005];
int under[100005];

int remaining[100005];

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(0);
  cout.tie(0);

  int N, M, Q;
  cin >> N >> M >> Q;

  // INPUT

  for (int i = 1; i <= N; ++i) {
    cin >> A[i];

  }
  D = A[1];

  for (int j = 1; j <= M; ++j) {
    cin >> R[j];

    B[j].push_back(-1);
    for (int i = 1; i <= R[j]; ++i) {
      int perf;
      cin >> perf;
      B[j].push_back(perf);
    }
  }

  for (int k = 0; k < Q; ++k) {
    cin >> X[k] >> Y[k] >> Z[k];
  }

  // PROC

  over[0] = 0;
  under[0] = 0;
  for (int i = 1; i <= N; ++i) {
    if (A[i] >= D) {
      over[0]++;
    } else {
      under[0]++;
    }
  }

  for (int j = 1; j <= M; ++j) {
    over[j] = 0;
    under[j] = 0;
    for (int i = 1; i <= R[j]; ++i) {
      if (B[j][i] > D) {
        over[j]++;
      } else {
        under[j]++;
      }
    }
  }

  int prefix_over = 0;
  for (int j = 0; j < M; ++j) {
    prefix_over += over[j];
    remaining[j] = N - prefix_over - R[j + 1];
  }

  vi array(M, 0);
  for (int j = 0; j < M; ++j) {
    array[j] = remaining[j];
  }
  SegmentTree segmentTree(array);

  bool cur_answer = true;
  int minv = segmentTree.query();
  if (minv < 0) {
    cur_answer = false;
  }

  // QUERY
  for (int k = 0; k < Q; ++k) {
    bool answer = true;

    if (B[X[k]][Y[k]] > D && Z[k] < D) {
      // over -> under
      over[X[k]]--;
      under[X[k]]++;

      int minv = segmentTree.perform(X[k], 1);
      if (minv < 0) {
        answer = false;
      }
    } else
    if (B[X[k]][Y[k]] < D && Z[k] > D) {
      // under -> over
      over[X[k]]++;
      under[X[k]]--;

      int minv = segmentTree.perform(X[k], -1);
      if (minv < 0) {
        answer = false;
      }
    } else {
      answer = cur_answer;
    }
    B[X[k]][Y[k]] = Z[k];
  
    if (answer) {
      cout << 1 << endl;
    } else {
      cout << 0 << endl;
    }

    cur_answer = answer;
  }

}