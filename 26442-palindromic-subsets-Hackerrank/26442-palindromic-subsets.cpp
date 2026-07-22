#include <bits/stdc++.h>

using namespace std;

string ltrim(const string &);
string rtrim(const string &);
vector<string> split(const string &);

static const int MOD = 1e9 + 7;

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

struct Node {
    int freq[26];
    int lazy;

    Node() {
        memset(freq, 0, sizeof(freq));
        lazy = 0;
    }
};

class SegmentTree {
    int n;
    vector<Node> tree;

    void applyShift(int node, int shift) {
        shift %= 26;
        if (shift == 0) return;
        
        int temp[26];
        for (int i = 0; i < 26; i++) {
            temp[(i + shift) % 26] = tree[node].freq[i];
        }
        for (int i = 0; i < 26; i++) {
            tree[node].freq[i] = temp[i];
        }
        tree[node].lazy = (tree[node].lazy + shift) % 26;
    }

    void push(int node) {
        if (tree[node].lazy != 0) {
            applyShift(2 * node, tree[node].lazy);
            applyShift(2 * node + 1, tree[node].lazy);
            tree[node].lazy = 0;
        }
    }

    void merge(int node) {
        for (int i = 0; i < 26; i++) {
            tree[node].freq[i] = tree[2 * node].freq[i] + tree[2 * node + 1].freq[i];
        }
    }

public:
    SegmentTree(const string& s) {
        n = s.size();
        tree.resize(4 * n);
        build(1, 0, n - 1, s);
    }

    void build(int node, int start, int end, const string& s) {
        if (start == end) {
            tree[node].freq[s[start] - 'a'] = 1;
            return;
        }
        int mid = start + (end - start) / 2;
        build(2 * node, start, mid, s);
        build(2 * node + 1, mid + 1, end, s);
        merge(node);
    }

    void updateRange(int node, int start, int end, int l, int r, int val) {
        if (r < start || end < l) return;
        if (l <= start && end <= r) {
            applyShift(node, val);
            return;
        }
        push(node);
        int mid = start + (end - start) / 2;
        updateRange(2 * node, start, mid, l, r, val);
        updateRange(2 * node + 1, mid + 1, end, l, r, val);
        merge(node);
    }

    void queryRange(int node, int start, int end, int l, int r, vector<int>& res) {
        if (r < start || end < l) return;
        if (l <= start && end <= r) {
            for (int i = 0; i < 26; i++) {
                res[i] += tree[node].freq[i];
            }
            return;
        }
        push(node);
        int mid = start + (end - start) / 2;
        queryRange(2 * node, start, mid, l, r, res);
        queryRange(2 * node + 1, mid + 1, end, l, r, res);
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    string first_multiple_input_temp;
    if (!getline(cin, first_multiple_input_temp)) return 0;

    vector<string> first_multiple_input = split(rtrim(first_multiple_input_temp));
    int n = stoi(first_multiple_input[0]);
    int q = stoi(first_multiple_input[1]);

    string s;
    getline(cin, s);

    SegmentTree segTree(s);

    for (int q_itr = 0; q_itr < q; q_itr++) {
        string query_str;
        getline(cin, query_str);
        vector<string> query_tokens = split(rtrim(query_str));

        int type = stoi(query_tokens[0]);

        if (type == 1) {
            int i = stoi(query_tokens[1]);
            int j = stoi(query_tokens[2]);
            int t = stoi(query_tokens[3]);
            segTree.updateRange(1, 0, n - 1, i, j, t);
        } else {
            int i = stoi(query_tokens[1]);
            int j = stoi(query_tokens[2]);

            vector<int> freq(26, 0);
            segTree.queryRange(1, 0, n - 1, i, j, freq);

            int non_zero_count = 0;
            int total_len = 0;
            for (int k = 0; k < 26; k++) {
                if (freq[k] > 0) {
                    non_zero_count++;
                    total_len += freq[k];
                }
            }

            long long ans = (1 + non_zero_count) * power(2, total_len - non_zero_count) % MOD;
            ans = (ans - 1 + MOD) % MOD;

            cout << ans << "\n";
        }
    }

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