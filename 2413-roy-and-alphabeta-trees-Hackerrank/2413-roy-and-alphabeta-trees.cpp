#include <bits/stdc++.h>

using namespace std;

string ltrim(const string &);
string rtrim(const string &);
vector<string> split(const string &);

// The special modulo used for this specific problem
long long MOD = 1000000009;

int solve(int alpha, int beta, vector<int> a) {
    int n = a.size();
    sort(a.begin(), a.end());

    // Precompute Catalan numbers up to n to represent empty/full subtree structural counts
    vector<long long> catalan(n + 1, 0);
    catalan[0] = 1;
    for (int i = 1; i <= n; i++) {
        for (int j = 0; j < i; j++) {
            catalan[i] = (catalan[i] + catalan[j] * catalan[i - 1 - j]) % MOD;
        }
    }

    // DP tables for the sums
    vector<vector<long long>> SumEven(n, vector<long long>(n, 0));
    vector<vector<long long>> SumOdd(n, vector<long long>(n, 0));

    for (int len = 1; len <= n; len++) {
        for (int i = 0; i <= n - len; i++) {
            int j = i + len - 1;
            long long total_even = 0;
            long long total_odd = 0;

            for (int k = i; k <= j; k++) {
                int left_len = k - i;
                int right_len = j - k;

                long long left_cat = catalan[left_len];
                long long right_cat = catalan[right_len];
                long long structures_with_k_root = (left_cat * right_cat) % MOD;

                // Root contributes to the even sum level
                long long root_val = a[k] % MOD;
                long long even_from_root = (root_val * structures_with_k_root) % MOD;

                long long left_even = (k > i) ? SumEven[i][k - 1] : 0;
                long long left_odd = (k > i) ? SumOdd[i][k - 1] : 0;
                long long right_even = (k < j) ? SumEven[k + 1][j] : 0;
                long long right_odd = (k < j) ? SumOdd[k + 1][j] : 0;

                // Child level transitions: subtrees flip roles (even components become odd, odd become even)
                long long even_from_subtrees = ((left_odd * right_cat) % MOD + (right_odd * left_cat) % MOD) % MOD;
                long long odd_from_subtrees = ((left_even * right_cat) % MOD + (right_even * left_cat) % MOD) % MOD;

                total_even = (total_even + even_from_root + even_from_subtrees) % MOD;
                total_odd = (total_odd + odd_from_subtrees) % MOD;
            }

            SumEven[i][j] = total_even;
            SumOdd[i][j] = total_odd;
        }
    }

    long long final_even = SumEven[0][n - 1];
    long long final_odd = SumOdd[0][n - 1];

    long long term1 = (final_even * (alpha % MOD)) % MOD;
    long long term2 = (final_odd * (beta % MOD)) % MOD;

    long long ans = (term1 - term2) % MOD;
    if (ans < 0) {
        ans = (ans + MOD) % MOD;
    }

    return ans;
}

int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    string t_temp;
    getline(cin, t_temp);

    int t = stoi(ltrim(rtrim(t_temp)));

    for (int t_itr = 0; t_itr < t; t_itr++) {
        string n_temp;
        getline(cin, n_temp);

        int n = stoi(ltrim(rtrim(n_temp)));

        string first_multiple_input_temp;
        getline(cin, first_multiple_input_temp);

        vector<string> first_multiple_input = split(rtrim(first_multiple_input_temp));

        int alpha = stoi(first_multiple_input[0]);
        int beta = stoi(first_multiple_input[1]);

        string a_temp_temp;
        getline(cin, a_temp_temp);

        vector<string> a_temp = split(rtrim(a_temp_temp));

        vector<int> a(n);
        for (int i = 0; i < n; i++) {
            a[i] = stoi(a_temp[i]);
        }

        int result = solve(alpha, beta, a);

        fout << result << "\n";
    }

    fout.close();

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