#include <bits/stdc++.h>

using namespace std;

string ltrim(const string &);
string rtrim(const string &);
vector<string> split(const string &);

// Global variable to capture the tree root from main
int global_root = 1;

/*
 * Complete the 'solve' function below.
 *
 * The function is expected to return an INTEGER_ARRAY.
 * The function accepts following parameters:
 *  1. 2D_INTEGER_ARRAY tree
 *  2. INTEGER_ARRAY color
 *  3. INTEGER_ARRAY s
 */
vector<int> solve(vector<vector<int>> tree_edges, vector<int> color, vector<int> s) {
    int n = color.size();
    
    // Reconstruct adjacency list (1-indexed nodes)
    vector<vector<int>> adj(n + 1);
    for (const auto& edge : tree_edges) {
        int u = edge[0];
        int v = edge[1];
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    
    vector<int> ans(n + 1, 0);
    vector<set<int>> subtree_sets(n + 1);
    
    // Small-to-Large merging DFS
    struct DFS {
        const vector<vector<int>>& adj;
        const vector<int>& color;
        vector<int>& ans;
        vector<set<int>>& subtree_sets;
        
        void run(int u, int p) {
            int max_child = -1;
            
            for (int v : adj[u]) {
                if (v != p) {
                    run(v, u);
                    if (max_child == -1 || subtree_sets[v].size() > subtree_sets[max_child].size()) {
                        max_child = v;
                    }
                }
            }
            
            // Move largest child's set to the parent in O(1)
            if (max_child != -1) {
                subtree_sets[u].swap(subtree_sets[max_child]);
            }
            
            // Merge all smaller children sets
            for (int v : adj[u]) {
                if (v != p && v != max_child) {
                    subtree_sets[u].insert(subtree_sets[v].begin(), subtree_sets[v].end());
                    subtree_sets[v].clear(); // Free memory early
                }
            }
            
            // Insert current node's color (color vector input is 0-indexed)
            subtree_sets[u].insert(color[u - 1]);
            ans[u] = subtree_sets[u].size();
        }
    } tree_dfs{adj, color, ans, subtree_sets};
    
    // Execute tree traversal starting from the correctly identified global root
    tree_dfs.run(global_root, 0);
    
    // Map answers for queries
    vector<int> result(s.size());
    for (size_t i = 0; i < s.size(); i++) {
        result[i] = ans[s[i]];
    }
    
    return result;
}

int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    string first_multiple_input_temp;
    getline(cin, first_multiple_input_temp);

    vector<string> first_multiple_input = split(rtrim(first_multiple_input_temp));

    int n = stoi(first_multiple_input[0]);

    int m = stoi(first_multiple_input[1]);

    int r = stoi(first_multiple_input[2]);
    
    // Capture the dynamic root globally
    global_root = r;

    vector<vector<int>> tree(n - 1);

    for (int i = 0; i < n - 1; i++) {
        tree[i].resize(2);

        string tree_row_temp_temp;
        getline(cin, tree_row_temp_temp);

        vector<string> tree_row_temp = split(rtrim(tree_row_temp_temp));

        for (int j = 0; j < 2; j++) {
            int tree_row_item = stoi(tree_row_temp[j]);

            tree[i][j] = tree_row_item;
        }
    }

    vector<int> color(n);

    for (int i = 0; i < n; i++) {
        string color_item_temp;
        getline(cin, color_item_temp);

        int color_item = stoi(ltrim(rtrim(color_item_temp)));

        color[i] = color_item;
    }

    vector<int> s(m);

    for (int i = 0; i < m; i++) {
        string s_item_temp;
        getline(cin, s_item_temp);

        int s_item = stoi(ltrim(rtrim(s_item_temp)));

        s[i] = s_item;
    }

    vector<int> result = solve(tree, color, s);

    for (size_t i = 0; i < result.size(); i++) {
        fout << result[i];

        if (i != result.size() - 1) {
            fout << "\n";
        }
    }

    fout << "\n";

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