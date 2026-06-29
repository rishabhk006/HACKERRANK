#include <bits/stdc++.h>

using namespace std;

string ltrim(const string &);
string rtrim(const string &);
vector<string> split(const string &);

/*
 * Complete the 'matrixRotation' function below.
 *
 * The function accepts following parameters:
 *  1. 2D_INTEGER_ARRAY matrix
 *  2. INTEGER r
 */

void matrixRotation(vector<vector<int>> matrix, int r) {
    int m = matrix.size();
    int n = matrix[0].size();

    vector<vector<int>> ans = matrix;

    int layers = min(m, n) / 2;

    for (int layer = 0; layer < layers; layer++) {
        vector<int> v;

        // Top
        for (int j = layer; j < n - layer; j++)
            v.push_back(matrix[layer][j]);

        // Right
        for (int i = layer + 1; i < m - layer - 1; i++)
            v.push_back(matrix[i][n - layer - 1]);

        // Bottom
        for (int j = n - layer - 1; j >= layer; j--)
            v.push_back(matrix[m - layer - 1][j]);

        // Left
        for (int i = m - layer - 2; i > layer; i--)
            v.push_back(matrix[i][layer]);

        int len = v.size();
        int rot = r % len;
        vector<int> t(len);

        for (int i = 0; i < len; i++)
            t[i] = v[(i + rot) % len];

        int k = 0;

        // Top
        for (int j = layer; j < n - layer; j++)
            ans[layer][j] = t[k++];

        // Right
        for (int i = layer + 1; i < m - layer - 1; i++)
            ans[i][n - layer - 1] = t[k++];

        // Bottom
        for (int j = n - layer - 1; j >= layer; j--)
            ans[m - layer - 1][j] = t[k++];

        // Left
        for (int i = m - layer - 2; i > layer; i--)
            ans[i][layer] = t[k++];
    }

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            cout << ans[i][j];
            if (j != n - 1)
                cout << " ";
        }
        cout << endl;
    }
}

int main()
{
    string first_multiple_input_temp;
    getline(cin, first_multiple_input_temp);

    vector<string> first_multiple_input = split(rtrim(first_multiple_input_temp));

    int m = stoi(first_multiple_input[0]);

    int n = stoi(first_multiple_input[1]);

    int r = stoi(first_multiple_input[2]);

    vector<vector<int>> matrix(m);

    for (int i = 0; i < m; i++) {
        matrix[i].resize(n);

        string matrix_row_temp_temp;
        getline(cin, matrix_row_temp_temp);

        vector<string> matrix_row_temp = split(rtrim(matrix_row_temp_temp));

        for (int j = 0; j < n; j++) {
            int matrix_row_item = stoi(matrix_row_temp[j]);

            matrix[i][j] = matrix_row_item;
        }
    }

    matrixRotation(matrix, r);

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