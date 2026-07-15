#include <bits/stdc++.h>

using namespace std;

string ltrim(const string &);
string rtrim(const string &);
vector<string> split(const string &);

const long long MOD = 1000000007;

long long power(long long base, long long exp) {
    long long res = 1;
    base %= MOD;
    while (exp > 0) {
        if (exp % 2 == 1) res = (res * base) % MOD;
        base = (base * base) % MOD;
        exp /= 2;
    }
    return res;
}

long long modInverse(long long n) {
    return power(n, MOD - 2);
}

struct DfsHelper {
    const vector<vector<int>>& adj;
    vector<vector<int>>& up;
    vector<int>& depth;
    int LOG;

    void dfs_lca(int u, int p, int d) {
        up[u][0] = p;
        depth[u] = d;
        for (int i = 1; i < LOG; i++) {
            up[u][i] = up[up[u][i - 1]][i - 1];
        }
        for (size_t i = 0; i < adj[u].size(); ++i) {
            int v = adj[u][i];
            if (v != p) dfs_lca(v, u, d + 1);
        }
    }

    vector<long long>& dC0; vector<long long>& dC1; vector<long long>& dC2;
    vector<long long>& dD0; vector<long long>& dD1; vector<long long>& dD2;

    void dfs_diff(int u, int p) {
        for (size_t i = 0; i < adj[u].size(); ++i) {
            int v = adj[u][i];
            if (v != p) {
                dfs_diff(v, u);
                dC0[u] = (dC0[u] + dC0[v]) % MOD;
                dC1[u] = (dC1[u] + dC1[v]) % MOD;
                dC2[u] = (dC2[u] + dC2[v]) % MOD;
                dD0[u] = (dD0[u] + dD0[v]) % MOD;
                dD1[u] = (dD1[u] + dD1[v]) % MOD;
                dD2[u] = (dD2[u] + dD2[v]) % MOD;
            }
        }
    }

    const vector<long long>& V;
    vector<long long>& S;

    void dfs_sum(int u, int p) {
        S[u] = (S[p] + V[u]) % MOD;
        for (size_t i = 0; i < adj[u].size(); ++i) {
            int v = adj[u][i];
            if (v != p) dfs_sum(v, u);
        }
    }
};

/*
 * Complete the 'solve' function below.
 *
 * The function is expected to return an INTEGER_ARRAY.
 * The function accepts following parameters:
 *  1. INTEGER R
 *  2. 2D_INTEGER_ARRAY tree
 *  3. 2D_INTEGER_ARRAY updates
 *  4. 2D_INTEGER_ARRAY queries
 */
