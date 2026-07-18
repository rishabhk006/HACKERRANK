#include <bits/stdc++.h>

using namespace std;

string ltrim(const string &);
string rtrim(const string &);
vector<string> split(const string &);

struct Fenwick {
    int size;
    vector<long long> tree;
    Fenwick(int n) : size(n), tree(n + 2, 0) {}
    void add(int i, long long delta) {
        for (; i <= size; i += i & -i) tree[i] += delta;
    }
    void range_add(int l, int r, long long delta) {
        add(l, delta);
        add(r + 1, -delta);
    }
    long long query(int i) {
        long long sum = 0;
        for (; i > 0; i -= i & -i) sum += tree[i];
        return sum;
    }
};

int timer;
vector<int> tin, tout, depth;
vector<vector<int>> up;
int l;

void dfs(int v, int p, const vector<vector<int>>& adj) {
    tin[v] = ++timer;
    up[v][0] = p;
    for (int i = 1; i <= l; ++i) {
        up[v][i] = up[up[v][i - 1]][i - 1];
    }
    for (int u : adj[v]) {
        if (u != p) {
            depth[u] = depth[v] + 1;
            dfs(u, v, adj);
        }
    }
    tout[v] = timer;
}

int get_lca(int u, int v) {
    if (depth[u] < depth[v]) swap(u, v);
    for (int i = l; i >= 0; --i) {
        if (depth[u] - (1 << i) >= depth[v]) {
            u = up[u][i];
        }
    }
    if (u == v) return u;
    for (int i = l; i >= 0; --i) {
        if (up[u][i] != up[v][i]) {
            u = up[u][i];
            v = up[v][i];
        }
    }
    return up[u][0];
}

vector<int> solve(vector<vector<int>> tree, vector<vector<int>> queries) {
    int n = tree.size() + 1;
    vector<vector<int>> adj(n);
    for (const auto& edge : tree) {
        adj[edge[0]].push_back(edge[1]);
        adj[edge[1]].push_back(edge[0]);
    }

    timer = 0;
    l = ceil(log2(n));
    if (l < 1) l = 1;
    tin.assign(n, 0);
    tout.assign(n, 0);
    depth.assign(n, 0);
    up.assign(n, vector<int>(l + 1, 0));

    dfs(0, 0, adj);

    Fenwick bit(n);
    vector<long long> current_vals(n, 0);
    vector<int> result;

    for (const auto& q : queries) {
        if (q[0] == 1) {
            int u = q[1];
            long long x = q[2];
            long long delta = x - current_vals[u];
            current_vals[u] = x;
            bit.range_add(tin[u], tout[u], delta);
        } else {
            int u = q[1];
            int v = q[2];
            int lca = get_lca(u, v);
            long long ans = bit.query(tin[u]) + bit.query(tin[v]) - bit.query(tin[lca]);
            if (lca != 0) {
                ans -= bit.query(tin[up[lca][0]]);
            }
            result.push_back((int)ans);
        }
    }

    return result;
}

int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    string first_multiple_input_temp;
    getline(cin, first_multiple_input_temp);

    vector<string> first_multiple_input = split(rtrim(first_multiple_input_temp));

    int n = stoi(first_multiple_input[0]);

    int q = stoi(first_multiple_input[1]);

    vector<vector<int>> tree(n - 1);

    for (int i = 0; i < n - 1; i++) {
        tree[i].resize(2);

        string tree_row_temp_temp;
        getline(cin, tree_row_temp_temp);

        vector<string> tree_row_temp = split(rtrim(tree_row_temp_temp));

        for (int j = 0; j < 2; j++) {
            int tree_row_item = stoi(tree_row_temp[j]);

            tree[i][j] = tree_row_item;
        }
    }

    vector<vector<int>> queries(q);

    for (int i = 0; i < q; i++) {
        queries[i].resize(3);

        string queries_row_temp_temp;
        getline(cin, queries_row_temp_temp);

        vector<string> queries_row_temp = split(rtrim(queries_row_temp_temp));

        for (int j = 0; j < 3; j++) {
            int queries_row_item = stoi(queries_row_temp[j]);

            queries[i][j] = queries_row_item;
        }
    }

    vector<int> result = solve(tree, queries);

    for (size_t i = 0; i < result.size(); i++) {
        fout << result[i];

        if (i != result.size() - 1) {
            fout << "\n";
        }
    }

    fout << "\n";

    fout.close();

    return 0;
}

string ltrim(const string &str) {
    string s(str);

    s.erase(
        s.begin(),
        find_if(s.begin(), s.end(), not1(ptr_fun<int, int>(isspace)))
    );

    return s;
}

string rtrim(const string &str) {
    string s(str);

    s.erase(
        find_if(s.rbegin(), s.rend(), not1(ptr_fun<int, int>(isspace))).base(),
        s.end()
    );

    return s;
}

vector<string> split(const string &str) {
    vector<string> tokens;

    string::size_type start = 0;
    string::size_type end = 0;

    while ((end = str.find(" ", start)) != string::npos) {
        tokens.push_back(str.substr(start, end - start));

        start = end + 1;
    }

    tokens.push_back(str.substr(start));

    return tokens;
}


// Synced seamlessly with LeetHub Pro
// Pro features: https://bit.ly/leethubpro | Free version: https://bit.ly/leethubv4
// Get it here: https://chromewebstore.google.com/detail/bcilpkkbokcopmabingnndookdogmbna