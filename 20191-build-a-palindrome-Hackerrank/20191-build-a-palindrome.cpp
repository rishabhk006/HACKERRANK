#include <bits/stdc++.h>

using namespace std;

string ltrim(const string &);
string rtrim(const string &);

struct SAM {
    struct State {
        int len, link;
        int next[26];
    };
    vector<State> st;
    int sz, last;

    void init(int maxlen) {
        st.assign(maxlen * 2, State());
        st[0].len = 0;
        st[0].link = -1;
        memset(st[0].next, -1, sizeof(st[0].next));
        sz = 1;
        last = 0;
    }

    void extend(char c) {
        int cur = sz++;
        st[cur].len = st[last].len + 1;
        memset(st[cur].next, -1, sizeof(st[cur].next));
        int p = last;
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
                int clone = sz++;
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
        last = cur;
    }
};

vector<int> get_palindromes(const string& s) {
    int n = s.length();
    string t = "^";
    for (char c : s) {
        t += "#";
        t += c;
    }
    t += "#$";
    int m = t.length();
    vector<int> p(m, 0);
    int c = 0, r = 0;
    for (int i = 1; i < m - 1; i++) {
        int i_mirror = 2 * c - i;
        if (r > i) p[i] = min(r - i, p[i_mirror]);
        while (t[i + 1 + p[i]] == t[i - 1 - p[i]]) p[i]++;
        if (i + p[i] > r) {
            c = i;
            r = i + p[i];
        }
    }
    vector<int> max_len(n, 0);
    for (int i = 1; i < m - 1; i++) {
        if (p[i] == 0) continue;
        int len = p[i];
        int start_idx = (i - len) / 2;
        max_len[start_idx] = max(max_len[start_idx], len);
    }
    for (int i = 1; i < n; i++) {
        max_len[i] = max(max_len[i], max_len[i - 1] - 2);
    }
    return max_len;
}

vector<int> buildSA(const string& s) {
    int n = s.length();
    vector<int> sa(n), rank(n);
    for (int i = 0; i < n; i++) {
        sa[i] = i;
        rank[i] = s[i];
    }
    for (int k = 1; k < n; k <<= 1) {
        auto cmp = [&](int i, int j) {
            if (rank[i] != rank[j]) return rank[i] < rank[j];
            int ri = (i + k < n) ? rank[i + k] : -1;
            int rj = (j + k < n) ? rank[j + k] : -1;
            return ri < rj;
        };
        sort(sa.begin(), sa.end(), cmp);
        vector<int> tmp(n);
        tmp[sa[0]] = 0;
        for (int i = 1; i < n; i++) {
            tmp[sa[i]] = tmp[sa[i - 1]] + (cmp(sa[i - 1], sa[i]) ? 1 : 0);
        }
        rank = tmp;
    }
    return sa;
}

vector<int> buildLCP(const string& s, const vector<int>& sa) {
    int n = s.length();
    vector<int> lcp(n, 0), rank(n, 0);
    for (int i = 0; i < n; i++) rank[sa[i]] = i;
    int h = 0;
    for (int i = 0; i < n; i++) {
        if (rank[i] > 0) {
            int j = sa[rank[i] - 1];
            while (i + h < n && j + h < n && s[i + h] == s[j + h]) h++;
            lcp[rank[i]] = h;
            if (h > 0) h--;
        }
    }
    return lcp;
}

struct RMQ {
    vector<vector<int>> st;
    vector<int> lg;
    void init(const vector<int>& r) {
        int n = r.size();
        lg.assign(n + 1, 0);
        for (int i = 2; i <= n; i++) lg[i] = lg[i / 2] + 1;
        st.assign(lg[n] + 1, vector<int>(n));
        for (int i = 0; i < n; i++) st[0][i] = r[i];
        for (int j = 1; (1 << j) <= n; j++) {
            for (int i = 0; i + (1 << j) <= n; i++) {
                st[j][i] = min(st[j - 1][i], st[j - 1][i + (1 << (j - 1))]);
            }
        }
    }
    int query(int l, int r) {
        if (l == r) return 1e9;
        if (l > r) swap(l, r);
        l++;
        int k = lg[r - l + 1];
        return min(st[k][l], st[k][r - (1 << k) + 1]);
    }
};

struct Piece {
    int start, len;
};

struct Candidate {
    vector<Piece> pieces;
};

