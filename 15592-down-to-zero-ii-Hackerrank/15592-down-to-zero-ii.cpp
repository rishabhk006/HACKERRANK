#include <bits/stdc++.h>

using namespace std;

string ltrim(const string &);
string rtrim(const string &);

/*
 * Complete the 'downToZero' function below.
 *
 * The function is expected to return an INTEGER.
 * The function accepts INTEGER n as parameter.
 */

int downToZero(int n) {
static vector<int> dp;
    static bool done = false;

    if (!done) {
        const int MAX = 1000000;
        dp.resize(MAX + 1);

        for (int i = 0; i <= MAX; i++)
            dp[i] = i;

        for (int i = 1; i <= MAX; i++) {
            if (i + 1 <= MAX)
                dp[i + 1] = min(dp[i + 1], dp[i] + 1);

            for (int j = 2; j <= i && 1LL * i * j <= MAX; j++) {
                dp[i * j] = min(dp[i * j], dp[i] + 1);
            }
        }

        done = true;
    }

    return dp[n];
}

int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    string q_temp;
    getline(cin, q_temp);

    int q = stoi(ltrim(rtrim(q_temp)));

    for (int q_itr = 0; q_itr < q; q_itr++) {
        string n_temp;
        getline(cin, n_temp);

        int n = stoi(ltrim(rtrim(n_temp)));

        int result = downToZero(n);

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