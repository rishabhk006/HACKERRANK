#include <bits/stdc++.h>
using namespace std;

const int MOD = 1e9 + 7;
const int MAXN = 100005;

vector<int> adj[MAXN];
int parent_node[MAXN], depth[MAXN], heavy[MAXN], head[MAXN], pos[MAXN], sz[MAXN];
int cur_pos = 1;
int node_at_pos[MAXN];

// --- Heavy-Light Decomposition ---
void dfs_sz(int u, int p, int d) {
    parent_node[u] = p;
    depth[u] = d;
    sz[u] = 1;
    int max_ch_sz = 0;
    heavy[u] = 0;
    for (int v : adj[u]) {
        if (v != p) {
            dfs_sz(v, u, d + 1);
            sz[u] += sz[v];
            if (sz[v] > max_ch_sz) {
                max_ch_sz = sz[v];
                heavy[u] = v;
            }
        }
    }
}

void dfs_hld(int u, int h) {
    head[u] = h;
    pos[u] = cur_pos;
    node_at_pos[cur_pos] = u;
    cur_pos++;
    if (heavy[u]) {
        dfs_hld(heavy[u], h);
    }
    for (int v : adj[u]) {
        if (v != parent_node[u] && v != heavy[u]) {
            dfs_hld(v, v);
        }
    }
}

// --- Lazy Segment Tree ---
struct SegNode {
    long long sum_A;
    long long sum_B_depth;
    long long sum_depth;
    long long lazy_A;
    long long lazy_B;
} tree[MAXN * 4];

void build(int node, int start, int end) {
    tree[node].lazy_A = 0;
    tree[node].lazy_B = 0;
    if (start == end) {
        tree[node].sum_A = 0;
        tree[node].sum_B_depth = 0;
        tree[node].sum_depth = depth[node_at_pos[start]];
        return;
    }
    int mid = (start + end) / 2;
    build(node * 2, start, mid);
    build(node * 2 + 1, mid + 1, end);
    tree[node].sum_depth = (tree[node * 2].sum_depth + tree[node * 2 + 1].sum_depth) % MOD;
    tree[node].sum_A = 0;
    tree[node].sum_B_depth = 0;
}

void push(int node, int start, int end) {
    int mid = (start + end) / 2;
    
    // Left Child
    if (tree[node].lazy_A) {
        long long len = (mid - start + 1);
        tree[node * 2].lazy_A = (tree[node * 2].lazy_A + tree[node].lazy_A) % MOD;
        tree[node * 2].sum_A = (tree[node * 2].sum_A + tree[node].lazy_A * len) % MOD;
    }
    if (tree[node].lazy_B) {
        tree[node * 2].lazy_B = (tree[node * 2].lazy_B + tree[node].lazy_B) % MOD;
        tree[node * 2].sum_B_depth = (tree[node * 2].sum_B_depth + tree[node].lazy_B * tree[node * 2].sum_depth) % MOD;
    }
    
    // Right Child
    if (tree[node].lazy_A) {
        long long len = (end - mid);
        tree[node * 2 + 1].lazy_A = (tree[node * 2 + 1].lazy_A + tree[node].lazy_A) % MOD;
        tree[node * 2 + 1].sum_A = (tree[node * 2 + 1].sum_A + tree[node].lazy_A * len) % MOD;
    }
    if (tree[node].lazy_B) {
        tree[node * 2 + 1].lazy_B = (tree[node * 2 + 1].lazy_B + tree[node].lazy_B) % MOD;
        tree[node * 2 + 1].sum_B_depth = (tree[node * 2 + 1].sum_B_depth + tree[node].lazy_B * tree[node * 2 + 1].sum_depth) % MOD;
    }
    
    tree[node].lazy_A = 0;
    tree[node].lazy_B = 0;
}

void update(int node, int start, int end, int l, int r, long long val_A, long long val_B) {
    if (r < start || end < l) return;
    if (l <= start && end <= r) {
        long long len = (end - start + 1);
        tree[node].lazy_A = (tree[node].lazy_A + val_A) % MOD;
        tree[node].sum_A = (tree[node].sum_A + val_A * len) % MOD;
        
        tree[node].lazy_B = (tree[node].lazy_B + val_B) % MOD;
        tree[node].sum_B_depth = (tree[node].sum_B_depth + val_B * tree[node].sum_depth) % MOD;
        return;
    }
    push(node, start, end);
    int mid = (start + end) / 2;
    update(node * 2, start, mid, l, r, val_A, val_B);
    update(node * 2 + 1, mid + 1, end, l, r, val_A, val_B);
    tree[node].sum_A = (tree[node * 2].sum_A + tree[node * 2 + 1].sum_A) % MOD;
    tree[node].sum_B_depth = (tree[node * 2].sum_B_depth + tree[node * 2 + 1].sum_B_depth) % MOD;
}

long long query(int node, int start, int end, int l, int r) {
    if (r < start || end < l) return 0;
    if (l <= start && end <= r) {
        return (tree[node].sum_A + tree[node].sum_B_depth) % MOD;
    }
    push(node, start, end);
    int mid = (start + end) / 2;
    long long p1 = query(node * 2, start, mid, l, r);
    long long p2 = query(node * 2 + 1, mid + 1, end, l, r);
    return (p1 + p2) % MOD;
}

// --- HLD Path Query ---
long long query_path(int u, int v, int n) {
    long long res = 0;
    while (head[u] != head[v]) {
        if (depth[head[u]] < depth[head[v]]) swap(u, v);
        res = (res + query(1, 1, n, pos[head[u]], pos[u])) % MOD;
        u = parent_node[head[u]];
    }
    if (depth[u] > depth[v]) swap(u, v);
    res = (res + query(1, 1, n, pos[u], pos[v])) % MOD;
    return res;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n, e, r_node;
    if (!(cin >> n >> e >> r_node)) return 0;
    
    for (int i = 0; i < n - 1; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    
    dfs_sz(r_node, 0, 0);
    dfs_hld(r_node, r_node);
    build(1, 1, n);
    
    while (e--) {
        char type;
        cin >> type;
        if (type == 'U') {
            int t;
            long long v, k;
            cin >> t >> v >> k;
            
            long long val_B = k % MOD;
            long long val_A = (v - (depth[t] * k) % MOD + MOD) % MOD;
            
            int l = pos[t];
            int r = pos[t] + sz[t] - 1;
            update(1, 1, n, l, r, val_A, val_B);
        } else if (type == 'Q') {
            int a, b;
            cin >> a >> b;
            long long ans = query_path(a, b, n);
            cout << (ans + MOD) % MOD << "\n";
        }
    }
    
    return 0;
}


// Synced seamlessly with LeetHub Pro
// Pro features: https://bit.ly/leethubpro | Free version: https://bit.ly/leethubv4
// Get it here: https://chromewebstore.google.com/detail/bcilpkkbokcopmabingnndookdogmbna