string buildPalindrome(string a, string b) {
    int len_a = a.length(), len_b = b.length();
    string a_rev = a, b_rev = b;
    reverse(a_rev.begin(), a_rev.end());
    reverse(b_rev.begin(), b_rev.end());

    vector<int> palA = get_palindromes(a);
    vector<int> palB_rev = get_palindromes(b_rev);

    string T = a + "#" + b + "$" + b_rev + "%" + a_rev + "&";
    int offset_a = 0;
    int offset_b = len_a + 1;
    int offset_brev = len_a + len_b + 2;
    int offset_arev = len_a + 2 * len_b + 3;

    vector<int> sa = buildSA(T);
    vector<int> lcp = buildLCP(T, sa);
    vector<int> rank(T.length());
    for (size_t i = 0; i < sa.size(); i++) rank[sa[i]] = i;

    RMQ rmq;
    rmq.init(lcp);

    auto get_lcp = [&](int i, int j) {
        if (i == j) return (int)T.length() - i;
        return rmq.query(rank[i], rank[j]);
    };

    SAM samB, samA;
    samB.init(len_b);
    for (char c : b_rev) samB.extend(c);
    samA.init(len_a);
    for (char c : a) samA.extend(c);

    vector<int> LmaxA(len_a, 0);
    int curr_node = 0, curr_len = 0;
    for (int i = 0; i < len_a; i++) {
        int c_idx = a[i] - 'a';
        while (curr_node != -1 && samB.st[curr_node].next[c_idx] == -1) {
            curr_node = samB.st[curr_node].link;
            if (curr_node != -1) curr_len = samB.st[curr_node].len;
        }
        if (curr_node == -1) {
            curr_node = 0;
            curr_len = 0;
        } else {
            curr_node = samB.st[curr_node].next[c_idx];
            curr_len++;
        }
        LmaxA[i] = curr_len;
    }

    vector<int> LmaxB(len_b, 0);
    curr_node = 0, curr_len = 0;
    for (int j = 0; j < len_b; j++) {
        int c_idx = b_rev[j] - 'a';
        while (curr_node != -1 && samA.st[curr_node].next[c_idx] == -1) {
            curr_node = samA.st[curr_node].link;
            if (curr_node != -1) curr_len = samA.st[curr_node].len;
        }
        if (curr_node == -1) {
            curr_node = 0;
            curr_len = 0;
        } else {
            curr_node = samA.st[curr_node].next[c_idx];
            curr_len++;
        }
        LmaxB[j] = curr_len;
    }

    int max_score = 0;
    for (int i = 0; i < len_a; i++) {
        if (LmaxA[i] > 0) {
            int p_len = (i + 1 < len_a) ? palA[i + 1] : 0;
            max_score = max(max_score, 2 * LmaxA[i] + p_len);
        }
    }
    for (int j = 0; j < len_b; j++) {
        if (LmaxB[j] > 0) {
            int p_len = (j + 1 < len_b) ? palB_rev[j + 1] : 0;
            max_score = max(max_score, 2 * LmaxB[j] + p_len);
        }
    }

    if (max_score == 0) return "-1";

    vector<Candidate> candidates;
    for (int i = 0; i < len_a; i++) {
        if (LmaxA[i] > 0) {
            int p_len = (i + 1 < len_a) ? palA[i + 1] : 0;
            if (2 * LmaxA[i] + p_len == max_score) {
                Candidate cand;
                int L = LmaxA[i];
                cand.pieces.push_back({offset_a + i - L + 1, L});
                if (p_len > 0) cand.pieces.push_back({offset_a + i + 1, p_len});
                cand.pieces.push_back({offset_arev + len_a - 1 - i, L});
                candidates.push_back(cand);
            }
        }
    }
    for (int j = 0; j < len_b; j++) {
        if (LmaxB[j] > 0) {
            int p_len = (j + 1 < len_b) ? palB_rev[j + 1] : 0;
            if (2 * LmaxB[j] + p_len == max_score) {
                Candidate cand;
                int L = LmaxB[j];
                cand.pieces.push_back({offset_brev + j - L + 1, L});
                if (p_len > 0) cand.pieces.push_back({offset_brev + j + 1, p_len});
                cand.pieces.push_back({offset_b + len_b - 1 - j, L});
                candidates.push_back(cand);
            }
        }
    }

    auto cmp_candidates = [&](const Candidate& A, const Candidate& B) {
        size_t idxA = 0, idxB = 0;
        int posA = 0, posB = 0;
        while (idxA < A.pieces.size() && idxB < B.pieces.size()) {
            int startA = A.pieces[idxA].start + posA;
            int remA = A.pieces[idxA].len - posA;
            int startB = B.pieces[idxB].start + posB;
            int remB = B.pieces[idxB].len - posB;

            int lcp_val = get_lcp(startA, startB);
            int match_len = min({lcp_val, remA, remB});

            if (match_len < remA && match_len < remB) {
                return T[startA + match_len] < T[startB + match_len];
            }
            posA += match_len;
            if (posA == A.pieces[idxA].len) { idxA++; posA = 0; }
            posB += match_len;
            if (posB == B.pieces[idxB].len) { idxB++; posB = 0; }
        }
        return false;
    };

    Candidate best = candidates[0];
    for (size_t i = 1; i < candidates.size(); i++) {
        if (cmp_candidates(candidates[i], best)) {
            best = candidates[i];
        }
    }

    string res = "";
    for (auto& p : best.pieces) {
        res += T.substr(p.start, p.len);
    }
    return res;
}

int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    string t_temp;
    getline(cin, t_temp);

    int t = stoi(ltrim(rtrim(t_temp)));

    for (int t_itr = 0; t_itr < t; t_itr++) {
        string a;
        getline(cin, a);

        string b;
        getline(cin, b);

        string result = buildPalindrome(a, b);

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