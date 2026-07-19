#include <bits/stdc++.h>

using namespace std;

/*
 * Complete the 'palindromicBorder' function below.
 *
 * The function is expected to return an INTEGER.
 * The function accepts STRING s as parameter.
 */

const int MOD = 1000000007;

struct PalindromicTree {
    struct Node {
        int next[8]; // Limited to first 8 lowercase letters (a-h)
        int len;
        int fail;
        long long cnt;
    };

    vector<Node> tree;
    string s;
    int sz;
    int currNode;

    PalindromicTree(string str) {
        s = str;
        int n = s.length();
        // The tree has at most n + 2 nodes
        tree.resize(n + 3);
        
        // Root 1: imaginary node of length -1 (used for odd length palindromes)
        tree[1].len = -1;
        tree[1].fail = 1;
        memset(tree[1].next, 0, sizeof(tree[1].next));
        tree[1].cnt = 0;

        // Root 2: root node of length 0 (used for even length palindromes)
        tree[2].len = 0;
        tree[2].fail = 1;
        memset(tree[2].next, 0, sizeof(tree[2].next));
        tree[2].cnt = 0;

        sz = 2;
        currNode = 1;
    }

    int getFail(int node, int idx) {
        while (true) {
            int len = tree[node].len;
            if (idx - 1 - len >= 0 && s[idx] == s[idx - 1 - len]) {
                return node;
            }
            node = tree[node].fail;
        }
    }

    void insert(int idx) {
        int c = s[idx] - 'a';
        int p = getFail(currNode, idx);

        if (!tree[p].next[c]) {
            sz++;
            tree[sz].len = tree[p].len + 2;
            memset(tree[sz].next, 0, sizeof(tree[sz].next));
            tree[sz].cnt = 0;

            if (tree[sz].len == 1) {
                tree[sz].fail = 2;
            } else {
                tree[sz].fail = tree[getFail(tree[p].fail, idx)].next[c];
            }
            tree[p].next[c] = sz;
        }

        currNode = tree[p].next[c];
        tree[currNode].cnt++;
    }

    long long solve() {
        for (int i = 0; i < (int)s.length(); i++) {
            insert(i);
        }

        // Propagate counts from child nodes up to their suffix fail links
        // Processing in reverse order ensures longer palindromes propagate to their sub-palindromes first
        for (int i = sz; i > 2; i--) {
            int parent = tree[i].fail;
            if (parent > 0) {
                tree[parent].cnt += tree[i].cnt;
            }
        }

        long long totalAns = 0;
        for (int i = 3; i <= sz; i++) {
            long long c = tree[i].cnt;
            if (c >= 2) {
                long long pairs = (c * (c - 1) / 2) % MOD;
                totalAns = (totalAns + pairs) % MOD;
            }
        }

        return totalAns;
    }
};

int palindromicBorder(string s) {
    PalindromicTree pt(s);
    return pt.solve();
}

int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    string s;
    getline(cin, s);

    int result = palindromicBorder(s);

    fout << result << "\n";

    fout.close();

    return 0;
}


// Synced seamlessly with LeetHub Pro
// Pro features: https://bit.ly/leethubpro | Free version: https://bit.ly/leethubv4
// Get it here: https://chromewebstore.google.com/detail/bcilpkkbokcopmabingnndookdogmbna