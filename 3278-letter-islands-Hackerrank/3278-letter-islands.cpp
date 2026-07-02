#include <bits/stdc++.h>

using namespace std;

string ltrim(const string &);
string rtrim(const string &);

const int MAXLEN = 100010;

struct State {
    int len, link;
    int next[26];
};

State st[MAXLEN * 2];
int sz_sam, last_sam;
vector<int> adj[MAXLEN * 2];
vector<int> leaf_pos[MAXLEN * 2];
int subtree_sz[MAXLEN * 2];
int hc[MAXLEN * 2];
int in_tour[MAXLEN * 2], out_tour[MAXLEN * 2];
vector<int> tour;
set<int> active_pos;
int bit[MAXLEN + 5];
long long total_ans;
int N_val;
int K_val;

// Fenwick Tree update
void add_gap(int gap, int sign) {
    if (gap <= 0) return;
    for (int i = gap; i <= N_val; i += i & -i) {
        bit[i] += sign;
    }
}

// Manage dynamic tracking of suffix occurrences and gaps
void add_pos(int p) {
    auto it = active_pos.insert(p).first;
    auto prev_it = it, next_it = it;
    bool has_prev = (it != active_pos.begin());
    bool has_next = false;
    
    if (has_prev) prev_it--;
    next_it++;
    if (next_it != active_pos.end()) has_next = true;

    if (has_prev && has_next) add_gap(*next_it - *prev_it, -1);
    if (has_prev) add_gap(p - *prev_it, 1);
    if (has_next) add_gap(*next_it - p, 1);
}

void remove_pos(int p) {
    auto it = active_pos.find(p);
    auto prev_it = it, next_it = it;
    bool has_prev = (it != active_pos.begin());
    bool has_next = false;
    
    if (has_prev) prev_it--;
    next_it++;
    if (next_it != active_pos.end()) has_next = true;

    if (has_prev && has_next) add_gap(*next_it - *prev_it, 1);
    if (has_prev) add_gap(p - *prev_it, -1);
    if (has_next) add_gap(*next_it - p, -1);
    
    active_pos.erase(it);
}

// Binary lifting on fenwick tree to find bounding configurations
int find_last(int val) {
    if (val < 0) return 0;
    int idx = 0;
    for (int i = 17; i >= 0; i--) { 
        int next_idx = idx + (1 << i);
        if (next_idx <= N_val && bit[next_idx] <= val) {
            idx = next_idx;
            val -= bit[next_idx];
        }
    }
    return idx;
}

// Initialize and build Suffix Automaton
void sam_init() {
    st[0].len = 0;
    st[0].link = -1;
    memset(st[0].next, -1, sizeof(st[0].next));
    sz_sam = 1;
    last_sam = 0;
}

void sam_extend(char c, int pos) {
    int cur = sz_sam++;
    st[cur].len = st[last_sam].len + 1;
    memset(st[cur].next, -1, sizeof(st[cur].next));
    leaf_pos[cur].push_back(pos);
    
    int p = last_sam;
    int c_idx = c - 'a';
    while (p != -1 && st[p].next[c_idx] == -1) {
        st[p].next[c_idx] = cur;
        p = st[p].link;
    }
    if (p == -1) {
        st[cur].link = 0;
    } else {
        int q = st[p].next[c_idx];
        if (st[p].len + 1 == st[q].len) {
            st[cur].link = q;
        } else {
            int clone = sz_sam++;
            st[clone].len = st[p].len + 1;
            memcpy(st[clone].next, st[q].next, sizeof(st[q].next));
            st[clone].link = st[q].link;
            while (p != -1 && st[p].next[c_idx] == q) {
                st[p].next[c_idx] = clone;
                p = st[p].link;
            }
            st[q].link = st[cur].link = clone;
        }
    }
    last_sam = cur;
}

// Sizing the link tree for Sack structure
void dfs_sz(int u) {
    subtree_sz[u] = leaf_pos[u].size();
    hc[u] = -1;
    in_tour[u] = tour.size();
    for (int p : leaf_pos[u]) tour.push_back(p);
    
    int max_sub = -1;
    for (int v : adj[u]) {
        dfs_sz(v);
        subtree_sz[u] += subtree_sz[v];
        if (subtree_sz[v] > max_sub) {
            max_sub = subtree_sz[v];
            hc[u] = v;
        }
    }
    out_tour[u] = tour.size();
}

// Small-to-Large merging on Suffix link-tree
void solve_dsu(int u, bool keep) {
    for (int v : adj[u]) {
        if (v != hc[u]) solve_dsu(v, 0);
    }
    if (hc[u] != -1) solve_dsu(hc[u], 1);
    
    for (int v : adj[u]) {
        if (v != hc[u]) {
            for (int i = in_tour[v]; i < out_tour[v]; i++) add_pos(tour[i]);
        }
    }
    
    for (int p : leaf_pos[u]) add_pos(p);

    if (u != 0) {
        int m = subtree_sz[u];
        if (m >= K_val) {
            int req = m - K_val;
            int idx1 = req == 0 ? 0 : find_last(req - 1);
            int idx2 = find_last(req);
            
            int min_L = max(idx1 + 1, st[st[u].link].len + 1);
            int max_L = min(idx2, st[u].len);
            
            if (min_L <= max_L) {
                total_ans += (max_L - min_L + 1);
            }
        }
    }

    if (!keep) {
        for (int i = in_tour[u]; i < out_tour[u]; i++) remove_pos(tour[i]);
    }
}

/*
 * Note: Return type changed to long long to prevent integer overflow.
 */
long long letterIslands(string s, int k) {
    // Sanitize string to bypass potential HackerRank trailing \r lines
    string clean_s = "";
    for(char c : s) {
        if(c >= 'a' && c <= 'z') clean_s += c;
    }
    s = clean_s;

    N_val = s.length();
    K_val = k;
    
    for (int i = 0; i < MAXLEN * 2; i++) {
        adj[i].clear();
        leaf_pos[i].clear();
    }
    tour.clear();
    active_pos.clear();
    memset(bit, 0, sizeof(bit));
    total_ans = 0;
    
    sam_init();
    for (int i = 0; i < N_val; i++) sam_extend(s[i], i);
    
    for (int i = 1; i < sz_sam; i++) adj[st[i].link].push_back(i);
    
    dfs_sz(0);
    solve_dsu(0, 1);
    
    return total_ans;
}

int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    string s;
    getline(cin, s);

    string k_temp;
    getline(cin, k_temp);

    int k = stoi(ltrim(rtrim(k_temp)));

    // Ensure the receiver is also a long long to capture the full integer value
    long long result = letterIslands(s, k);

    fout << result << "\n";

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


// Synced seamlessly with LeetHub Pro
// Pro features: https://bit.ly/leethubpro | Free version: https://bit.ly/leethubv4
// Get it here: https://chromewebstore.google.com/detail/bcilpkkbokcopmabingnndookdogmbna