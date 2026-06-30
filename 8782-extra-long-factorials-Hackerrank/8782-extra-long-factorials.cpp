#include <bits/stdc++.h>

using namespace std;

string ltrim(const string &);
string rtrim(const string &);

void extraLongFactorials(int n) {
    vector<int> result;
    result.push_back(1);

    for (int i = 2; i <= n; i++) {
        int carry = 0;
        for (int j = 0; j < result.size(); j++) {
            int prod = result[j] * i + carry;
            result[j] = prod % 10;
            carry = prod / 10;
        }
        while (carry > 0) {
            result.push_back(carry % 10);
            carry /= 10;
        }
    }

    for (int i = (int)result.size() - 1; i >= 0; i--) {
        cout << result[i];
    }
    cout << "\n";
}

int main()
{
    string n_temp;
    if (!getline(cin, n_temp)) return 0;

    int n = stoi(ltrim(rtrim(n_temp)));

    extraLongFactorials(n);

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


// Synced seamlessly with LeetHub Pro
// Pro features: https://bit.ly/leethubpro | Free version: https://bit.ly/leethubv4
// Get it here: https://chromewebstore.google.com/detail/bcilpkkbokcopmabingnndookdogmbna