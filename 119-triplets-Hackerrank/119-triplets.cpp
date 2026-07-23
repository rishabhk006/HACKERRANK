#include <bits/stdc++.h>

using namespace std;

string ltrim(const string &);
string rtrim(const string &);
vector<string> split(const string &);

// Fenwick Tree with Point Update (Set) and Range Query
class FenwickTree {
private:
    int n;
    vector<long long> tree;
    vector<long long> val_at; // Stores current value at each index to perform set()

public:
    FenwickTree(int n) : n(n), tree(n + 1, 0), val_at(n + 1, 0) {}

    void add(int idx, long long delta) {
        for (; idx <= n; idx += idx & -idx) tree[idx] += delta;
    }

    // Set value at idx
    void set(int idx, long long val) {
        long long delta = val - val_at[idx];
        val_at[idx] = val;
        add(idx, delta);
    }

    long long query(int idx) {
        long long sum = 0;
        for (; idx > 0; idx -= idx & -idx) sum += tree[idx];
        return sum;
    }
};

/*
 * Complete the 'solve' function below.
 *
 * The function is expected to return a LONG_INTEGER.
 * The function accepts LONG_INTEGER_ARRAY d as parameter.
 */
long solve(vector<long> d) {
    int n = d.size();
    if (n < 3) return 0;

    // Coordinate Compression
    vector<long> vals = d;
    sort(vals.begin(), vals.end());
    vals.erase(unique(vals.begin(), vals.end()), vals.end());
    int U = vals.size();

    vector<int> h(n);
    for (int i = 0; i < n; i++) {
        h[i] = lower_bound(vals.begin(), vals.end(), d[i]) - vals.begin() + 1;
    }

    FenwickTree T1(U), T2(U), T3(U);

    for (int i = 0; i < n; i++) {
        int v = h[i];
        T1.set(v, 1);
        T2.set(v, T1.query(v - 1));
        T3.set(v, T2.query(v - 1));
    }

    return T3.query(U);
}

int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    string d_count_temp;
    getline(cin, d_count_temp);

    int d_count = stoi(ltrim(rtrim(d_count_temp)));

    string d_temp_temp;
    getline(cin, d_temp_temp);

    vector<string> d_temp = split(rtrim(d_temp_temp));

    vector<long> d(d_count);

    for (int i = 0; i < d_count; i++) {
        long d_item = stol(d_temp[i]);

        d[i] = d_item;
    }

    long result = solve(d);

    if (getenv("OUTPUT_PATH")) {
        fout << result << "\n";
        fout.close();
    } else {
        cout << result << "\n";
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