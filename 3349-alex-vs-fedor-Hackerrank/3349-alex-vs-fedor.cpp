#include <bits/stdc++.h>

using namespace std;

string ltrim(const string &);
string rtrim(const string &);
vector<string> split(const string &);

// Disjoint Set Union (DSU) Structure
struct DSU {
    vector<int> parent;
    DSU(int n) {
        parent.resize(n);
        iota(parent.begin(), parent.end(), 0);
    }
    int find(int i) {
        if (parent[i] == i)
            return i;
        return parent[i] = find(parent[i]);
    }
    bool unite(int i, int j) {
        int root_i = find(i);
        int root_j = find(j);
        if (root_i != root_j) {
            parent[root_i] = root_j;
            return true;
        }
        return false;
    }
};

struct Edge {
    int u, v, weight;
};

// Computes the exact determinant of an integer matrix using the Euclidean algorithm
long long exact_determinant(vector<vector<long long>> mat) {
    int n = mat.size();
    __int128 det = 1;
    bool sign = false;

    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            while (mat[j][i] != 0) {
                long long t = mat[i][i] / mat[j][i];
                for (int k = i; k < n; ++k) {
                    mat[i][k] -= t * mat[j][k];
                }
                swap(mat[i], mat[j]);
                sign = !sign;
            }
        }
        if (mat[i][i] == 0) return 0;
        det *= mat[i][i];
    }
    
    long long final_det = (long long)det;
    return sign ? -final_det : final_det;
}

// Counts spanning trees of a graph given its adjacency structure using Matrix Tree Theorem
long long count_spanning_trees(int num_nodes, const vector<pair<int, int>>& edges) {
    if (num_nodes <= 1) return 1;
    vector<vector<long long>> laplacian(num_nodes, vector<long long>(num_nodes, 0));
    for (auto& edge : edges) {
        int u = edge.first;
        int v = edge.second;
        laplacian[u][u]++;
        laplacian[v][v]++;
        laplacian[u][v]--;
        laplacian[v][u]--;
    }
    // Remove the last row and column
    vector<vector<long long>> reduced(num_nodes - 1, vector<long long>(num_nodes - 1, 0));
    for (int i = 0; i < num_nodes - 1; ++i) {
        for (int j = 0; j < num_nodes - 1; ++j) {
            reduced[i][j] = laplacian[i][j];
        }
    }
    return abs(exact_determinant(reduced));
}

/*
 * Complete the 'alexFedor' function below.
 */
string alexFedor(int graph_nodes, vector<int> graph_from, vector<int> graph_to, vector<int> graph_weight) {
    int m = graph_from.size();
    vector<Edge> edges(m);
    vector<pair<int, int>> total_edges_list;
    
    for (int i = 0; i < m; ++i) {
        edges[i] = {graph_from[i] - 1, graph_to[i] - 1, graph_weight[i]};
        total_edges_list.push_back({graph_from[i] - 1, graph_to[i] - 1});
    }

    // 1. Calculate the total number of spanning trees in the entire graph
    long long total_spanning_trees = count_spanning_trees(graph_nodes, total_edges_list);
    if (total_spanning_trees == 0) return "0/1";

    // 2. Count the number of Minimum Spanning Trees (MSTs)
    sort(edges.begin(), edges.end(), [](const Edge& a, const Edge& b) {
        return a.weight < b.weight;
    });

    DSU dsu(graph_nodes);
    long long mst_count = 1;

    int idx = 0;
    while (idx < m) {
        int j = idx;
        while (j < m && edges[j].weight == edges[idx].weight) j++;

        // Group component vertices involved in this weight level
        map<int, int> comp_to_local;
        vector<pair<int, int>> weight_edges;

        for (int k = idx; k < j; ++k) {
            int root_u = dsu.find(edges[k].u);
            int root_v = dsu.find(edges[k].v);
            if (root_u != root_v) {
                if (comp_to_local.find(root_u) == comp_to_local.end()) {
                    int sz = comp_to_local.size();
                    comp_to_local[root_u] = sz;
                }
                if (comp_to_local.find(root_v) == comp_to_local.end()) {
                    int sz = comp_to_local.size();
                    comp_to_local[root_v] = sz;
                }
                weight_edges.push_back({comp_to_local[root_u], comp_to_local[root_v]});
            }
        }

        // Find independent sub-forest chunks within this weight group
        int sub_nodes = comp_to_local.size();
        if (sub_nodes > 0) {
            DSU local_dsu(sub_nodes);
            vector<vector<pair<int, int>>> subgraphs(sub_nodes);
            for (auto& e : weight_edges) {
                local_dsu.unite(e.first, e.second);
            }

            map<int, vector<pair<int, int>>> component_edges;
            map<int, set<int>> component_vertices;
            for (auto& e : weight_edges) {
                int root = local_dsu.find(e.first);
                component_edges[root].push_back(e);
                component_vertices[root].insert(e.first);
                component_vertices[root].insert(e.second);
            }

            for (auto& item : component_edges) {
                int root = item.first;
                auto& e_list = item.second;
                auto& v_set = component_vertices[root];
                
                // Remap to continuous indices for Matrix Tree evaluation
                map<int, int> remap;
                for (int v : v_set) {
                    int sz = remap.size();
                    remap[v] = sz;
                }
                vector<pair<int, int>> final_sub_edges;
                for (auto& e : e_list) {
                    final_sub_edges.push_back({remap[e.first], remap[e.second]});
                }
                mst_count *= count_spanning_trees(remap.size(), final_sub_edges);
            }
        }

        // Contract the global components for the next weight level
        for (int k = idx; k < j; ++k) {
            dsu.unite(edges[k].u, edges[k].v);
        }
        idx = j;
    }

    // Reduce the fraction using GCD
    long long common_divisor = __gcd(mst_count, total_spanning_trees);
    mst_count /= common_divisor;
    total_spanning_trees /= common_divisor;

    return to_string(mst_count) + "/" + to_string(total_spanning_trees);
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    ofstream fout(getenv("OUTPUT_PATH"));

    string graph_nodes_edges_temp;
    if (!getline(cin, graph_nodes_edges_temp)) return 0;

    vector<string> graph_nodes_edges = split(rtrim(graph_nodes_edges_temp));

    int graph_nodes = stoi(graph_nodes_edges[0]);
    int graph_edges = stoi(graph_nodes_edges[1]);

    vector<int> graph_from(graph_edges);
    vector<int> graph_to(graph_edges);
    vector<int> graph_weight(graph_edges);

    for (int i = 0; i < graph_edges; i++) {
        string graph_from_to_weight_temp;
        getline(cin, graph_from_to_weight_temp);

        vector<string> graph_from_to_weight = split(rtrim(graph_from_to_weight_temp));

        int graph_from_temp = stoi(graph_from_to_weight[0]);
        int graph_to_temp = stoi(graph_from_to_weight[1]);
        int graph_weight_temp = stoi(graph_from_to_weight[2]);

        graph_from[i] = graph_from_temp;
        graph_to[i] = graph_to_temp;
        graph_weight[i] = graph_weight_temp;
    }

    string result = alexFedor(graph_nodes, graph_from, graph_to, graph_weight);

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