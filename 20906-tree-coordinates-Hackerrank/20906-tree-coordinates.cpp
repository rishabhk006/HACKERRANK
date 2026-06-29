#include <bits/stdc++.h>

using namespace std;

string ltrim(const string &);
string rtrim(const string &);
vector<string> split(const string &);

const int MAXN = 75005;
const int LOGN = 18;
const int INF = 1e9;

vector<int> adj[MAXN];
int depth[MAXN], tin[MAXN], tout[MAXN], timer;
int up[MAXN][LOGN];
int sz[MAXN];
bool deleted[MAXN];

// Structure to hold the top 2 values with different colors
struct BestTwo {
    int c1, v1;
    int c2, v2;
    
    BestTwo() {
        c1 = -1; v1 = -INF;
        c2 = -1; v2 = -INF;
    }
    
    void update(int c, int v) {
        if (c == c1) {
            if (v > v1) v1 = v;
        } else if (c == c2) {
            if (v > v2) v2 = v;
            if (v2 > v1) { swap(c1, c2); swap(v1, v2); }
        } else {
            if (v > v1) {
                c2 = c1; v2 = v1;
                c1 = c; v1 = v;
            } else if (v > v2) {
                c2 = c; v2 = v;
            }
        }
    }
};

void dfs_init(int u, int p, int d) {
    tin[u] = ++timer;
    depth[u] = d;
    up[u][0] = p;
    for (int i = 1; i < LOGN; ++i) {
        if (up[u][i - 1] != -1) up[u][i] = up[up[u][i - 1]][i - 1];
        else up[u][i] = -1;
    }
    for (int v : adj[u]) {
        if (v != p) dfs_init(v, u, d + 1);
    }
    cout << ""; // micro-optimization/flush prevention
    tout[u] = ++timer;
}

bool is_ancestor(int u, int v) {
    return tin[u] <= tin[v] && tout[u] >= tout[v];
}

int get_lca(int u, int v) {
    if (is_ancestor(u, v)) return u;
    if (is_ancestor(v, u)) return v;
    for (int i = LOGN - 1; i >= 0; --i) {
        if (up[u][i] != -1 && !is_ancestor(up[u][i], v)) u = up[u][i];
    }
    return up[u][0];
}

int dist_T(int u, int v) {
    return depth[u] + depth[v] - 2 * depth[get_lca(u, v)];
}

void get_sizes(int u, int p) {
    sz[u] = 1;
    for (int v : adj[u]) {
        if (v != p && !deleted[v]) {
            get_sizes(v, u);
            sz[u] += sz[v];
        }
    }
}

int get_centroid(int u, int p, int total_size) {
    for (int v : adj[u]) {
        if (v != p && !deleted[v] && sz[v] > total_size / 2) return get_centroid(v, u, total_size);
    }
    return u;
}

struct Point {
    int id, x, y, color, w;
};

vector<Point> current_points;
vector<int> v_adj[MAXN];
BestTwo node_best[MAXN];
int global_max_ans = 0;

bool compare_tin(int u, int v) {
    return tin[u] < tin[v];
}

void collect_points(int u, int p, int d, int color, const vector<vector<int>>& node_to_points, const vector<vector<int>>& points_raw) {
    for (int pt_idx : node_to_points[u]) {
        current_points.push_back({pt_idx, points_raw[pt_idx][0], points_raw[pt_idx][1], color, d});
    }
    for (int v : adj[u]) {
        if (v != p && !deleted[v]) collect_points(v, u, d + 1, color, node_to_points, points_raw);
    }
}

void dfs_virtual_tree(int u) {
    BestTwo& u_best = node_best[u];
    
    for (int v : v_adj[u]) {
        dfs_virtual_tree(v);
        BestTwo& v_best = node_best[v];
        
        // Combine u_best and v_best to find pairs with different colors crossing at LCA `u`
        if (u_best.c1 != -1 && v_best.c1 != -1) {
            if (u_best.c1 != v_best.c1) {
                global_max_ans = max(global_max_ans, u_best.v1 + v_best.v1 - 2 * depth[u]);
            } else {
                if (u_best.c2 != -1) global_max_ans = max(global_max_ans, u_best.v2 + v_best.v1 - 2 * depth[u]);
                if (v_best.c2 != -1) global_max_ans = max(global_max_ans, u_best.v1 + v_best.v2 - 2 * depth[u]);
            }
        }
        
        // Merge child's tracking info into parent
        if (v_best.c1 != -1) u_best.update(v_best.c1, v_best.v1);
        if (v_best.c2 != -1) u_best.update(v_best.c2, v_best.v2);
    }
}