vector<int> solve(int R, vector<vector<int>> tree, vector<vector<int>> updates, vector<vector<int>> queries) {
    int N = tree.size() + 1;
    vector<vector<int>> adj(N + 1);
    for (size_t i = 0; i < tree.size(); ++i) {
        adj[tree[i][0]].push_back(tree[i][1]);
        adj[tree[i][1]].push_back(tree[i][0]);
    }

    int LOG = 20;
    vector<vector<int>> up(N + 1, vector<int>(LOG, 0));
    vector<int> depth(N + 1, 0);

    vector<long long> diff_C0(N + 1, 0), diff_C1(N + 1, 0), diff_C2(N + 1, 0);
    vector<long long> diff_D0(N + 1, 0), diff_D1(N + 1, 0), diff_D2(N + 1, 0);
    vector<long long> V(N + 1, 0);
    vector<long long> S(N + 1, 0);

    DfsHelper helper{adj, up, depth, LOG, diff_C0, diff_C1, diff_C2, diff_D0, diff_D1, diff_D2, V, S};
    helper.dfs_lca(1, 0, 0);

    auto get_lca = [&](int u, int v) -> int {
        if (depth[u] < depth[v]) swap(u, v);
        int diff = depth[u] - depth[v];
        for (int i = 0; i < LOG; i++) {
            if ((diff >> i) & 1) u = up[u][i];
        }
        if (u == v) return u;
        for (int i = LOG - 1; i >= 0; i--) {
            if (up[u][i] != up[v][i]) {
                u = up[u][i];
                v = up[v][i];
            }
        }
        return up[u][0];
    };

    int max_depth = 2 * N + 5;
    vector<long long> powR(max_depth, 0), powInvR(max_depth, 0);
    long long invR = modInverse(R);
    powR[0] = 1; powInvR[0] = 1;
    for (int i = 1; i < max_depth; i++) {
        powR[i] = (powR[i - 1] * R) % MOD;
        powInvR[i] = (powInvR[i - 1] * invR) % MOD;
    }

    for (size_t k = 0; k < updates.size(); ++k) {
        const auto& u_req = updates[k];
        long long a1 = u_req[0] % MOD, d1 = u_req[1] % MOD;
        long long a2 = u_req[2] % MOD, d2 = u_req[3] % MOD;
        int A = u_req[4], B = u_req[5];

        long long c0 = (a1 * a2) % MOD;
        long long c1 = (a1 * d2 + a2 * d1) % MOD;
        long long c2 = (d1 * d2) % MOD;

        int L = get_lca(A, B);
        long long dA = depth[A];
        long long dL = depth[L];

        long long R_dA = powR[dA];

        long long K0 = (c0 + c1 * dA + c2 * (dA * dA % MOD)) % MOD;
        K0 = (K0 * R_dA) % MOD;

        long long K1 = (c1 + 2LL * c2 * dA) % MOD;
        K1 = (MOD - K1) % MOD;
        K1 = (K1 * R_dA) % MOD;

        long long K2 = (c2 * R_dA) % MOD;

        diff_C0[A] = (diff_C0[A] + K0) % MOD;
        diff_C1[A] = (diff_C1[A] + K1) % MOD;
        diff_C2[A] = (diff_C2[A] + K2) % MOD;

        int pL = up[L][0];
        diff_C0[pL] = (diff_C0[pL] - K0 + MOD) % MOD;
        diff_C1[pL] = (diff_C1[pL] - K1 + MOD) % MOD;
        diff_C2[pL] = (diff_C2[pL] - K2 + MOD) % MOD;

        if (B != L) {
            long long Y_val = dA - 2LL * dL;
            long long Y = (Y_val % MOD + MOD) % MOD;
            long long R_Y = (Y_val >= 0) ? powR[Y_val] : powInvR[-Y_val];

            long long M0 = (c0 + c1 * Y + c2 * (Y * Y % MOD)) % MOD;
            M0 = (M0 * R_Y) % MOD;

            long long M1 = (c1 + 2LL * c2 * Y) % MOD;
            M1 = (M1 * R_Y) % MOD;

            long long M2 = (c2 * R_Y) % MOD;

            diff_D0[B] = (diff_D0[B] + M0) % MOD;
            diff_D1[B] = (diff_D1[B] + M1) % MOD;
            diff_D2[B] = (diff_D2[B] + M2) % MOD;

            diff_D0[L] = (diff_D0[L] - M0 + MOD) % MOD;
            diff_D1[L] = (diff_D1[L] - M1 + MOD) % MOD;
            diff_D2[L] = (diff_D2[L] - M2 + MOD) % MOD;
        }
    }

    helper.dfs_diff(1, 0);

    for (int i = 1; i <= N; i++) {
        long long du = depth[i];

        long long termC0 = (diff_C0[i] % MOD + MOD) % MOD;
        long long termC1 = ((diff_C1[i] % MOD + MOD) % MOD * du) % MOD;
        long long termC2 = ((diff_C2[i] % MOD + MOD) % MOD * (du * du % MOD)) % MOD;
        long long VC = (termC0 + termC1 + termC2) % MOD;
        VC = (VC * powInvR[du]) % MOD;

        long long termD0 = (diff_D0[i] % MOD + MOD) % MOD;
        long long termD1 = ((diff_D1[i] % MOD + MOD) % MOD * du) % MOD;
        long long termD2 = ((diff_D2[i] % MOD + MOD) % MOD * (du * du % MOD)) % MOD;
        long long VD = (termD0 + termD1 + termD2) % MOD;
        VD = (VD * powR[du]) % MOD;

        V[i] = (VC + VD) % MOD;
    }

    helper.dfs_sum(1, 0);

    vector<int> res;
    res.reserve(queries.size());
    for (size_t k = 0; k < queries.size(); ++k) {
        int i = queries[k][0], j = queries[k][1];
        int lca = get_lca(i, j);
        long long ans = (S[i] + S[j] - S[lca] - S[up[lca][0]]) % MOD;
        ans = (ans % MOD + MOD) % MOD;
        res.push_back((int)ans);
    }

    return res;
}

int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    string first_multiple_input_temp;
    getline(cin, first_multiple_input_temp);

    vector<string> first_multiple_input = split(rtrim(first_multiple_input_temp));

    int n = stoi(first_multiple_input[0]);

    int R = stoi(first_multiple_input[1]);

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

    string second_multiple_input_temp;
    getline(cin, second_multiple_input_temp);

    vector<string> second_multiple_input = split(rtrim(second_multiple_input_temp));

    int u = stoi(second_multiple_input[0]);

    int q = stoi(second_multiple_input[1]);

    vector<vector<int>> updates(u);

    for (int i = 0; i < u; i++) {
        updates[i].resize(6);

        string updates_row_temp_temp;
        getline(cin, updates_row_temp_temp);

        vector<string> updates_row_temp = split(rtrim(updates_row_temp_temp));

        for (int j = 0; j < 6; j++) {
            int updates_row_item = stoi(updates_row_temp[j]);

            updates[i][j] = updates_row_item;
        }
    }

    vector<vector<int>> queries(q);

    for (int i = 0; i < q; i++) {
        queries[i].resize(2);

        string queries_row_temp_temp;
        getline(cin, queries_row_temp_temp);

        vector<string> queries_row_temp = split(rtrim(queries_row_temp_temp));

        for (int j = 0; j < 2; j++) {
            int queries_row_item = stoi(queries_row_temp[j]);

            queries[i][j] = queries_row_item;
        }
    }

    vector<int> result = solve(R, tree, updates, queries);

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