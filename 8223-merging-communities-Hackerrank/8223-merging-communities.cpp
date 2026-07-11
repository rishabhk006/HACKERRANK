#include <bits/stdc++.h>
using namespace std;

vector<int> parent, sz;

int find(int x) {
    if (parent[x] == x)
        return x;
    return parent[x] = find(parent[x]);
}

void unite(int a, int b) {
    a = find(a);
    b = find(b);

    if (a == b)
        return;

    if (sz[a] < sz[b])
        swap(a, b);

    parent[b] = a;
    sz[a] += sz[b];
}

int main() {
    int n, q;
    cin >> n >> q;

    parent.resize(n + 1);
    sz.assign(n + 1, 1);

    for (int i = 1; i <= n; i++)
        parent[i] = i;

    while (q--) {
        char ch;
        cin >> ch;

        if (ch == 'M') {
            int a, b;
            cin >> a >> b;
            unite(a, b);
        } else {
            int a;
            cin >> a;
            cout << sz[find(a)] << "\n";
        }
    }

    return 0;
}


// Synced seamlessly with LeetHub Pro
// Pro features: https://bit.ly/leethubpro | Free version: https://bit.ly/leethubv4
// Get it here: https://chromewebstore.google.com/detail/bcilpkkbokcopmabingnndookdogmbna