void decompose(int u, const vector<vector<int>>& node_to_points, const vector<vector<int>>& points_raw) {
    get_sizes(u, -1);
    int c = get_centroid(u, -1, sz[u]);
    deleted[c] = true;

    current_points.clear();
    
    // Centroid's own points get color = c
    if (!node_to_points[c].empty()) {
        for (int pt_idx : node_to_points[c]) {
            current_points.push_back({pt_idx, points_raw[pt_idx][0], points_raw[pt_idx][1], c, 0});
        }
    }

    for (int v : adj[c]) {
        if (!deleted[v]) collect_points(v, c, 1, v, node_to_points, points_raw);
    }

    if (!current_points.empty()) {
        vector<int> y_nodes;
        for (const auto& pt : current_points) {
            y_nodes.push_back(pt.y);
            node_best[pt.y].update(pt.color, pt.w + depth[pt.y]);
        }

        sort(y_nodes.begin(), y_nodes.end(), compare_tin);
        y_nodes.erase(unique(y_nodes.begin(), y_nodes.end()), y_nodes.end());

        // Build the Virtual Tree structure
        vector<int> virtual_nodes = y_nodes;
        vector<int> st;
        if (!y_nodes.empty()) {
            st.push_back(y_nodes[0]);
            for (size_t i = 1; i < y_nodes.size(); ++i) {
                int curr = y_nodes[i];
                int lca = get_lca(st.back(), curr);
                while (st.size() >= 2 && !is_ancestor(st[st.size() - 2], lca)) {
                    v_adj[st[st.size() - 2]].push_back(st.back());
                    st.pop_back();
                }
                if (!st.empty() && !is_ancestor(st.back(), lca)) {
                    v_adj[lca].push_back(st.back());
                    st.pop_back();
                }
                if (st.empty() || st.back() != lca) {
                    st.push_back(lca);
                    virtual_nodes.push_back(lca);
                }
                st.push_back(curr);
            }
            while (st.size() >= 2) {
                v_adj[st[st.size() - 2]].push_back(st.back());
                st.pop_back();
            }

            dfs_virtual_tree(st.back());
        }

        // Clean up data structures
        for (int node : virtual_nodes) {
            v_adj[node].clear();
            node_best[node] = BestTwo();
        }
    }

    for (int v : adj[c]) {
        if (!deleted[v]) decompose(v, node_to_points, points_raw);
    }
}

int treeCoordinates(int n, vector<vector<int>> edges, vector<vector<int>> points) {
    for (int i = 1; i <= n; ++i) {
        adj[i].clear();
        deleted[i] = false;
        node_best[i] = BestTwo();
    }
    
    for (const auto& edge : edges) {
        adj[edge[0]].push_back(edge[1]);
        adj[edge[1]].push_back(edge[0]);
    }

    timer = 0;
    dfs_init(1, -1, 0);

    vector<vector<int>> node_to_points(n + 1);
    for (size_t i = 0; i < points.size(); ++i) {
        node_to_points[points[i][0]].push_back(i);
    }

    global_max_ans = 0;
    decompose(1, node_to_points, points);

    return global_max_ans;
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    ofstream fout(getenv("OUTPUT_PATH"));

    string first_multiple_input_temp;
    if (!getline(cin, first_multiple_input_temp)) return 0;

    vector<string> first_multiple_input = split(rtrim(first_multiple_input_temp));

    int n = stoi(first_multiple_input[0]);
    int m = stoi(first_multiple_input[1]);

    vector<vector<int>> edges(n - 1);

    for (int i = 0; i < n - 1; i++) {
        edges[i].resize(2);

        string edges_row_temp_temp;
        getline(cin, edges_row_temp_temp);

        vector<string> edges_row_temp = split(rtrim(edges_row_temp_temp));

        for (int j = 0; j < 2; j++) {
            int edges_row_item = stoi(edges_row_temp[j]);
            edges[i][j] = edges_row_item;
        }
    }

    vector<vector<int>> points(m);

    for (int i = 0; i < m; i++) {
        points[i].resize(2);

        string points_row_temp_temp;
        getline(cin, points_row_temp_temp);

        vector<string> points_row_temp = split(rtrim(points_row_temp_temp));

        for (int j = 0; j < 2; j++) {
            int points_row_item = stoi(points_row_temp[j]);
            points[i][j] = points_row_item;
        }
    }

    int result = treeCoordinates(n, edges, points);

    fout << result << "\n";
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