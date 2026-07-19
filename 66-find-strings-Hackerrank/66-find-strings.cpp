#include <bits/stdc++.h>

using namespace std;

string ltrim(const string &);
string rtrim(const string &);

/*
 * Complete the 'findStrings' function below.
 *
 * The function is expected to return a STRING_ARRAY.
 * The function accepts following parameters:
 *  1. STRING_ARRAY w
 *  2. INTEGER_ARRAY queries
 */

struct State {
    int len, link;
    int next[26];
};

const int MAXLEN = 100005; // Maximum possible total characters across all strings
State st[MAXLEN * 2];
int sz, last;
long long cnt[MAXLEN * 2]; // Stores the number of distinct substrings from this state

void sa_init() {
    st[0].len = 0;
    st[0].link = -1;
    memset(st[0].next, -1, sizeof(st[0].next));
    sz = 1;
    last = 0;
}

void sa_extend(char c) {
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

// Compute the number of distinct paths starting from each state
long long dfs(int u) {
    if (cnt[u] != -1) return cnt[u];
    long long total = 1; // Count the empty path ending right at state `u`
    for (int c = 0; c < 26; ++c) {
        if (st[u].next[c] != -1) {
            total += dfs(st[u].next[c]);
        }
    }
    return cnt[u] = total;
}

vector<string> findStrings(vector<string> w, vector<int> queries) {
    sa_init();
    
    // Build the generalized suffix automaton by resetting `last` to 0 for each string
    for (const string &s : w) {
        last = 0;
        for (char c : s) {
            sa_extend(c);
        }
    }

    memset(cnt, -1, sizeof(cnt));
    dfs(0);

    vector<string> ans;
    // Total number of unique substrings is the total paths out of root minus 1 (empty string)
    long long total_distinct = cnt[0] - 1; 

    for (int k : queries) {
        if (k > total_distinct || k <= 0) {
            ans.push_back("INVALID");
            continue;
        }

        int curr_node = 0;
        string current_str = "";
        long long rank = k;

        while (rank > 0) {
            for (int c = 0; c < 26; ++c) {
                int next_node = st[curr_node].next[c];
                if (next_node != -1) {
                    long long paths_available = cnt[next_node];
                    if (rank <= paths_available) {
                        current_str += (char)('a' + c);
                        rank--; // Found a character transition path matching this rank segment
                        curr_node = next_node;
                        break;
                    } else {
                        rank -= paths_available;
                    }
                }
            }
        }
        ans.push_back(current_str);
    }

    return ans;
}

int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    string w_count_temp;
    getline(cin, w_count_temp);

    int w_count = stoi(ltrim(rtrim(w_count_temp)));

    vector<string> w(w_count);

    for (int i = 0; i < w_count; i++) {
        string w_item;
        getline(cin, w_item);

        w[i] = w_item;
    }

    string queries_count_temp;
    getline(cin, queries_count_temp);

    int queries_count = stoi(ltrim(rtrim(queries_count_temp)));

    vector<int> queries(queries_count);

    for (int i = 0; i < queries_count; i++) {
        string queries_item_temp;
        getline(cin, queries_item_temp);

        int queries_item = stoi(ltrim(rtrim(queries_item_temp)));

        queries[i] = queries_item;
    }

    vector<string> result = findStrings(w, queries);

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


// Synced seamlessly with LeetHub Pro
// Pro features: https://bit.ly/leethubpro | Free version: https://bit.ly/leethubv4
// Get it here: https://chromewebstore.google.com/detail/bcilpkkbokcopmabingnndookdogmbna