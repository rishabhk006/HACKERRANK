#include <bits/stdc++.h>

using namespace std;

string ltrim(const string &);
string rtrim(const string &);
vector<string> split(const string &);

// --- Custom fast pair hashing for edge lookups ---
struct pair_hash {
    template <class T1, class T2>
    std::size_t operator () (const std::pair<T1, T2> &p) const {
        return ((size_t)p.first << 32) ^ p.second;
    }
};

struct LCTNode {
    int ch[2];
    int p;
    int val;
    int sum;
    bool rev;
};

vector<LCTNode> tree;
int lct_node_cnt = 0;

// Dynamic array mapping for (admin, server) -> LCT ID
vector<vector<int>> admin_server_to_lct;
// Dynamic array mapping for (admin, server) -> degree
vector<vector<int>> admin_server_degree;

// Fast lookup for edge info and edge LCT mapping using unordered_map with customized fast hash
unordered_map<pair<int, int>, pair<int, int>, pair_hash> edge_info; 
unordered_map<pair<int, int>, vector<int>, pair_hash> edge_admin_lct;

int get_server_lct_id(int admin, int u) {
    if (admin_server_to_lct[admin][u] == 0) {
        lct_node_cnt++;
        if (lct_node_cnt >= (int)tree.size()) tree.resize(lct_node_cnt + 200005);
        admin_server_to_lct[admin][u] = lct_node_cnt;
        tree[lct_node_cnt] = { {0, 0}, 0, 0, 0, false };
    }
    return admin_server_to_lct[admin][u];
}

int get_edge_lct_id(int admin, int u, int v) {
    pair<int, int> edge_key = {u, v};
    auto& vec = edge_admin_lct[edge_key];
    if (admin >= (int)vec.size()) {
        vec.resize(admin + 1, 0);
    }
    if (vec[admin] == 0) {
        lct_node_cnt++;
        if (lct_node_cnt >= (int)tree.size()) tree.resize(lct_node_cnt + 200005);
        vec[admin] = lct_node_cnt;
        tree[lct_node_cnt] = { {0, 0}, 0, 0, 0, false };
    }
    return vec[admin];
}

bool is_root(int x) {
    return tree[tree[x].p].ch[0] != x && tree[tree[x].p].ch[1] != x;
}

void push_up(int x) {
    if (!x) return;
    tree[x].sum = tree[tree[x].ch[0]].sum + tree[tree[x].ch[1]].sum + tree[x].val;
}

void push_down(int x) {
    if (x && tree[x].rev) {
        swap(tree[x].ch[0], tree[x].ch[1]);
        if (tree[x].ch[0]) tree[tree[x].ch[0]].rev ^= 1;
        if (tree[x].ch[1]) tree[tree[x].ch[1]].rev ^= 1;
        tree[x].rev = false;
    }
}

void push_all(int x) {
    if (!is_root(x)) push_all(tree[x].p);
    push_down(x);
}

void rotate(int x) {
    int y = tree[x].p, z = tree[y].p;
    int k = (tree[y].ch[1] == x);
    if (!is_root(y)) tree[z].ch[tree[z].ch[1] == y] = x;
    tree[x].p = z;
    tree[y].ch[k] = tree[x].ch[k ^ 1];
    if (tree[x].ch[k ^ 1]) tree[tree[x].ch[k ^ 1]].p = y;
    tree[x].ch[k ^ 1] = y;
    tree[y].p = x;
    push_up(y);
    push_up(x);
}

void splay(int x) {
    push_all(x);
    while (!is_root(x)) {
        int y = tree[x].p, z = tree[y].p;
        if (!is_root(y)) {
            if ((tree[y].ch[1] == x) ^ (tree[z].ch[1] == y)) rotate(x);
            else rotate(y);
        }
        rotate(x);
    }
}

void access(int x) {
    for (int t = 0; x; t = x, x = tree[x].p) {
        splay(x);
        tree[x].ch[1] = t;
        push_up(x);
    }
}

void make_root(int x) {
    access(x);
    splay(x);
    tree[x].rev ^= 1;
}

int find_root(int x) {
    access(x);
    splay(x);
    while (tree[x].ch[0]) {
        push_down(x);
        x = tree[x].ch[0];
    }
    splay(x);
    return x;
}

void split(int x, int y) {
    make_root(x);
    access(y);
    splay(y);
}

void link(int x, int y) {
    make_root(x);
    if (find_root(y) != x) {
        tree[x].p = y;
    }
}

void cut(int x, int y) {
    make_root(x);
    if (find_root(y) == x && tree[y].p == x && !tree[y].ch[0]) {
        tree[y].p = tree[x].ch[1] = 0;
        push_up(x);
    }
}

/*
 * Complete the 'solve' function below.
 */
