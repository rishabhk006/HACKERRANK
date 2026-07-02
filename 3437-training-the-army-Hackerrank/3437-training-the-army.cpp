#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <queue>
#include <cstring>

using namespace std;

const int INF = 1e9;

struct Edge {
    int to, cap, cost, flow, rev;
};

vector<vector<Edge>> adj;
vector<int> dist_arr, parent_node, parent_edge;
vector<bool> in_queue;

void add_edge(int from, int to, int cap, int cost) {
    adj[from].push_back({to, cap, cost, 0, (int)adj[to].size()});
    adj[to].push_back({from, 0, -cost, 0, (int)adj[from].size() - 1});
}

bool spfa(int s, int t, int num_nodes) {
    dist_arr.assign(num_nodes, INF);
    parent_node.assign(num_nodes, -1);
    parent_edge.assign(num_nodes, -1);
    in_queue.assign(num_nodes, false);
    
    queue<int> q;
    dist_arr[s] = 0;
    q.push(s);
    in_queue[s] = true;
    
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        in_queue[u] = false;
        
        for (int i = 0; i < (int)adj[u].size(); ++i) {
            Edge &e = adj[u][i];
            if (e.cap - e.flow > 0 && dist_arr[e.to] > dist_arr[u] + e.cost) {
                dist_arr[e.to] = dist_arr[u] + e.cost;
                parent_node[e.to] = u;
                parent_edge[e.to] = i;
                if (!in_queue[e.to]) {
                    q.push(e.to);
                    in_queue[e.to] = true;
                }
            }
        }
    }
    return dist_arr[t] != INF;
}

pair<int, int> min_cost_max_flow(int s, int t, int num_nodes) {
    int flow = 0, cost = 0;
    while (spfa(s, t, num_nodes)) {
        int push = INF;
        for (int v = t; v != s; v = parent_node[v]) {
            int u = parent_node[v];
            int idx = parent_edge[v];
            push = min(push, adj[u][idx].cap - adj[u][idx].flow);
        }
        for (int v = t; v != s; v = parent_node[v]) {
            int u = parent_node[v];
            int idx = parent_edge[v];
            adj[u][idx].flow += push;
            adj[adj[u][idx].to][adj[u][idx].rev].flow -= push;
            cost += push * adj[u][idx].cost;
        }
        flow += push;
    }
    return {flow, cost};
}

int main() {
    int n, m;
    if (!(cin >> n >> m)) return 0;
    
    vector<int> a(n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }
    
    int source = 0;
    int sink = n + 2 * m + 1;
    int num_nodes = sink + 1;
    adj.resize(num_nodes);
    
    for (int i = 0; i < n; ++i) {
        int skillset_node = i + 1;
        add_edge(source, skillset_node, a[i], 0);
        add_edge(skillset_node, sink, 1, -1);
        add_edge(skillset_node, sink, INF, 0);
    }
    
    for (int j = 0; j < m; ++j) {
        int w_in = n + 2 * j + 1;
        int w_out = n + 2 * j + 2;
        add_edge(w_in, w_out, INF, 0);
        
        int len_s;
        cin >> len_s;
        for (int k = 0; k < len_s; ++k) {
            int s;
            cin >> s;
            add_edge(s, w_in, 1, 0);
        }
        
        int len_d;
        cin >> len_d;
        for (int k = 0; k < len_d; ++k) {
            int d;
            cin >> d;
            add_edge(w_out, d, 1, 0);
        }
    }
    
    pair<int, int> result = min_cost_max_flow(source, sink, num_nodes);
    cout << -result.second << endl;
    
    return 0;
}


// Synced seamlessly with LeetHub Pro
// Pro features: https://bit.ly/leethubpro | Free version: https://bit.ly/leethubv4
// Get it here: https://chromewebstore.google.com/detail/bcilpkkbokcopmabingnndookdogmbna