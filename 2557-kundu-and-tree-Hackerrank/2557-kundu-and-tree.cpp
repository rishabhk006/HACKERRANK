#include <bits/stdc++.h>
using namespace std;

const long long MOD = 1000000007;

vector<vector<int>> adj;
vector<int> vis;

long long dfs(int u) {
    vis[u] = 1;
    long long cnt = 1;

    for (int v : adj[u]) {
        if (!vis[v])
            cnt += dfs(v);
    }

    return cnt;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    adj.assign(n + 1, {});
    vis.assign(n + 1, 0);

    for (int i = 0; i < n - 1; i++) {
        int u, v;
        char c;
        cin >> u >> v >> c;

        if (c == 'b') {
            adj[u].push_back(v);
            adj[v].push_back(u);
        }
    }

    long long total = 1LL * n * (n - 1) * (n - 2) / 6;
    long long bad = 0;

    for (int i = 1; i <= n; i++) {
        if (!vis[i]) {
            long long s = dfs(i);

            if (s >= 2)
                bad += s * (s - 1) / 2 * (n - s);

            if (s >= 3)
                bad += s * (s - 1) * (s - 2) / 6;
        }
    }

    cout << (total - bad) % MOD << '\n';

    return 0;
}


// Synced seamlessly with LeetHub Pro
// Pro features: https://bit.ly/leethubpro | Free version: https://bit.ly/leethubv4
// Get it here: https://chromewebstore.google.com/detail/bcilpkkbokcopmabingnndookdogmbna