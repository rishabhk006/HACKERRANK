#include <bits/stdc++.h>
using namespace std;

const int MAXLEN = 300005;
const long long INF = 2e18; // Safe saturation threshold above max K (1e18)

// Safe addition to prevent long long overflow
long long safe_add(long long a, long long b) {
    if (a > INF - b) return INF;
    return a + b;
}

struct State {
    int len, link;
    int next[26];
};

struct SuffixAutomaton {
    State st[MAXLEN * 2];
    int sz, last;
    int grundy[MAXLEN * 2];

    void init() {
        sz = 1;
        last = 0;
        st[0].len = 0;
        st[0].link = -1;
        memset(st[0].next, -1, sizeof(st[0].next));
    }

    void extend(char c) {
        int cur = sz++;
        st[cur].len = st[last].len + 1;
        memset(st[cur].next, -1, sizeof(st[cur].next));
        int p = last;
        while (p != -1 && st[p].next[c - 'a'] == -1) {
            st[p].next[c - 'a'] = cur;
            p = st[p].link;
        }
        if (p == -1) {
            st[cur].link = 0;
        } else {
            int q = st[p].next[c - 'a'];
            if (st[p].len + 1 == st[q].len) {
                st[cur].link = q;
            } else {
                int clone = sz++;
                st[clone].len = st[p].len + 1;
                memcpy(st[clone].next, st[q].next, sizeof(st[q].next));
                st[clone].link = st[q].link;
                while (p != -1 && st[p].next[c - 'a'] == q) {
                    st[p].next[c - 'a'] = clone;
                    p = st[p].link;
                }
                st[q].link = st[cur].link = clone;
            }
        }
        last = cur;
    }

    void compute_grundy() {
        vector<int> p(sz);
        iota(p.begin(), p.end(), 0);
        sort(p.begin(), p.end(), [&](int a, int b) {
            return st[a].len > st[b].len;
        });

        for (int u : p) {
            int mask = 0;
            for (int c = 0; c < 26; c++) {
                if (st[u].next[c] != -1) {
                    mask |= (1 << grundy[st[u].next[c]]);
                }
            }
            int g = 0;
            while ((mask & (1 << g))) g++;
            grundy[u] = g;
        }
    }
} samA, samB;

vector<long long> dpB[MAXLEN * 2];
bool visitedB[MAXLEN * 2];

void dfsB(int u) {
    if (visitedB[u]) return;
    visitedB[u] = true;
    dpB[u].assign(30, 0LL);
    dpB[u][samB.grundy[u]]++;

    for (int c = 0; c < 26; c++) {
        int v = samB.st[u].next[c];
        if (v != -1) {
            dfsB(v);
            for (int g = 0; g < 30; g++) {
                dpB[u][g] = safe_add(dpB[u][g], dpB[v][g]);
            }
        }
    }
}

long long dpWin[MAXLEN * 2];
bool visitedA[MAXLEN * 2];

void dfsA(int u) {
    if (visitedA[u]) return;
    visitedA[u] = true;

    int gA = samA.grundy[u];
    long long current_ways = 0;
    for (int gB = 0; gB < 30; gB++) {
        if ((gA ^ gB) != 0) {
            current_ways = safe_add(current_ways, dpB[0][gB]);
        }
    }
    dpWin[u] = current_ways;

    for (int c = 0; c < 26; c++) {
        int v = samA.st[u].next[c];
        if (v != -1) {
            dfsA(v);
            dpWin[u] = safe_add(dpWin[u], dpWin[v]);
        }
    }
}

vector<string> twoStrings(long long k, string a, string b) {
    samA.init();
    for (char c : a) samA.extend(c);
    samA.compute_grundy();

    samB.init();
    for (char c : b) samB.extend(c);
    samB.compute_grundy();

    memset(visitedB, 0, sizeof(visitedB));
    dfsB(0);

    memset(visitedA, 0, sizeof(visitedA));
    dfsA(0);

    int currA = 0;
    string resA = "";
    string resB = "";

    while (true) {
        int gA = samA.grundy[currA];
        
        auto get_win_B = [&](int uB) {
            long long ways = 0;
            for (int gB = 0; gB < 30; gB++) {
                if ((gA ^ gB) != 0) ways = safe_add(ways, dpB[uB][gB]);
            }
            return ways;
        };

        long long total_win_here = get_win_B(0);
        
        auto find_B = [&](int uB, long long &rem_k) -> string {
            string sB = "";
            int u = uB;
            while (true) {
                long long local_win = ((gA ^ samB.grundy[u]) != 0) ? 1 : 0;
                if (rem_k <= local_win) {
                    return sB;
                }
                rem_k -= local_win;
                
                bool moved = false;
                for (int c = 0; c < 26; c++) {
                    int v = samB.st[u].next[c];
                    if (v != -1) {
                        long long ways_v = 0;
                        for (int gB = 0; gB < 30; gB++) {
                            if ((gA ^ gB) != 0) ways_v = safe_add(ways_v, dpB[v][gB]);
                        }
                        if (rem_k <= ways_v) {
                            sB += (char)('a' + c);
                            u = v;
                            moved = true;
                            break;
                        }
                        rem_k -= ways_v;
                    }
                }
                if (!moved) return "no solution";
            }
        };

        if (k <= total_win_here) {
            resB = find_B(0, k);
            if (resB == "no solution") return {"no solution"};
            return {resA, resB};
        }
        
        k -= total_win_here;
        
        bool movedA = false;
        for (int c = 0; c < 26; c++) {
            int nextA = samA.st[currA].next[c];
            if (nextA != -1) {
                if (k <= dpWin[nextA]) {
                    resA += (char)('a' + c);
                    currA = nextA;
                    movedA = true;
                    break;
                }
                k -= dpWin[nextA];
            }
        }
        
        if (!movedA) {
            return {"no solution"};
        }
    }
}

int main() {
    // Fast I/O for large strings
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    long long n, m, k;
    if (!(cin >> n >> m >> k)) return 0;

    string a, b;
    cin >> a >> b;

    vector<string> result = twoStrings(k, a, b);

    if (result.size() == 1 && result[0] == "no solution") {
        cout << "no solution\n";
    } else {
        cout << result[0] << "\n" << result[1] << "\n";
    }

    return 0;
}


// Synced seamlessly with LeetHub Pro
// Pro features: https://bit.ly/leethubpro | Free version: https://bit.ly/leethubv4
// Get it here: https://chromewebstore.google.com/detail/bcilpkkbokcopmabingnndookdogmbna