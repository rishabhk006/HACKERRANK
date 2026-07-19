#include <bits/stdc++.h>

using namespace std;

string ltrim(const string &);
string rtrim(const string &);
vector<string> split(const string &);

/*
 * Complete the 'tspGrid' function below.
 *
 * The function is expected to return an INTEGER.
 * The function accepts following parameters:
 *  1. 2D_INTEGER_ARRAY horizontal
 *  2. 2D_INTEGER_ARRAY vertical
 */

int tspGrid(vector<vector<int>> horizontal, vector<vector<int>> vertical) {
    int m = horizontal.size();
    int n = vertical.empty() ? 1 : vertical[0].size();
    
    // A grid graph is bipartite; any Hamiltonian cycle must have an even number of vertices.
    // Also, a cycle cannot be formed on a single line of cells (m=1 or n=1) without repeating nodes.
    if ((m * n) % 2 != 0 || m == 1 || n == 1) {
        return 0;
    }

    // Transpose the grid if needed to ensure the profile size (columns) is optimized
    bool transposed = false;
    if (n > m) {
        transposed = true;
        swap(m, n);
    }

    vector<vector<int>> H(m, vector<int>(n - 1));
    vector<vector<int>> V(m - 1, vector<int>(n));

    if (!transposed) {
        H = horizontal;
        V = vertical;
    } else {
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n - 1; ++j) {
                H[i][j] = vertical[j][i];
            }
        }
        for (int i = 0; i < m - 1; ++i) {
            for (int j = 0; j < n; ++j) {
                V[i][j] = horizontal[j][i];
            }
        }
    }

    // Map to store configuration states and their minimum cost
    unordered_map<long long, int> dp;
    dp[0] = 0;

    for (int i = 0; i < m; ++i) {
        // Transition to a new row: shift all plug indicators left by 1 position (2 bits)
        unordered_map<long long, int> next_dp;
        for (auto const& [state, cost] : dp) {
            if ((state >> (2 * n)) == 0) {
                next_dp[state << 2] = cost;
            }
        }
        dp = move(next_dp);

        for (int j = 0; j < n; ++j) {
            unordered_map<long long, int> next_dp2;
            int left_shift = 2 * j;
            int up_shift = 2 * (j + 1);

            for (auto const& [state, cost] : dp) {
                int plug_left = (state >> left_shift) & 3;
                int plug_up = (state >> up_shift) & 3;

                long long base_state = state & ~((3LL << left_shift) | (3LL << up_shift));

                if (plug_left == 0 && plug_up == 0) {
                    if (i < m - 1 && j < n - 1) {
                        long long new_state = base_state | (1LL << left_shift) | (2LL << up_shift);
                        int new_cost = cost + V[i][j] + H[i][j];
                        if (next_dp2.find(new_state) == next_dp2.end() || next_dp2[new_state] > new_cost) {
                            next_dp2[new_state] = new_cost;
                        }
                    }
                } 
                else if (plug_left > 0 && plug_up == 0) {
                    if (i < m - 1) {
                        long long new_state = base_state | ((long long)plug_left << left_shift);
                        int new_cost = cost + V[i][j];
                        if (next_dp2.find(new_state) == next_dp2.end() || next_dp2[new_state] > new_cost) {
                            next_dp2[new_state] = new_cost;
                        }
                    }
                    if (j < n - 1) {
                        long long new_state = base_state | ((long long)plug_left << up_shift);
                        int new_cost = cost + H[i][j];
                        if (next_dp2.find(new_state) == next_dp2.end() || next_dp2[new_state] > new_cost) {
                            next_dp2[new_state] = new_cost;
                        }
                    }
                } 
                else if (plug_left == 0 && plug_up > 0) {
                    if (i < m - 1) {
                        long long new_state = base_state | ((long long)plug_up << left_shift);
                        int new_cost = cost + V[i][j];
                        if (next_dp2.find(new_state) == next_dp2.end() || next_dp2[new_state] > new_cost) {
                            next_dp2[new_state] = new_cost;
                        }
                    }
                    if (j < n - 1) {
                        long long new_state = base_state | ((long long)plug_up << up_shift);
                        int new_cost = cost + H[i][j];
                        if (next_dp2.find(new_state) == next_dp2.end() || next_dp2[new_state] > new_cost) {
                            next_dp2[new_state] = new_cost;
                        }
                    }
                } 
                else if (plug_left == 1 && plug_up == 1) {
                    int count = 1;
                    int k = j + 2;
                    while (k <= n) {
                        int p = (base_state >> (2 * k)) & 3;
                        if (p == 1) count++;
                        else if (p == 2) count--;
                        if (count == 0) {
                            base_state &= ~(3LL << (2 * k));
                            base_state |= (1LL << (2 * k));
                            break;
                        }
                        k++;
                    }
                    if (next_dp2.find(base_state) == next_dp2.end() || next_dp2[base_state] > cost) {
                        next_dp2[base_state] = cost;
                    }
                } 
                else if (plug_left == 2 && plug_up == 2) {
                    int count = 1;
                    int k = j - 1;
                    while (k >= 0) {
                        int p = (base_state >> (2 * k)) & 3;
                        if (p == 2) count++;
                        else if (p == 1) count--;
                        if (count == 0) {
                            base_state &= ~(3LL << (2 * k));
                            base_state |= (2LL << (2 * k));
                            break;
                        }
                        k--;
                    }
                    if (next_dp2.find(base_state) == next_dp2.end() || next_dp2[base_state] > cost) {
                        next_dp2[base_state] = cost;
                    }
                } 
                else if (plug_left == 2 && plug_up == 1) {
                    if (next_dp2.find(base_state) == next_dp2.end() || next_dp2[base_state] > cost) {
                        next_dp2[base_state] = cost;
                    }
                } 
                else if (plug_left == 1 && plug_up == 2) {
                    if (i == m - 1 && j == n - 1 && base_state == 0) {
                        if (next_dp2.find(base_state) == next_dp2.end() || next_dp2[base_state] > cost) {
                            next_dp2[base_state] = cost;
                        }
                    }
                }
            }
            dp = move(next_dp2);
        }
    }

    return dp.count(0) ? dp[0] : 0;
}

int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    string first_multiple_input_temp;
    getline(cin, first_multiple_input_temp);

    vector<string> first_multiple_input = split(rtrim(first_multiple_input_temp));

    int m = stoi(first_multiple_input[0]);

    int n = stoi(first_multiple_input[1]);

    vector<vector<int>> horizontal(m);

    for (int i = 0; i < m; i++) {
        horizontal[i].resize(n - 1);

        string horizontal_row_temp_temp;
        getline(cin, horizontal_row_temp_temp);

        vector<string> horizontal_row_temp = split(rtrim(horizontal_row_temp_temp));

        for (int j = 0; j < n - 1; j++) {
            int horizontal_row_item = stoi(horizontal_row_temp[j]);

            horizontal[i][j] = horizontal_row_item;
        }
    }

    vector<vector<int>> vertical(m - 1);

    for (int i = 0; i < m - 1; i++) {
        vertical[i].resize(n);

        string vertical_row_temp_temp;
        getline(cin, vertical_row_temp_temp);

        vector<string> vertical_row_temp = split(rtrim(vertical_row_temp_temp));

        for (int j = 0; j < n; j++) {
            int vertical_row_item = stoi(vertical_row_temp[j]);

            vertical[i][j] = vertical_row_item;
        }
    }

    int result = tspGrid(horizontal, vertical);

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