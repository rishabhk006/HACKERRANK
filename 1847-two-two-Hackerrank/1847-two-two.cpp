#include <bits/stdc++.h>

using namespace std;

string ltrim(const string &);
string rtrim(const string &);

// Aho-Corasick Node Structure
struct Node {
    int next[10];
    int fail;
    int cnt;
    Node() {
        fill(next, next + 10, -1);
        fail = -1;
        cnt = 0;
    }
};

vector<Node> trie;
bool initialized = false;

// Insert a pattern into the Trie
void insertPattern(const string& s) {
    int u = 0;
    for (char c : s) {
        int d = c - '0';
        if (trie[u].next[d] == -1) {
            trie[u].next[d] = trie.size();
            trie.push_back(Node());
        }
        u = trie[u].next[d];
    }
    trie[u].cnt++;
}

// Build failure links for Aho-Corasick
void build_ac() {
    queue<int> q;
    for (int d = 0; d < 10; ++d) {
        if (trie[0].next[d] != -1) {
            trie[trie[0].next[d]].fail = 0;
            q.push(trie[0].next[d]);
        } else {
            trie[0].next[d] = 0;
        }
    }
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        
        trie[u].cnt += trie[trie[u].fail].cnt;
        
        for (int d = 0; d < 10; ++d) {
            if (trie[u].next[d] != -1) {
                trie[trie[u].next[d]].fail = trie[trie[u].fail].next[d];
                q.push(trie[u].next[d]);
            } else {
                trie[u].next[d] = trie[trie[u].fail].next[d];
            }
        }
    }
}

// Initialize powers of 2 and construct the automaton once
void init() {
    if (initialized) return;
    trie.push_back(Node());
    
    vector<string> powers;
    string current = "1";
    powers.push_back(current);
    
    for (int i = 1; i <= 800; ++i) {
        string next = "";
        int carry = 0;
        for (int j = (int)current.length() - 1; j >= 0; --j) {
            int prod = (current[j] - '0') * 2 + carry;
            next += (prod % 10) + '0';
            carry = prod / 10;
        }
        if (carry) next += carry + '0';
        reverse(next.begin(), next.end());
        powers.push_back(next);
        current = next;
    }
    
    for (const string& s : powers) {
        insertPattern(s);
    }
    build_ac();
    initialized = true;
}

/*
 * Complete the 'twoTwo' function below.
 */
int twoTwo(string a) {
    init();
    int u = 0;
    int total = 0;
    for (char c : a) {
        int d = c - '0';
        u = trie[u].next[d];
        total += trie[u].cnt;
    }
    return total;
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    ofstream fout(getenv("OUTPUT_PATH"));

    string t_temp;
    if (!getline(cin, t_temp)) return 0;

    int t = stoi(ltrim(rtrim(t_temp)));

    for (int t_itr = 0; t_itr < t; t_itr++) {
        string a;
        getline(cin, a);

        int result = twoTwo(a);

        fout << result << "\n";
    }

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