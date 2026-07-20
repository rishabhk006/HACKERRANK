#include <bits/stdc++.h>
using namespace std;

const int MAXN = 100005;

int ch[MAXN][2];
int p[MAXN];
int sz[MAXN];
bool rev[MAXN];
int root;

void push_down(int x) {
    if (x && rev[x]) {
        swap(ch[x][0], ch[x][1]);
        if (ch[x][0]) rev[ch[x][0]] ^= 1;
        if (ch[x][1]) rev[ch[x][1]] ^= 1;
        rev[x] = false;
    }
}

void push_up(int x) {
    if (x) {
        sz[x] = 1 + sz[ch[x][0]] + sz[ch[x][1]];
    }
}

void rotate(int x) {
    int y = p[x], z = p[y];
    int k = (ch[y][1] == x);
    if (z) ch[z][ch[z][1] == y] = x;
    p[x] = z;
    ch[y][k] = ch[x][k ^ 1];
    if (ch[x][k ^ 1]) p[ch[x][k ^ 1]] = y;
    ch[x][k ^ 1] = y;
    p[y] = x;
    push_up(y);
    push_up(x);
}

// Push tags from the root down to node x
void push_from_root(int x) {
    if (p[x]) push_from_root(p[x]);
    push_down(x);
}

void splay(int x, int goal = 0) {
    push_from_root(x);
    while (p[x] != goal) {
        int y = p[x], z = p[y];
        if (z != goal) {
            if ((ch[y][1] == x) ^ (ch[z][1] == y)) rotate(x);
            else rotate(y);
        }
        rotate(x);
    }
    if (goal == 0) root = x;
}

// Find the node at the 1-based index k
int find_kth(int k) {
    int u = root;
    while (u) {
        push_down(u);
        int left_sz = sz[ch[u][0]];
        if (k <= left_sz) {
            u = ch[u][0];
        } else if (k == left_sz + 1) {
            return u;
        } else {
            k -= (left_sz + 1);
            u = ch[u][1];
        }
    }
    return 0;
}

// Build the initial tree linearly
int build_tree(int l, int r, int parent) {
    if (l > r) return 0;
    int mid = l + (r - l) / 2;
    p[mid] = parent;
    ch[mid][0] = build_tree(l, mid - 1, mid);
    ch[mid][1] = build_tree(mid + 1, r, mid);
    push_up(mid);
    return mid;
}

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, q;
    if (!(cin >> n >> q)) return 0;

    // Node 1: dummy front
    // Nodes 2 to n + 1: elements 1 to n
    // Node n + 2: dummy back
    root = build_tree(1, n + 2, 0);

    while (q--) {
        int type;
        cin >> type;
        if (type == 1) {
            int a, b;
            cin >> a >> b;
            
            // To isolate [a, b], we need predecessor of a (rank a)
            // and successor of b (rank b + 2)
            int node_prev = find_kth(a);
            int node_next = find_kth(b + 2);
            
            splay(node_prev, 0);
            splay(node_next, node_prev);
            
            int target = ch[node_next][0];
            rev[target] ^= 1;
        } 
        else if (type == 2) {
            int a;
            cin >> a;
            // Element value 'a' corresponds directly to Node ID 'a + 1'
            splay(a + 1, 0);
            // Size of left subtree naturally contains the dummy front node, 
            // so sz[ch[root][0]] gives exactly its 1-based position.
            int pos = sz[ch[root][0]];
            cout << "element " << a << " is at position " << pos << "\n";
        } 
        else if (type == 3) {
            int a;
            cin >> a;
            // Position 'a' in array corresponds to rank 'a + 1' in the tree
            int node = find_kth(a + 1);
            splay(node, 0);
            // Element value is Node ID - 1
            cout << "element at position " << a << " is " << node - 1 << "\n";
        }
    }

    return 0;
}


// Synced seamlessly with LeetHub Pro
// Pro features: https://bit.ly/leethubpro | Free version: https://bit.ly/leethubv4
// Get it here: https://chromewebstore.google.com/detail/bcilpkkbokcopmabingnndookdogmbna