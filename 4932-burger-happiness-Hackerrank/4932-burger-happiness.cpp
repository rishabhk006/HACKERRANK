#include <bits/stdc++.h>

using namespace std;

string ltrim(const string &);
string rtrim(const string &);
vector<string> split(const string &);

const long long INF = 1e18;

class SegmentTree {
    int n;
    vector<long long> tree;
    vector<long long> lazy;

    void push(int node) {
        if (lazy[node] != 0) {
            tree[2 * node] += lazy[node];
            lazy[2 * node] += lazy[node];
            tree[2 * node + 1] += lazy[node];
            lazy[2 * node + 1] += lazy[node];
            lazy[node] = 0;
        }
    }

    void update_range(int node, int start, int end, int l, int r, long long val) {
        if (r < start || end < l) return;
        if (l <= start && end <= r) {
            tree[node] += val;
            lazy[node] += val;
            return;
        }
        push(node);
        int mid = (start + end) / 2;
        update_range(2 * node, start, mid, l, r, val);
        update_range(2 * node + 1, mid + 1, end, l, r, val);
        tree[node] = max(tree[2 * node], tree[2 * node + 1]);
    }

    void update_point(int node, int start, int end, int idx, long long val) {
        if (start == end) {
            tree[node] = max(tree[node], val);
            return;
        }
        push(node);
        int mid = (start + end) / 2;
        if (idx <= mid) update_point(2 * node, start, mid, idx, val);
        else update_point(2 * node + 1, mid + 1, end, idx, val);
        tree[node] = max(tree[2 * node], tree[2 * node + 1]);
    }

    long long query(int node, int start, int end, int l, int r) {
        if (r < start || end < l) return -INF;
        if (l <= start && end <= r) return tree[node];
        push(node);
        int mid = (start + end) / 2;
        return max(query(2 * node, start, mid, l, r), query(2 * node + 1, mid + 1, end, l, r));
    }

public:
    SegmentTree(int n) {
        this->n = n;
        tree.assign(4 * n, -INF);
        lazy.assign(4 * n, 0);
    }

    void add_range(int l, int r, long long val) {
        if (l > r) return;
        update_range(1, 0, n - 1, l, r, val);
    }

    void set_point(int idx, long long val) {
        update_point(1, 0, n - 1, idx, val);
    }

    long long get_max(int l, int r) {
        if (l > r) return -INF;
        return query(1, 0, n - 1, l, r);
    }
};

struct Fenwick {
    int n;
    vector<long long> tree;
    Fenwick(int n) : n(n), tree(n + 1, 0) {}
    void add(int i, long long delta) {
        for (i++; i <= n; i += i & -i) tree[i] += delta;
    }
    long long query(int i) {
        long long sum = 0;
        for (i++; i > 0; i -= i & -i) sum += tree[i];
        return sum;
    }
};

int solve(vector<vector<int>> restaurants) {
    int n = restaurants.size();
    vector<int> coords;
    for (int i = 0; i < n; i++) {
        coords.push_back(restaurants[i][0]);
    }
    sort(coords.begin(), coords.end());
    coords.erase(unique(coords.begin(), coords.end()), coords.end());
    int m = coords.size();

    SegmentTree tree_left(m), tree_right(m);
    Fenwick bit(m);

    long long global_max = 0;

    for (int i = 0; i < n; i++) {
        int x = restaurants[i][0];
        long long a = restaurants[i][1];
        long long b = restaurants[i][2];

        int idx = lower_bound(coords.begin(), coords.end(), x) - coords.begin();

        long long p_minus = bit.query(idx - 1);
        long long p_plus = bit.query(idx);

        long long max_left = tree_left.get_max(0, idx - 1);
        long long max_right = tree_right.get_max(idx + 1, m - 1);

        long long current_dp = a;
        if (max_left > -INF) {
            current_dp = max(current_dp, a + max_left - p_minus);
        }
        if (max_right > -INF) {
            current_dp = max(current_dp, a + max_right + p_plus);
        }

        global_max = max(global_max, current_dp);

        tree_left.set_point(idx, current_dp + p_minus);
        tree_right.set_point(idx, current_dp - p_plus);

        tree_left.add_range(idx + 1, m - 1, b);
        tree_right.add_range(idx, m - 1, -b);
        bit.add(idx, b);
    }

    return global_max;
}

int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    string n_temp;
    getline(cin, n_temp);

    int n = stoi(ltrim(rtrim(n_temp)));

    vector<vector<int>> restaurants(n);

    for (int i = 0; i < n; i++) {
        restaurants[i].resize(3);

        string restaurants_row_temp_temp;
        getline(cin, restaurants_row_temp_temp);

        vector<string> restaurants_row_temp = split(rtrim(restaurants_row_temp_temp));

        for (int j = 0; j < 3; j++) {
            int restaurants_row_item = stoi(restaurants_row_temp[j]);

            restaurants[i][j] = restaurants_row_item;
        }
    }

    int result = solve(restaurants);

    fout << result << "\n";

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