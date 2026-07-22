#include <bits/stdc++.h>

using namespace std;

string ltrim(const string &);
string rtrim(const string &);
vector<string> split(const string &);

static const int MOD = 1e9 + 7;

inline long long S(long long L) {
    return (L * (L + 1) / 2) % MOD;
}

// Computes C(L + 2, 4) % MOD
inline long long C4(long long L) {
    if (L < 2) return 0;
    long long n = L + 2;
    unsigned __int128 prod = (unsigned __int128)n * (n - 1) * (n - 2) * (n - 3) / 24;
    return (long long)(prod % MOD);
}

int getParent(int i, vector<int>& parent) {
    if (parent[i] < 0) return i;
    return parent[i] = getParent(parent[i], parent);
}

/*
 * Complete the 'solve' function below.
 */
int solve(vector<int> arr) {
    int n = arr.size();

    vector<pair<int, int>> elems(n);
    for (int i = 0; i < n; i++) {
        elems[i] = {arr[i], i};
    }

    // Sort descending by element value
    sort(elems.rbegin(), elems.rend());

    vector<int> parent(n + 2, -1);
    vector<long long> len(n + 2, 0);
    vector<bool> active(n + 2, false);

    long long sum_S = 0;
    long long sum_S2 = 0;
    long long sum_C4 = 0;

    long long prev_pairs = 0;
    long long ans = 0;

    for (int i = 0; i < n; ) {
        int j = i;
        while (j < n && elems[j].first == elems[i].first) {
            j++;
        }

        // Activate all elements with current value
        for (int k = i; k < j; k++) {
            int idx = elems[k].second + 1; // 1-based index
            active[idx] = true;
            parent[idx] = -1;
            len[idx] = 1;

            long long left_len = (idx > 1 && active[idx - 1]) ? len[getParent(idx - 1, parent)] : 0;
            long long right_len = (idx < n && active[idx + 1]) ? len[getParent(idx + 1, parent)] : 0;

            // Remove old statistics of adjacent components
            sum_S = (sum_S - S(left_len) - S(right_len) + 2 * MOD) % MOD;
            sum_S2 = (sum_S2 - (S(left_len) * S(left_len)) % MOD - (S(right_len) * S(right_len)) % MOD + 2 * MOD) % MOD;
            sum_C4 = (sum_C4 - C4(left_len) - C4(right_len) + 2 * MOD) % MOD;

            long long new_len = left_len + 1 + right_len;

            // Add new merged block statistics
            sum_S = (sum_S + S(new_len)) % MOD;
            sum_S2 = (sum_S2 + (S(new_len) * S(new_len)) % MOD) % MOD;
            sum_C4 = (sum_C4 + C4(new_len)) % MOD;

            if (left_len > 0) {
                int root_left = getParent(idx - 1, parent);
                parent[root_left] = idx;
            }
            if (right_len > 0) {
                int root_right = getParent(idx + 1, parent);
                parent[root_right] = idx;
            }
            len[idx] = new_len;
        }

        // Cross-block pairs = 0.5 * ((sum_S)^2 - sum_S2)
        long long cross_pairs = ((sum_S * sum_S) % MOD - sum_S2 + MOD) % MOD;
        if (cross_pairs % 2 != 0) cross_pairs += MOD;
        cross_pairs /= 2;

        long long current_pairs = (sum_C4 + cross_pairs) % MOD;
        long long count_exact = (current_pairs - prev_pairs + MOD) % MOD;

        ans = (ans + count_exact * (elems[i].first % MOD)) % MOD;
        prev_pairs = current_pairs;

        i = j;
    }

    return (int)((ans % MOD + MOD) % MOD);
}

int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    string arr_count_temp;
    getline(cin, arr_count_temp);

    int arr_count = stoi(ltrim(rtrim(arr_count_temp)));

    string arr_temp_temp;
    getline(cin, arr_temp_temp);

    vector<string> arr_temp = split(rtrim(arr_temp_temp));

    vector<int> arr(arr_count);

    for (int i = 0; i < arr_count; i++) {
        int arr_item = stoi(arr_temp[i]);

        arr[i] = arr_item;
    }

    int result = solve(arr);

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