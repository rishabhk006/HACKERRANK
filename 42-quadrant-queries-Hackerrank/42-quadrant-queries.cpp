#include <bits/stdc++.h>

using namespace std;

string ltrim(const string &);
string rtrim(const string &);
vector<string> split(const string &);

// Structure representing the counts of points in all 4 quadrants for a range
struct Node {
    int q[4];
    Node() {
        q[0] = q[1] = q[2] = q[3] = 0;
    }
};

const int MAXN = 100005;
Node tree[4 * MAXN];
bool lazyX[4 * MAXN];
bool lazyY[4 * MAXN];

// Combine children results
Node merge_nodes(const Node& a, const Node& b) {
    Node res;
    for (int i = 0; i < 4; ++i) {
        res.q[i] = a.q[i] + b.q[i];
    }
    return res;
}

// Swap quadrants when reflecting along X-axis (Y changes sign)
void applyX(Node& node) {
    swap(node.q[0], node.q[3]); // Q1 <-> Q4
    swap(node.q[1], node.q[2]); // Q2 <-> Q3
}

// Swap quadrants when reflecting along Y-axis (X changes sign)
void applyY(Node& node) {
    swap(node.q[0], node.q[1]); // Q1 <-> Q2
    swap(node.q[2], node.q[3]); // Q3 <-> Q4
}

// Push lazy transformations to children
void push(int id) {
    if (lazyX[id]) {
        lazyX[2 * id] = !lazyX[2 * id];
        lazyX[2 * id + 1] = !lazyX[2 * id + 1];
        applyX(tree[2 * id]);
        applyX(tree[2 * id + 1]);
        lazyX[id] = false;
    }
    if (lazyY[id]) {
        lazyY[2 * id] = !lazyY[2 * id];
        lazyY[2 * id + 1] = !lazyY[2 * id + 1];
        applyY(tree[2 * id]);
        applyY(tree[2 * id + 1]);
        lazyY[id] = false;
    }
}

// Build segment tree
void build(int id, int l, int r, const vector<int>& initial_q) {
    lazyX[id] = false;
    lazyY[id] = false;
    if (l == r) {
        tree[id] = Node();
        tree[id].q[initial_q[l]] = 1;
        return;
    }
    int mid = l + (r - l) / 2;
    build(2 * id, l, mid, initial_q);
    build(2 * id + 1, mid + 1, r, initial_q);
    tree[id] = merge_nodes(tree[2 * id], tree[2 * id + 1]);
}

// Range update for reflections
void update(int id, int l, int r, int ql, int qr, char type) {
    if (ql <= l && r <= qr) {
        if (type == 'X') {
            lazyX[id] = !lazyX[id];
            applyX(tree[id]);
        } else {
            lazyY[id] = !lazyY[id];
            applyY(tree[id]);
        }
        return;
    }
    push(id);
    int mid = l + (r - l) / 2;
    if (ql <= mid) update(2 * id, l, mid, ql, qr, type);
    if (qr > mid) update(2 * id + 1, mid + 1, r, ql, qr, type);
    tree[id] = merge_nodes(tree[2 * id], tree[2 * id + 1]);
}

// Range query for counts
Node query(int id, int l, int r, int ql, int qr) {
    if (ql <= l && r <= qr) {
        return tree[id];
    }
    push(id);
    int mid = l + (r - l) / 2;
    if (qr <= mid) return query(2 * id, l, mid, ql, qr);
    if (ql > mid) return query(2 * id + 1, mid + 1, r, ql, qr);
    return merge_nodes(query(2 * id, l, mid, ql, qr), query(2 * id + 1, mid + 1, r, ql, qr));
}

/*
 * Complete the 'quadrants' function below.
 */
void quadrants(vector<vector<int>> p, vector<string> queries) {
    int n = p.size();
    vector<int> initial_q(n + 1);
    
    // Convert coordinate positions into 0-indexed quadrant identifiers
    for (int i = 0; i < n; i++) {
        int x = p[i][0];
        int y = p[i][1];
        if (x > 0 && y > 0) initial_q[i + 1] = 0; // Q1
        else if (x < 0 && y > 0) initial_q[i + 1] = 1; // Q2
        else if (x < 0 && y < 0) initial_q[i + 1] = 2; // Q3
        else if (x > 0 && y < 0) initial_q[i + 1] = 3; // Q4
    }

    build(1, 1, n, initial_q);

    for (const string& q_str : queries) {
        if (q_str.empty()) continue;
        vector<string> tokens = split(q_str);
        char type = tokens[0][0];
        int i = stoi(tokens[1]);
        int j = stoi(tokens[2]);

        if (type == 'C') {
            Node res = query(1, 1, n, i, j);
            cout << res.q[0] << " " << res.q[1] << " " << res.q[2] << " " << res.q[3] << "\n";
        } else {
            update(1, 1, n, i, j, type);
        }
    }
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    string n_temp;
    if (!getline(cin, n_temp)) return 0;

    int n = stoi(ltrim(rtrim(n_temp)));

    vector<vector<int>> p(n);

    for (int i = 0; i < n; i++) {
        p[i].resize(2);

        string p_row_temp_temp;
        getline(cin, p_row_temp_temp);

        vector<string> p_row_temp = split(rtrim(p_row_temp_temp));

        for (int j = 0; j < 2; j++) {
            int p_row_item = stoi(p_row_temp[j]);
            p[i][j] = p_row_item;
        }
    }

    string q_temp;
    getline(cin, q_temp);

    int q = stoi(ltrim(rtrim(q_temp)));

    vector<string> queries(q);

    for (int i = 0; i < q; i++) {
        string queries_item;
        getline(cin, queries_item);
        queries[i] = queries_item;
    }

    quadrants(p, queries);

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