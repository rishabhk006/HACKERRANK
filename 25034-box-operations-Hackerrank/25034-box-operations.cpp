#include <bits/stdc++.h>

using namespace std;

string ltrim(const string &);
string rtrim(const string &);
vector<string> split(const string &);

long long floor_div(long long a, long long d) {
    if (a >= 0) return a / d;
    return (a - d + 1) / d;
}

struct Node {
    long long sum;
    long long mn;
    long long mx;
    long long lazy;
};

vector<Node> tree;

void build(int node, int start, int end, const vector<int>& box) {
    if (start == end) {
        tree[node] = {box[start], box[start], box[start], 0};
        return;
    }
    int mid = (start + end) / 2;
    build(2 * node, start, mid, box);
    build(2 * node + 1, mid + 1, end, box);
    tree[node].sum = tree[2 * node].sum + tree[2 * node + 1].sum;
    tree[node].mn = min(tree[2 * node].mn, tree[2 * node + 1].mn);
    tree[node].mx = max(tree[2 * node].mx, tree[2 * node + 1].mx);
    tree[node].lazy = 0;
}

void push(int node, int start, int end) {
    if (tree[node].lazy != 0) {
        long long val = tree[node].lazy;
        int mid = (start + end) / 2;
        
        tree[2 * node].sum += val * (mid - start + 1);
        tree[2 * node].mn += val;
        tree[2 * node].mx += val;
        tree[2 * node].lazy += val;
        
        tree[2 * node + 1].sum += val * (end - mid);
        tree[2 * node + 1].mn += val;
        tree[2 * node + 1].mx += val;
        tree[2 * node + 1].lazy += val;
        
        tree[node].lazy = 0;
    }
}

void update_add(int node, int start, int end, int l, int r, long long val) {
    if (r < start || end < l) return;
    if (l <= start && end <= r) {
        tree[node].sum += val * (end - start + 1);
        tree[node].mn += val;
        tree[node].mx += val;
        tree[node].lazy += val;
        return;
    }
    push(node, start, end);
    int mid = (start + end) / 2;
    update_add(2 * node, start, mid, l, r, val);
    update_add(2 * node + 1, mid + 1, end, l, r, val);
    tree[node].sum = tree[2 * node].sum + tree[2 * node + 1].sum;
    tree[node].mn = min(tree[2 * node].mn, tree[2 * node + 1].mn);
    tree[node].mx = max(tree[2 * node].mx, tree[2 * node + 1].mx);
}

void update_div(int node, int start, int end, int l, int r, long long d) {
    if (r < start || end < l) return;
    if (l <= start && end <= r) {
        long long div_mn = floor_div(tree[node].mn, d);
        long long div_mx = floor_div(tree[node].mx, d);
        if (tree[node].mn - div_mn == tree[node].mx - div_mx) {
            long long diff = div_mn - tree[node].mn;
            tree[node].sum += diff * (end - start + 1);
            tree[node].mn += diff;
            tree[node].mx += diff;
            tree[node].lazy += diff;
            return;
        }
    }
    push(node, start, end);
    int mid = (start + end) / 2;
    update_div(2 * node, start, mid, l, r, d);
    update_div(2 * node + 1, mid + 1, end, l, r, d);
    tree[node].sum = tree[2 * node].sum + tree[2 * node + 1].sum;
    tree[node].mn = min(tree[2 * node].mn, tree[2 * node + 1].mn);
    tree[node].mx = max(tree[2 * node].mx, tree[2 * node + 1].mx);
}

long long query_min(int node, int start, int end, int l, int r) {
    if (r < start || end < l) return LLONG_MAX;
    if (l <= start && end <= r) return tree[node].mn;
    push(node, start, end);
    int mid = (start + end) / 2;
    return min(query_min(2 * node, start, mid, l, r), query_min(2 * node + 1, mid + 1, end, l, r));
}

long long query_sum(int node, int start, int end, int l, int r) {
    if (r < start || end < l) return 0;
    if (l <= start && end <= r) return tree[node].sum;
    push(node, start, end);
    int mid = (start + end) / 2;
    return query_sum(2 * node, start, mid, l, r) + query_sum(2 * node + 1, mid + 1, end, l, r);
}

int main()
{
    string first_multiple_input_temp;
    getline(cin, first_multiple_input_temp);

    vector<string> first_multiple_input = split(rtrim(first_multiple_input_temp));

    int n = stoi(first_multiple_input[0]);
    int q = stoi(first_multiple_input[1]);

    string box_temp_temp;
    getline(cin, box_temp_temp);

    vector<string> box_temp = split(rtrim(box_temp_temp));

    vector<int> box(n);

    for (int i = 0; i < n; i++) {
        int box_item = stoi(box_temp[i]);
        box[i] = box_item;
    }

    tree.resize(4 * n);
    build(1, 0, n - 1, box);

    for (int i = 0; i < q; i++) {
        string type_temp;
        getline(cin, type_temp);
        vector<string> ops = split(rtrim(type_temp));
        int type = stoi(ops[0]);
        int l = stoi(ops[1]);
        int r = stoi(ops[2]);

        if (type == 1) {
            long long c = stoll(ops[3]);
            update_add(1, 0, n - 1, l, r, c);
        } else if (type == 2) {
            long long d = stoll(ops[3]);
            update_div(1, 0, n - 1, l, r, d);
        } else if (type == 3) {
            cout << query_min(1, 0, n - 1, l, r) << "\n";
        } else if (type == 4) {
            cout << query_sum(1, 0, n - 1, l, r) << "\n";
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