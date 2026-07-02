#include <bits/stdc++.h>

using namespace std;

string ltrim(const string &);
string rtrim(const string &);
vector<string> split(const string &);

vector<int> get_z(const string &s) {
    int n = s.length();
    vector<int> z(n, 0);
    int l = 0, r = 0;
    for (int i = 1; i < n; i++) {
        if (i <= r) {
            z[i] = min(r - i + 1, z[i - l]);
        }
        while (i + z[i] < n && s[z[i]] == s[i + z[i]]) {
            z[i]++;
        }
        if (i + z[i] - 1 > r) {
            l = i;
            r = i + z[i] - 1;
        }
    }
    return z;
}

void virusIndices(string p, string v) {
    int p_len = p.length();
    int v_len = v.length();

    if (v_len > p_len) {
        cout << "No Match!\n";
        return;
    }

    string s1 = v + "#" + p;
    
    string v_rev = v;
    string p_rev = p;
    reverse(v_rev.begin(), v_rev.end());
    reverse(p_rev.begin(), p_rev.end());
    string s2 = v_rev + "#" + p_rev;

    vector<int> z1 = get_z(s1);
    vector<int> z2 = get_z(s2);

    bool found = false;

    for (int i = 0; i <= p_len - v_len; i++) {
        int lcp = z1[v_len + 1 + i];
        int lcs = z2[p_len - i + 1];

        if (lcp + lcs >= v_len - 1) {
            if (found) cout << " ";
            cout << i;
            found = true;
        }
    }

    if (!found) {
        cout << "No Match!";
    }
    cout << "\n";
}
int main()
{
    string t_temp;
    getline(cin, t_temp);

    int t = stoi(ltrim(rtrim(t_temp)));

    for (int t_itr = 0; t_itr < t; t_itr++) {
        string first_multiple_input_temp;
        getline(cin, first_multiple_input_temp);

        vector<string> first_multiple_input = split(rtrim(first_multiple_input_temp));

        string p = first_multiple_input[0];

        string v = first_multiple_input[1];

        virusIndices(p, v);
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