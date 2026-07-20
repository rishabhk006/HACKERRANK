#include <bits/stdc++.h>

using namespace std;

string ltrim(const string &);
string rtrim(const string &);
vector<string> split(const string &);

const int MOD = 1000000007;
const long long inv2 = 500000004;
const long long inv6 = 166666668;

long long binom3(long long x) {
    if (x < 3) return 0;
    x %= MOD;
    return x * (x - 1) % MOD * (x - 2) % MOD * inv6 % MOD;
}

long long calc_S1(long long m, long long S) {
    m %= MOD;
    S %= MOD;
    long long term1 = m * ((S * S % MOD - S + MOD) % MOD) % MOD;
    long long term2 = (4 * S - 2 + MOD) % MOD * (m * (m - 1 + MOD) % MOD) % MOD * inv2 % MOD;
    long long term3 = 4 * (m * (m - 1 + MOD) % MOD * (2 * m - 1 + MOD) % MOD) % MOD * inv6 % MOD;
    long long res = (term1 + term2 + term3) % MOD;
    res = res * inv2 % MOD;
    return res;
}

/*
 * Complete the 'solve' function below.
 *
 * The function is expected to return an INTEGER.
 * The function accepts INTEGER_ARRAY A as parameter.
 */
int solve(vector<int> A) {
    int n = A.size();
    if (n == 0) return 0;

    vector<pair<int, int>> vals(n);
    for (int i = 0; i < n; i++) {
        vals[i] = {A[i], i};
    }
    sort(vals.begin(), vals.end());

    vector<int> parent(n), sz(n);
    vector<bool> is_left(n, false), is_right(n, false), active(n, false);
    long long middle_sum = 0;
    long long L_left = 0, L_right = 0;

    // C++11 safe structural definition for DSU tracking
    struct DSUHelper {
        vector<int>& parent;
        int find_set(int v) {
            if (v == parent[v]) return v;
            return parent[v] = find_set(parent[v]);
        }
    } dsu{parent};

    auto merge_sets = [&](int a, int b) {
        a = dsu.find_set(a);
        b = dsu.find_set(b);
        if (a != b) {
            if (!is_left[a] && !is_right[a]) {
                middle_sum = (middle_sum - binom3(sz[a] + 2) + MOD) % MOD;
            }
            if (!is_left[b] && !is_right[b]) {
                middle_sum = (middle_sum - binom3(sz[b] + 2) + MOD) % MOD;
            }
            parent[b] = a;
            sz[a] += sz[b];
            is_left[a] = is_left[a] | is_left[b];
            is_right[a] = is_right[a] | is_right[b];

            if (!is_left[a] && !is_right[a]) {
                middle_sum = (middle_sum + binom3(sz[a] + 2)) % MOD;
            }
            if (is_left[a]) L_left = sz[a];
            if (is_right[a]) L_right = sz[a];
        }
    };

    for (int i = 0; i < n; i++) {
        parent[i] = i;
        sz[i] = 1;
    }

    auto get_C = [&]() -> long long {
        if (L_left == n) {
            long long N_mod = (1LL * n * (n + 1) / 2) % MOD;
            return N_mod * (N_mod + 1) % MOD * inv2 % MOD;
        }
        long long res = middle_sum;
        long long L_term = (1LL * L_left * (L_left + 1) / 2) % MOD;
        res = (res + L_term) % MOD;

        long long A_len = L_left;
        long long B_len = L_right;
        long long S1 = 0, S2 = 0;

        if (A_len <= B_len) {
            S2 = binom3(B_len - A_len + 2);
            S1 = calc_S1(A_len, B_len - A_len + 2);
        } else {
            S2 = binom3(A_len - B_len);
            S1 = calc_S1(B_len + 1, A_len - B_len);
        }

        res = (res + S1 + S2) % MOD;
        return res;
    };

    long long total_fB = 0;
    int i = 0;
    long long prev_C = 0;

    while (i < n) {
        int j = i;
        while (j < n && vals[j].first == vals[i].first) {
            int idx = vals[j].second;
            active[idx] = true;
            sz[idx] = 1;
            is_left[idx] = (idx == 0);
            is_right[idx] = (idx == n - 1);

            if (!is_left[idx] && !is_right[idx]) {
                middle_sum = (middle_sum + binom3(sz[idx] + 2)) % MOD;
            }
            if (is_left[idx]) L_left = sz[idx];
            if (is_right[idx]) L_right = sz[idx];

            if (idx > 0 && active[idx - 1]) merge_sets(idx, idx - 1);
            if (idx < n - 1 && active[idx + 1]) merge_sets(idx, idx + 1);

            j++;
        }
        
        long long current_C = get_C();
        long long val = vals[i].first;
        long long diff = (current_C - prev_C + MOD) % MOD;
        total_fB = (total_fB + val * diff) % MOD;

        prev_C = current_C;
        i = j;
    }

    return total_fB;
}

int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    string n_temp;
    getline(cin, n_temp);

    int n = stoi(ltrim(rtrim(n_temp)));

    string A_temp_temp;
    getline(cin, A_temp_temp);

    vector<string> A_temp = split(rtrim(A_temp_temp));

    vector<int> A(n);

    for (int i = 0; i < n; i++) {
        int A_item = stoi(A_temp[i]);

        A[i] = A_item;
    }

    int result = solve(A);

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