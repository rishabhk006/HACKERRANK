#include <bits/stdc++.h>

using namespace std;

string ltrim(const string &);
string rtrim(const string &);
vector<string> split(const string &);

struct CentroidSolver {
    int n;
    long long H, V;
    vector<long long> X, Y;
    vector<vector<int>> adj;
    vector<int> sz;
    vector<bool> removed;
    long long valid_pairs;

    CentroidSolver(int n, long long h, long long v, const vector<vector<int>>& junctions, const vector<vector<int>>& edges) {
        this->n = n;
        H = h;
        V = v;
        X.resize(n);
        Y.resize(n);
        for (int i = 0; i < n; i++) {
            X[i] = junctions[i][0];
            Y[i] = junctions[i][1];
        }
        adj.resize(n);
        for (const auto& e : edges) {
            int u = e[0] - 1; // Convert to 0-indexed
            int v = e[1] - 1; // Convert to 0-indexed
            adj[u].push_back(v);
            adj[v].push_back(u);
        }
        sz.resize(n);
        removed.assign(n, false);
        valid_pairs = 0;
    }

    int get_sizes(int u, int p) {
        sz[u] = 1;
        for (int v : adj[u]) {
            if (v != p && !removed[v]) {
                sz[u] += get_sizes(v, u);
            }
        }
        return sz[u];
    }

    int get_centroid(int u, int p, int total) {
        for (int v : adj[u]) {
            if (v != p && !removed[v] && sz[v] * 2 > total) {
                return get_centroid(v, u, total);
            }
        }
        return u;
    }

    void get_dists(int u, int p, long long dx, long long dy, vector<pair<long long, long long>>& dists) {
        dists.push_back({dx, dy});
        for (int v : adj[u]) {
            if (v != p && !removed[v]) {
                get_dists(v, u, dx + abs(X[u] - X[v]), dy + abs(Y[u] - Y[v]), dists);
            }
        }
    }

    long long count_pairs(vector<pair<long long, long long>>& dists) {
        if (dists.empty()) return 0;
        sort(dists.begin(), dists.end());
        int m = dists.size();
        
        vector<long long> ys;
        ys.reserve(m);
        for (int i = 0; i < m; i++) ys.push_back(dists[i].second);
        sort(ys.begin(), ys.end());
        ys.erase(unique(ys.begin(), ys.end()), ys.end());
        
        int K = ys.size();
        vector<int> bit(K + 1, 0);
        auto add = [&](int idx, int val) {
            for (; idx <= K; idx += idx & -idx) bit[idx] += val;
        };
        auto query = [&](int idx) {
            int sum = 0;
            for (; idx > 0; idx -= idx & -idx) sum += bit[idx];
            return sum;
        };
        
        long long total_ordered = 0;
        long long self_pairs = 0;
        int ptr = 0;
        
        for (int i = m - 1; i >= 0; i--) {
            while (ptr < m && dists[ptr].first <= H - dists[i].first) {
                int y_idx = lower_bound(ys.begin(), ys.end(), dists[ptr].second) - ys.begin() + 1;
                add(y_idx, 1);
                ptr++;
            }
            long long target_y = V - dists[i].second;
            int y_idx = upper_bound(ys.begin(), ys.end(), target_y) - ys.begin();
            total_ordered += query(y_idx);
            
            if (dists[i].first * 2 <= H && dists[i].second * 2 <= V) {
                self_pairs++;
            }
        }
        
        return (total_ordered - self_pairs) / 2;
    }

    void solve_tree(int u) {
        int total = get_sizes(u, -1);
        int C = get_centroid(u, -1, total);
        
        // Count pairs going through or terminating at the centroid
        vector<pair<long long, long long>> all_dists;
        get_dists(C, -1, 0, 0, all_dists);
        valid_pairs += count_pairs(all_dists);
        
        removed[C] = true;
        
        // Remove overcounted pairs that strictly belong to the same subtree
        for (int v : adj[C]) {
            if (!removed[v]) {
                vector<pair<long long, long long>> sub_dists;
                get_dists(v, C, abs(X[C] - X[v]), abs(Y[C] - Y[v]), sub_dists);
                valid_pairs -= count_pairs(sub_dists);
                solve_tree(v);
            }
        }
    }

    long long get_invalid_pairs() {
        solve_tree(0);
        long long total_pairs = (1LL * n * (n - 1)) / 2;
        return total_pairs - valid_pairs;
    }
};

/*
 * Complete the 'solve' function below.
 *
 * The function is expected to return a LONG_INTEGER.
 * The function accepts following parameters:
 *  1. LONG_INTEGER h
 *  2. LONG_INTEGER v
 *  3. 2D_INTEGER_ARRAY junctions
 *  4. 2D_INTEGER_ARRAY edges
 */
long long solve(long h, long v, vector<vector<int>> junctions, vector<vector<int>> edges) {
    int n = junctions.size();
    if (n <= 1) return 0;
    CentroidSolver solver(n, h, v, junctions, edges);
    return solver.get_invalid_pairs();
}

int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    string first_multiple_input_temp;
    getline(cin, first_multiple_input_temp);

    vector<string> first_multiple_input = split(rtrim(first_multiple_input_temp));

    int n = stoi(first_multiple_input[0]);
    long h = stol(first_multiple_input[1]);
    long v = stol(first_multiple_input[2]);

    vector<vector<int>> junctions(n);

    for (int i = 0; i < n; i++) {
        junctions[i].resize(2);

        string junctions_row_temp_temp;
        getline(cin, junctions_row_temp_temp);

        vector<string> junctions_row_temp = split(rtrim(junctions_row_temp_temp));

        for (int j = 0; j < 2; j++) {
            int junctions_row_item = stoi(junctions_row_temp[j]);
            junctions[i][j] = junctions_row_item;
        }
    }

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

    long long result = solve(h, v, junctions, edges);

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