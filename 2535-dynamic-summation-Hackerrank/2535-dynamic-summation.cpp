#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

const int MAXN = 100005;
const int LOGN = 18;

vector<long long> MODULI;
int group_assignment[105];

vector<int> adj[MAXN];
int tin[MAXN], tout[MAXN], depth[MAXN], timer;
int up[MAXN][LOGN];

vector<vector<__int128>> tree;
vector<vector<__int128>> lazy;

long long gcd(long long a, long long b) {
    while (b) { a %= b; swap(a, b); }
    return a;
}

long long power(long long base, long long exp, long long mod) {
    long long res = 1;
    base %= mod;
    while (exp > 0) {
        if (exp % 2 == 1) res = (__int128)res * base % mod;
        base = (__int128)base * base % mod;
        exp /= 2;
    }
    return res;
}

void dfs(int u, int p, int d) {
    tin[u] = ++timer;
    depth[u] = d;
    up[u][0] = p;
    for (int i = 1; i < LOGN; ++i) {
        up[u][i] = up[up[u][i - 1]][i - 1];
    }
    for (int v : adj[u]) {
        if (v != p) {
            dfs(v, u, d + 1);
        }
    }
    tout[u] = timer;
}

bool is_ancestor(int u, int v) {
    return tin[u] <= tin[v] && tout[u] >= tout[v];
}

int get_child_ancestor(int ancestor, int descendant) {
    for (int i = LOGN - 1; i >= 0; --i) {
        if (!is_ancestor(up[descendant][i], ancestor)) {
            descendant = up[descendant][i];
        }
    }
    return descendant;
}

void push(int m_idx, int node, int start, int end) {
    if (lazy[m_idx][node] == 0) return;
    __int128 mod = MODULI[m_idx];
    int mid = (start + end) / 2;

    lazy[m_idx][2 * node] = (lazy[m_idx][2 * node] + lazy[m_idx][node]) % mod;
    tree[m_idx][2 * node] = (tree[m_idx][2 * node] + lazy[m_idx][node] * (mid - start + 1)) % mod;

    lazy[m_idx][2 * node + 1] = (lazy[m_idx][2 * node + 1] + lazy[m_idx][node]) % mod;
    tree[m_idx][2 * node + 1] = (tree[m_idx][2 * node + 1] + lazy[m_idx][node] * (end - mid)) % mod;

    lazy[m_idx][node] = 0;
}

void update(int m_idx, int node, int start, int end, int l, int r, __int128 val) {
    if (r < start || end < l) return;
    __int128 mod = MODULI[m_idx];
    if (l <= start && end <= r) {
        lazy[m_idx][node] = (lazy[m_idx][node] + val) % mod;
        tree[m_idx][node] = (tree[m_idx][node] + val * (end - start + 1)) % mod;
        return;
    }
    push(m_idx, node, start, end);
    int mid = (start + end) / 2;
    update(m_idx, 2 * node, start, mid, l, r, val);
    update(m_idx, 2 * node + 1, mid + 1, end, l, r, val);
    tree[m_idx][node] = (tree[m_idx][2 * node] + tree[m_idx][2 * node + 1]) % mod;
}

__int128 query(int m_idx, int node, int start, int end, int l, int r) {
    if (r < start || end < l) return 0;
    if (l <= start && end <= r) return tree[m_idx][node];
    push(m_idx, node, start, end);
    int mid = (start + end) / 2;
    __int128 p1 = query(m_idx, 2 * node, start, mid, l, r);
    __int128 p2 = query(m_idx, 2 * node + 1, mid + 1, end, l, r);
    return (p1 + p2) % MODULI[m_idx];
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // Dynamically group moduli up to 2*10^18
    for (int m = 1; m <= 101; ++m) {
        bool assigned = false;
        for (size_t i = 0; i < MODULI.size(); ++i) {
            __int128 next_lcm = (__int128)MODULI[i] * m / gcd(MODULI[i], m);
            if (next_lcm <= 2000000000000000000LL) {
                MODULI[i] = next_lcm;
                group_assignment[m] = i;
                assigned = true;
                break;
            }
        }
        if (!assigned) {
            MODULI.push_back(m);
            group_assignment[m] = MODULI.size() - 1;
        }
    }

    int num_trees = MODULI.size();
    tree.assign(num_trees, vector<__int128>(4 * MAXN, 0));
    lazy.assign(num_trees, vector<__int128>(4 * MAXN, 0));

    int n;
    if (!(cin >> n)) return 0;

    for (int i = 0; i < n - 1; ++i) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    timer = 0;
    dfs(1, 1, 0);

    int q;
    cin >> q;
    while (q--) {
        char type;
        cin >> type;
        if (type == 'U') {
            int r, t;
            long long a, b;
            cin >> r >> t >> a >> b;

            vector<pair<int, int>> ranges;
            if (r == t) {
                ranges.push_back({1, n});
            } else if (is_ancestor(t, r)) {
                int child = get_child_ancestor(t, r);
                if (tin[child] > 1) ranges.push_back({1, tin[child] - 1});
                if (tout[child] < n) ranges.push_back({tout[child] + 1, n});
            } else {
                ranges.push_back({tin[t], tout[t]});
            }

            for (int m_idx = 0; m_idx < num_trees; ++m_idx) {
                long long mod = MODULI[m_idx];
                long long term1 = power(a, b, mod);
                long long term2 = power(a + 1, b, mod);
                long long term3 = power(b + 1, a, mod);
                long long val = (term1 + term2 + term3) % mod;

                for (auto& range : ranges) {
                    update(m_idx, 1, 1, n, range.first, range.second, val);
                }
            }
        } else if (type == 'R') {
            int r, t;
            long long m;
            cin >> r >> t >> m;

            int target_idx = group_assignment[m];

            vector<pair<int, int>> ranges;
            if (r == t) {
                ranges.push_back({1, n});
            } else if (is_ancestor(t, r)) {
                int child = get_child_ancestor(t, r);
                if (tin[child] > 1) ranges.push_back({1, tin[child] - 1});
                if (tout[child] < n) ranges.push_back({tout[child] + 1, n});
            } else {
                ranges.push_back({tin[t], tout[t]});
            }

            long long total_sum = 0;
            for (auto& range : ranges) {
                total_sum += query(target_idx, 1, 1, n, range.first, range.second);
            }
            
            cout << (total_sum % m) << "\n";
        }
    }

    return 0;
}


// Synced seamlessly with LeetHub Pro
// Pro features: https://bit.ly/leethubpro | Free version: https://bit.ly/leethubv4
// Get it here: https://chromewebstore.google.com/detail/bcilpkkbokcopmabingnndookdogmbna