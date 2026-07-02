#include <bits/stdc++.h>

using namespace std;

string ltrim(const string &);
string rtrim(const string &);
vector<string> split(const string &);

bool dfs(int u, const vector<vector<int>>& adj, vector<int>& match, vector<bool>& visited) {
    for (int v : adj[u]) {
        if (!visited[v]) {
            visited[v] = true;
            if (match[v] < 0 || dfs(match[v], adj, match, visited)) {
                match[v] = u;
                return true;
            }
        }
    }
    return false;
}

int problemSolving(int k, vector<int> v) {
    int n = v.size();
    vector<vector<int>> adj(n);
    
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (abs(v[i] - v[j]) >= k) {
                adj[i].push_back(j);
            }
        }
    }
    
    int matching_size = 0;
    vector<int> match(n, -1);
    
    for (int i = 0; i < n; i++) {
        vector<bool> visited(n, false);
        if (dfs(i, adj, match, visited)) {
            matching_size++;
        }
    }
    
    return n - matching_size;
}

int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    string t_temp;
    getline(cin, t_temp);

    int t = stoi(ltrim(rtrim(t_temp)));

    for (int t_itr = 0; t_itr < t; t_itr++) {
        string first_multiple_input_temp;
        getline(cin, first_multiple_input_temp);

        vector<string> first_multiple_input = split(rtrim(first_multiple_input_temp));

        int n = stoi(first_multiple_input[0]);
        int k = stoi(first_multiple_input[1]);

        string v_temp_temp;
        getline(cin, v_temp_temp);

        vector<string> v_temp = split(rtrim(v_temp_temp));

        vector<int> v(n);

        for (int i = 0; i < n; i++) {
            int v_item = stoi(v_temp[i]);
            v[i] = v_item;
        }

        int result = problemSolving(k, v);

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