vector<string> solve(int s_num, int a_num, vector<vector<int>> links, vector<vector<int>> queries) {
    // Find the maximal administrator id present in initial setup or queries
    int max_admin = a_num + 5;
    for (const auto& lnk : links) max_admin = max(max_admin, lnk[2]);
    for (const auto& qry : queries) {
        if (qry[0] == 1) max_admin = max(max_admin, qry[3]);
        if (qry[0] == 3) max_admin = max(max_admin, qry[3]);
    }
    
    // Direct indexing allocations for O(1) performance
    tree.clear();
    tree.resize(links.size() * 2 + s_num * 2 + queries.size() * 2 + 100005);
    admin_server_to_lct.assign(max_admin + 1, vector<int>(s_num + 1, 0));
    admin_server_degree.assign(max_admin + 1, vector<int>(s_num + 1, 0));
    edge_info.clear();
    edge_admin_lct.clear();
    lct_node_cnt = 0;

    for (const auto& lnk : links) {
        int u = lnk[0];
        int v = lnk[1];
        int k = lnk[2];
        if (u > v) swap(u, v);
        pair<int, int> e = {u, v};
        edge_info[e] = {k, 0};
        
        int u_lct = get_server_lct_id(k, u);
        int v_lct = get_server_lct_id(k, v);
        int edge_lct = get_edge_lct_id(k, u, v);
        
        link(u_lct, edge_lct);
        link(edge_lct, v_lct);
        
        admin_server_degree[k][u]++;
        admin_server_degree[k][v]++;
    }

    vector<string> results;
    results.reserve(queries.size());

    for (const auto& qry : queries) {
        int type = qry[0];
        if (type == 1) {
            int u = qry[1];
            int v = qry[2];
            int new_k = qry[3];
            if (u > v) swap(u, v);
            pair<int, int> e = {u, v};

            auto it = edge_info.find(e);
            if (it == edge_info.end()) {
                results.push_back("Wrong link");
                continue;
            }

            auto& prop = it->second;
            int old_k = prop.first;
            int devices = prop.second;

            if (old_k == new_k) {
                results.push_back("Already controlled link");
                continue;
            }

            if (admin_server_degree[new_k][u] >= 2 || admin_server_degree[new_k][v] >= 2) {
                results.push_back("Server overload");
                continue;
            }

            int new_u_lct = get_server_lct_id(new_k, u);
            int new_v_lct = get_server_lct_id(new_k, v);

            if (find_root(new_u_lct) == find_root(new_v_lct)) {
                results.push_back("Network redundancy");
                continue;
            }

            results.push_back("Assignment done");

            int old_u_lct = get_server_lct_id(old_k, u);
            int old_v_lct = get_server_lct_id(old_k, v);
            int old_edge_lct = get_edge_lct_id(old_k, u, v);
            
            cut(old_u_lct, old_edge_lct);
            cut(old_edge_lct, old_v_lct);
            admin_server_degree[old_k][u]--;
            admin_server_degree[old_k][v]--;

            int new_edge_lct = get_edge_lct_id(new_k, u, v);
            splay(new_edge_lct);
            tree[new_edge_lct].val = devices;
            push_up(new_edge_lct);

            link(new_u_lct, new_edge_lct);
            link(new_edge_lct, new_v_lct);
            admin_server_degree[new_k][u]++;
            admin_server_degree[new_k][v]++;

            prop.first = new_k;
        } 
        else if (type == 2) {
            int u = qry[1];
            int v = qry[2];
            int d = qry[3];
            if (u > v) swap(u, v);
            pair<int, int> e = {u, v};

            auto& prop = edge_info[e];
            int k = prop.first;
            prop.second = d;

            int edge_lct = get_edge_lct_id(k, u, v);
            splay(edge_lct);
            tree[edge_lct].val = d;
            push_up(edge_lct);
        } 
        else if (type == 3) {
            int u = qry[1];
            int v = qry[2];
            int k = qry[3];

            int u_lct = get_server_lct_id(k, u);
            int v_lct = get_server_lct_id(k, v);

            if (find_root(u_lct) != find_root(v_lct)) {
                results.push_back("No connection");
            } else {
                split(u_lct, v_lct);
                results.push_back(to_string(tree[v_lct].sum) + " security devices placed");
            }
        }
    }

    return results;
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    ofstream fout(getenv("OUTPUT_PATH"));

    string first_multiple_input_temp;
    getline(cin, first_multiple_input_temp);

    vector<string> first_multiple_input = split(rtrim(first_multiple_input_temp));

    int s = stoi(first_multiple_input[0]);
    int l = stoi(first_multiple_input[1]);
    int a = stoi(first_multiple_input[2]);
    int t = stoi(first_multiple_input[3]);

    vector<vector<int>> links(l);

    for (int i = 0; i < l; i++) {
        links[i].resize(3);
        string links_row_temp_temp;
        getline(cin, links_row_temp_temp);
        vector<string> links_row_temp = split(rtrim(links_row_temp_temp));
        for (int j = 0; j < 3; j++) {
            links[i][j] = stoi(links_row_temp[j]);
        }
    }

    vector<vector<int>> queries(t);

    for (int i = 0; i < t; i++) {
        queries[i].resize(4);
        string queries_row_temp_temp;
        getline(cin, queries_row_temp_temp);
        vector<string> queries_row_temp = split(rtrim(queries_row_temp_temp));
        for (int j = 0; j < 4; j++) {
            queries[i][j] = stoi(queries_row_temp[j]);
        }
    }

    vector<string> result = solve(s, a, links, queries);

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