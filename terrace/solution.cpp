#include <bits/stdc++.h>

#define REP(i,n) for (int i = 0; i < n; i++)

using namespace std;

int N, M, K, G1, G2, G3;
char S[100005];
int dp[100005][55];

int main() {
    scanf("%d %d %d %d %d", &N, &K, &G1, &G2, &G3);
    scanf("%s", S);

    REP(i, N) {
        M += S[i] == '#';
    }

    memset(dp, -1, sizeof dp);
    dp[0][0] = 0;

    REP(i, N) REP(j, M+1) {
        if (dp[i][j] == -1) {
            continue;
        }

        // skip
        dp[i+1][j] = max(dp[i+1][j], dp[i][j]);
        
        // tile 2
        if (i+1 < N && S[i] == '.' && S[i+1] == '.') {
            dp[i+2][j] = max(dp[i+2][j], 1 + dp[i][j]);
        }

        // tile 3
        if (i+2 < N && S[i] == '.' && S[i+1] == '#' && S[i+2] == '.') {
            dp[i+3][j+1] = max(dp[i+3][j+1], dp[i][j]);
        }
    }

    int res = 0;
    REP(t3, M+1) {
        if (dp[N][t3] == -1) {
            continue;
        }

        int t2 = dp[N][t3];
        int t1 = min(K, N - 2*t2 - 3*t3 - (M-t3));
        
        if (G1*2 > G2) {
            while (t2 && t1+2 <= K) {
                t2--;
                t1 += 2;
            }
        }
        if (G1 > G2) {
            while (t2 && t1+1 <= K) {
                t2--;
                t1++;
            }
        }
        res = max(res, t1*G1 + t2*G2 + t3*G3);
    }

    printf("%d\n", res);
}
