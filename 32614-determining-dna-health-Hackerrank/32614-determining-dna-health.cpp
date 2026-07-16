#include <bits/stdc++.h>

using namespace std;

struct TrieNode {
    int children[26];
    int fail;
    int next_valid_fail; // Points directly to the closest fail node that contains genes
    vector<pair<int, int>> gene_indices; // ONLY stores genes that end EXACTLY at this node

    TrieNode() {
        memset(children, -1, sizeof(children));
        fail = -1;
        next_valid_fail = -1;
    }
};

vector<TrieNode> trie;

void insert_gene(const string& gene, int gene_idx, int health_val) {
    int current = 0;
    for (char c : gene) {
        int idx = c - 'a';
        if (trie[current].children[idx] == -1) {
            trie[current].children[idx] = trie.size();
            trie.push_back(TrieNode());
        }
        current = trie[current].children[idx];
    }
    trie[current].gene_indices.push_back({gene_idx, health_val});
}

void build_aho_corasick() {
    queue<int> q;
    trie[0].next_valid_fail = 0;

    for (int i = 0; i < 26; ++i) {
        if (trie[0].children[i] != -1) {
            trie[trie[0].children[i]].fail = 0;
            // The root (0) acts as the base termination point
            trie[trie[0].children[i]].next_valid_fail = 0; 
            q.push(trie[0].children[i]);
        } else {
            trie[0].children[i] = 0;
        }
    }

    while (!q.empty()) {
        int current = q.front();
        q.pop();

        for (int i = 0; i < 26; ++i) {
            int child = trie[current].children[i];
            if (child != -1) {
                int fail_node = trie[current].fail;
                while (trie[fail_node].children[i] == -1 && fail_node != 0) {
                    fail_node = trie[fail_node].fail;
                }
                int f = (trie[fail_node].children[i] != -1) ? trie[fail_node].children[i] : 0;
                trie[child].fail = f;

                // Path compression: If the failure node has no genes, 
                // skip it and point to its next_valid_fail instead.
                if (!trie[f].gene_indices.empty()) {
                    trie[child].next_valid_fail = f;
                } else {
                    trie[child].next_valid_fail = trie[f].next_valid_fail;
                }

                q.push(child);
            }
        }
    }
    
    // Sort local gene vectors individually to allow binary searching
    for (size_t i = 0; i < trie.size(); ++i) {
        if (!trie[i].gene_indices.empty()) {
            sort(trie[i].gene_indices.begin(), trie[i].gene_indices.end());
        }
    }
}

// Counts health contribution for a specific node using binary search
long long calculate_node_health(int node_idx, int first, int last) {
    const auto& vec = trie[node_idx].gene_indices;
    if (vec.empty()) return 0;

    auto it_start = lower_bound(vec.begin(), vec.end(), make_pair(first, 0));
    auto it_end = upper_bound(vec.begin(), vec.end(), make_pair(last, INT_MAX));

    long long current_health = 0;
    for (auto it = it_start; it != it_end; ++it) {
        current_health += it->second;
    }
    return current_health;
}

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    if (!(cin >> n)) return 0;

    vector<string> genes(n);
    for (int i = 0; i < n; i++) {
        cin >> genes[i];
    }

    vector<int> health(n);
    for (int i = 0; i < n; i++) {
        cin >> health[i];
    }

    trie.push_back(TrieNode());
    for (int i = 0; i < n; i++) {
        insert_gene(genes[i], i, health[i]);
    }

    build_aho_corasick();

    int s;
    if (!(cin >> s)) return 0;

    long long min_health = LLONG_MAX;
    long long max_health = LLONG_MIN;

    for (int s_itr = 0; s_itr < s; s_itr++) {
        int first, last;
        string d;
        cin >> first >> last >> d;

        long long total_health = 0;
        int current_state = 0;

        for (char c : d) {
            int idx = c - 'a';
            while (trie[current_state].children[idx] == -1 && current_state != 0) {
                current_state = trie[current_state].fail;
            }
            current_state = trie[current_state].children[idx];
            if (current_state == -1) current_state = 0;

            // Follow the compressed failure chain to collect health points
            int temp_state = current_state;
            while (temp_state > 0) {
                total_health += calculate_node_health(temp_state, first, last);
                temp_state = trie[temp_state].next_valid_fail;
            }
        }

        min_health = min(min_health, total_health);
        max_health = max(max_health, total_health);
    }

    cout << min_health << " " << max_health << "\n";

    return 0;
}


// Synced seamlessly with LeetHub Pro
// Pro features: https://bit.ly/leethubpro | Free version: https://bit.ly/leethubv4
// Get it here: https://chromewebstore.google.com/detail/bcilpkkbokcopmabingnndookdogmbna