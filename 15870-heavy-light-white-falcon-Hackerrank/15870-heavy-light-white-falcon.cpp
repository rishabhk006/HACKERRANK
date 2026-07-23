#include <bits/stdc++.h>

using namespace std;

string ltrim(const string &);
string rtrim(const string &);
vector<string> split(const string &);

// Global arrays for Heavy-Light Decomposition and Segment Tree
int parent_node[50005], depth[50005], heavy[50005], head[50005], pos[50005];
int sz[50005];
int cur_pos;
int tree_seg[4 * 50005];
vector<int> adj[50005];

void dfs(int v, int p) {
    sz[v] = 1;
    int max_c_size = 0;
    for (int c : adj[v]) {
        if (c != p) {
            parent_node[c] = v;
            depth[c] = depth[v] + 1;
            dfs(c, v);
            sz[v] += sz[c];
            if (sz[c] > max_c_size) {
                max_c_size = sz[c];
                heavy[v] = c;
            }
        }
    }
}

void decompose(int v, int h) {
    head[v] = h;
    pos[v] = cur_pos++;
    if (heavy[v] != -1) {
        decompose(heavy[v], h);
    }
    for (int c : adj[v]) {
        if (c != parent_node[v] && c != heavy[v]) {
            decompose(c, c);
        }
    }
}

void update_seg(int node, int start, int end, int idx, int val) {
    if (start == end) {
        tree_seg[node] = val;
        return;
    }
    int mid = (start + end) / 2;
    if (idx <= mid)
        update_seg(2 * node, start, mid, idx, val);
    else
        update_seg(2 * node + 1, mid + 1, end, idx, val);
    tree_seg[node] = max(tree_seg[2 * node], tree_seg[2 * node + 1]);
}

int query_seg(int node, int start, int end, int l, int r) {
    if (r < start || end < l)
        return 0;
    if (l <= start && end <= r)
        return tree_seg[node];
    int mid = (start + end) / 2;
    return max(query_seg(2 * node, start, mid, l, r), query_seg(2 * node + 1, mid + 1, end, l, r));
}

int query_hld(int a, int b, int n) {
    int res = 0;
    for (; head[a] != head[b]; b = parent_node[head[b]]) {
        if (depth[head[a]] > depth[head[b]])
            swap(a, b);
        int cur_max = query_seg(1, 0, n - 1, pos[head[b]], pos[b]);
        res = max(res, cur_max);
    }
    if (depth[a] > depth[b])
        swap(a, b);
    int last_max = query_seg(1, 0, n - 1, pos[a], pos[b]);
    res = max(res, last_max);
    return res;
}

vector<int> solve(vector<vector<int>> tree, vector<vector<int>> queries) {
    int n = tree.size() + 1;
    for (int i = 0; i < n; i++) {
        adj[i].clear();
        heavy[i] = -1;
    }
    for (const auto& edge : tree) {
        adj[edge[0]].push_back(edge[1]);
        adj[edge[1]].push_back(edge[0]);
    }
    
    cur_pos = 0;
    parent_node[0] = 0;
    depth[0] = 0;
    dfs(0, -1);
    decompose(0, 0);

    memset(tree_seg, 0, sizeof(tree_seg));

    vector<int> result;
    for (const auto& q : queries) {
        int type = q[0];
        if (type == 1) {
            int u = q[1];
            int x = q[2];
            update_seg(1, 0, n - 1, pos[u], x);
        } else if (type == 2) {
            int u = q[1];
            int v = q[2];
            result.push_back(query_hld(u, v, n));
        }
    }
    return result;
}

int main()
{
    // Redirect output to stdout (cout) instead of a file for custom input testing
    string first_multiple_input_temp;
    if (!getline(cin, first_multiple_input_temp) || first_multiple_input_temp.empty()) return 0;

    vector<string> first_multiple_input = split(rtrim(first_multiple_input_temp));

    int n = stoi(first_multiple_input[0]);
    int q = stoi(first_multiple_input[1]);

    vector<vector<int>> tree(n - 1, vector<int>(2));

    for (int i = 0; i < n - 1; i++) {
        string tree_row_temp_temp;
        getline(cin, tree_row_temp_temp);

        vector<string> tree_row_temp = split(rtrim(tree_row_temp_temp));

        for (int j = 0; j < 2; j++) {
            tree[i][j] = stoi(tree_row_temp[j]);
        }
    }

    vector<vector<int>> queries(q, vector<int>(3));

    for (int i = 0; i < q; i++) {
        string queries_row_temp_temp;
        getline(cin, queries_row_temp_temp);

        vector<string> queries_row_temp = split(rtrim(queries_row_temp_temp));

        for (int j = 0; j < 3; j++) {
            queries[i][j] = stoi(queries_row_temp[j]);
        }
    }

    vector<int> result = solve(tree, queries);

    for (size_t i = 0; i < result.size(); i++) {
        cout << result[i] << "\n";
    }

    return 0;
}

string ltrim(const string &str) {
    string s(str);
    s.erase(
        s.begin(),
        find_if(s.begin(), s.end(), [](unsigned char ch) { return !isspace(ch); })
    );
    return s;
}

string rtrim(const string &str) {
    string s(str);
    s.erase(
        find_if(s.rbegin(), s.rend(), [](unsigned char ch) { return !isspace(ch); }).base(),
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