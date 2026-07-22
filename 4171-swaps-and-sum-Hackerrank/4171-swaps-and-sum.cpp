#include <bits/stdc++.h>

using namespace std;

string ltrim(const string &);
string rtrim(const string &);
vector<string> split(const string &);

mt19937 rng(1337);

/*
 * Implicit Treap (Cartesian Tree) Implementation
 */
struct Node {
    long long val;
    long long sum;
    uint32_t priority;
    int size;
    Node* left;
    Node* right;

    Node(long long v) : val(v), sum(v), priority(rng()), size(1), left(nullptr), right(nullptr) {}
};

int getSize(Node* t) {
    return t ? t->size : 0;
}

long long getSum(Node* t) {
    return t ? t->sum : 0;
}

void update(Node* t) {
    if (t) {
        t->size = 1 + getSize(t->left) + getSize(t->right);
        t->sum = t->val + getSum(t->left) + getSum(t->right);
    }
}

void splitNode(Node* t, int k, Node*& l, Node*& r) {
    if (!t) {
        l = r = nullptr;
        return;
    }
    int leftSize = getSize(t->left);
    if (leftSize >= k) {
        splitNode(t->left, k, l, t->left);
        r = t;
    } else {
        splitNode(t->right, k - leftSize - 1, t->right, r);
        l = t;
    }
    update(t);
}

void mergeNode(Node*& t, Node* l, Node* r) {
    if (!l || !r) {
        t = l ? l : r;
        return;
    }
    if (l->priority > r->priority) {
        mergeNode(l->right, l->right, r);
        t = l;
    } else {
        mergeNode(r->left, l, r->left);
        t = r;
    }
    update(t);
}

long long queryRange(Node*& root, int ql, int qr) {
    if (ql > qr) return 0;
    Node *l, *mid, *r;
    splitNode(root, ql - 1, l, mid);
    splitNode(mid, qr - ql + 1, mid, r);
    long long ans = getSum(mid);
    mergeNode(mid, mid, r);
    mergeNode(root, l, mid);
    return ans;
}

void swapRange(Node*& rootA, int lA, int rA, Node*& rootB, int lB, int rB) {
    Node *lA_node, *midA, *rA_node;
    Node *lB_node, *midB, *rB_node;

    splitNode(rootA, lA - 1, lA_node, midA);
    splitNode(midA, rA - lA + 1, midA, rA_node);

    splitNode(rootB, lB - 1, lB_node, midB);
    splitNode(midB, rB - lB + 1, midB, rB_node);

    // Swap subtrees midA and midB
    mergeNode(lA_node, lA_node, midB);
    mergeNode(rootA, lA_node, rA_node);

    mergeNode(lB_node, lB_node, midA);
    mergeNode(rootB, lB_node, rB_node);
}

/*
 * Complete the 'solve' function below.
 */
vector<long long> solve(vector<int> a, vector<vector<int>> queries) {
    int n = a.size();

    Node* rootO = nullptr;
    Node* rootE = nullptr;

    for (int i = 1; i <= n; i++) {
        Node* node = new Node(a[i - 1]);
        if (i % 2 != 0) {
            mergeNode(rootO, rootO, node);
        } else {
            mergeNode(rootE, rootE, node);
        }
    }

    vector<long long> result;

    for (const auto& q : queries) {
        int type = q[0];
        int l = q[1];
        int r = q[2];

        if (type == 1) {
            int len = r - l + 1;
            int P = len / 2;

            if (l % 2 != 0) {
                int k = (l - 1) / 2;
                int startO = k + 1;
                int endO = k + P;
                int startE = k + 1;
                int endE = k + P;
                swapRange(rootO, startO, endO, rootE, startE, endE);
            } else {
                int k = l / 2;
                int startE = k;
                int endE = k + P - 1;
                int startO = k + 1;
                int endO = k + P;
                swapRange(rootE, startE, endE, rootO, startO, endO);
            }
        } else {
            long long totalSum = 0;

            // Odd indices in original 1-based indexing for range [l, r]
            int l_odd = (l % 2 != 0) ? l : l + 1;
            int r_odd = (r % 2 != 0) ? r : r - 1;
            if (l_odd <= r_odd) {
                int startO = (l_odd + 1) / 2;
                int endO = (r_odd + 1) / 2;
                totalSum += queryRange(rootO, startO, endO);
            }

            // Even indices in original 1-based indexing for range [l, r]
            int l_even = (l % 2 == 0) ? l : l + 1;
            int r_even = (r % 2 == 0) ? r : r - 1;
            if (l_even <= r_even) {
                int startE = l_even / 2;
                int endE = r_even / 2;
                totalSum += queryRange(rootE, startE, endE);
            }

            result.push_back(totalSum);
        }
    }

    return result;
}

int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    string first_multiple_input_temp;
    getline(cin, first_multiple_input_temp);

    vector<string> first_multiple_input = split(rtrim(first_multiple_input_temp));

    int n = stoi(first_multiple_input[0]);

    int q = stoi(first_multiple_input[1]);

    string a_temp_temp;
    getline(cin, a_temp_temp);

    vector<string> a_temp = split(rtrim(a_temp_temp));

    vector<int> a(n);

    for (int i = 0; i < n; i++) {
        int a_item = stoi(a_temp[i]);

        a[i] = a_item;
    }

    vector<vector<int>> queries(q);

    for (int i = 0; i < q; i++) {
        queries[i].resize(3);

        string queries_row_temp_temp;
        getline(cin, queries_row_temp_temp);

        vector<string> queries_row_temp = split(rtrim(queries_row_temp_temp));

        for (int j = 0; j < 3; j++) {
            int queries_row_item = stoi(queries_row_temp[j]);

            queries[i][j] = queries_row_item;
        }
    }

    vector<long long> result = solve(a, queries);

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