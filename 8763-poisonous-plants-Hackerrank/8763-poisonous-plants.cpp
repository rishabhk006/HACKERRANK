#include <bits/stdc++.h>

using namespace std;

string ltrim(const string &);
string rtrim(const string &);
vector<string> split(const string &);

int poisonousPlants(vector<int> p) {
    int n = p.size();
    vector<int> days(n, 0);
    stack<int> st;
    int max_days = 0;

    for (int i = 0; i < n; i++) {
        int cur_days = 0;

        while (!st.empty() && p[st.top()] >= p[i]) {
            cur_days = max(cur_days, days[st.top()]);
            st.pop();
        }

        if (!st.empty()) {
            days[i] = cur_days + 1;
        } else {
            days[i] = 0;
        }

        max_days = max(max_days, days[i]);
        st.push(i);
    }

    return max_days;
}

int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    string n_temp;
    getline(cin, n_temp);

    int n = stoi(ltrim(rtrim(n_temp)));

    string p_temp_temp;
    getline(cin, p_temp_temp);

    vector<string> p_temp = split(rtrim(p_temp_temp));

    vector<int> p(n);

    for (int i = 0; i < n; i++) {
        int p_item = stoi(p_temp[i]);

        p[i] = p_item;
    }

    int result = poisonousPlants(p);

    fout << result << "\n";

    fout.close();

    return 0;
}

string ltrim(const string &str) {
    string s(str);

    s.erase(
        s.begin(),
        find_if(s.begin(), s.end(), [](unsigned char ch) { return !isspace(ch); })
    );

    return s;
}

string rtrim(const string &str) {
    string s(str);

    s.erase(
        find_if(s.rbegin(), s.rend(), [](unsigned char ch) { return !isspace(ch); }).base(),
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