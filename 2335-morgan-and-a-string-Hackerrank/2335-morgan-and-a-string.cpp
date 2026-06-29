#include <bits/stdc++.h>

using namespace std;

string ltrim(const string &);
string rtrim(const string &);

/*
 * Complete the 'morganAndString' function below.
 *
 * The function is expected to return a STRING.
 * The function accepts following parameters:
 *  1. STRING a
 *  2. STRING b
 */

string morganAndString(string a, string b) {
    // Append a sentinel character larger than 'Z' to handle matching suffixes cleanly
    a += 'z';
    b += 'z';
    
    int n = a.length();
    int m = b.length();
    string result = "";
    result.reserve(n + m - 2); // Pre-allocate memory for optimal performance
    
    int i = 0, j = 0;
    while (i < n - 1 && j < m - 1) {
        if (a[i] < b[j]) {
            result += a[i++];
        } else if (a[i] > b[j]) {
            result += b[j++];
        } else {
            // When characters match, look ahead by comparing the remaining substrings
            if (a.compare(i, n - i, b, j, m - j) < 0) {
                result += a[i++];
            } else {
                result += b[j++];
            }
        }
    }
    
    // Append any remaining characters from either stack
    while (i < n - 1) result += a[i++];
    while (j < m - 1) result += b[j++];
    
    return result;
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

        string result = morganAndString(a, b);

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