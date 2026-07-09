#include <bits/stdc++.h>

using namespace std;

string ltrim(const string &);
string rtrim(const string &);
vector<string> split(const string &);

/*
 * Complete the 'solve' function below.
 *
 * The function is expected to return an INTEGER.
 * The function accepts following parameters:
 *  1. 2D_INTEGER_ARRAY shots
 *  2. 2D_INTEGER_ARRAY players
 */

int solve(vector<vector<int>> shots, vector<vector<int>> players) {
    vector<int> start, finish;

    for (auto &s : shots) {
        start.push_back(s[0]);
        finish.push_back(s[1]);
    }

    sort(start.begin(), start.end());
    sort(finish.begin(), finish.end());

    long long ans = 0;

    for (auto &p : players) {
        int l = p[0], r = p[1];

        int left = upper_bound(start.begin(), start.end(), r) - start.begin();
        int right = lower_bound(finish.begin(), finish.end(), l) - finish.begin();

        ans += (left - right);
    }

    return (int)ans;
}

int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    string first_multiple_input_temp;
    getline(cin, first_multiple_input_temp);

    vector<string> first_multiple_input = split(rtrim(first_multiple_input_temp));

    int n = stoi(first_multiple_input[0]);

    int m = stoi(first_multiple_input[1]);

    vector<vector<int>> shots(n);

    for (int i = 0; i < n; i++) {
        shots[i].resize(2);

        string shots_row_temp_temp;
        getline(cin, shots_row_temp_temp);

        vector<string> shots_row_temp = split(rtrim(shots_row_temp_temp));

        for (int j = 0; j < 2; j++) {
            int shots_row_item = stoi(shots_row_temp[j]);

            shots[i][j] = shots_row_item;
        }
    }

    vector<vector<int>> players(m);

    for (int i = 0; i < m; i++) {
        players[i].resize(2);

        string players_row_temp_temp;
        getline(cin, players_row_temp_temp);

        vector<string> players_row_temp = split(rtrim(players_row_temp_temp));

        for (int j = 0; j < 2; j++) {
            int players_row_item = stoi(players_row_temp[j]);

            players[i][j] = players_row_item;
        }
    }

    int result = solve(shots, players);

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