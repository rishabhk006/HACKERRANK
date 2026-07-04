#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

int BLOCK_SIZE;

struct Query {
    int l, r, id;
    bool operator<(const Query& other) const {
        int block_own = l / BLOCK_SIZE;
        int block_other = other.l / BLOCK_SIZE;
        if (block_own != block_other) {
            return block_own < block_other;
        }
        return (block_own & 1) ? (r < other.r) : (r > other.r);
    }
};

struct Fenwick {
    int size;
    vector<int> tree;
    Fenwick(int n) : size(n), tree(n + 1, 0) {}
    
    void update(int i, int delta) {
        for (; i <= size; i += i & -i) {
            tree[i] += delta;
        }
    }
    
    int query(int i) {
        int sum = 0;
        for (; i > 0; i -= i & -i) {
            sum += tree[i];
        }
        return sum;
    }
    
    int query_range(int l, int r) {
        if (l > r) return 0;
        return query(r) - query(l - 1);
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    long long k;
    if (!(cin >> n >> k)) return 0;

    vector<long long> h(n);
    vector<long long> coords;
    coords.reserve(n);
    for (int i = 0; i < n; i++) {
        cin >> h[i];
        coords.push_back(h[i]);
    }

    sort(coords.begin(), coords.end());
    coords.erase(unique(coords.begin(), coords.end()), coords.end());
    int m = coords.size();

    vector<int> h_compressed(n);
    vector<int> lower_bound_idx(n);
    vector<int> upper_bound_idx(n);

    for (int i = 0; i < n; i++) {
        h_compressed[i] = lower_bound(coords.begin(), coords.end(), h[i]) - coords.begin() + 1;
        
        auto it_low = lower_bound(coords.begin(), coords.end(), h[i] - k);
        lower_bound_idx[i] = it_low - coords.begin() + 1;
        
        auto it_high = upper_bound(coords.begin(), coords.end(), h[i] + k);
        upper_bound_idx[i] = it_high - coords.begin();
    }

    int q;
    cin >> q;
    vector<Query> queries(q);
    BLOCK_SIZE = max(1, (int)(n / sqrt(q)));

    for (int i = 0; i < q; i++) {
        cin >> queries[i].l >> queries[i].r;
        queries[i].id = i;
    }

    sort(queries.begin(), queries.end());

    Fenwick bit(m);
    vector<long long> answers(q);
    long long current_pairs = 0;
    int cur_l = 0, cur_r = -1;

    auto add = [&](int idx) {
        current_pairs += bit.query_range(lower_bound_idx[idx], upper_bound_idx[idx]);
        bit.update(h_compressed[idx], 1);
    };

    auto remove = [&](int idx) {
        bit.update(h_compressed[idx], -1);
        current_pairs -= bit.query_range(lower_bound_idx[idx], upper_bound_idx[idx]);
    };

    for (const auto& query : queries) {
        while (cur_l > query.l) {
            cur_l--;
            add(cur_l);
        }
        while (cur_r < query.r) {
            cur_r++;
            add(cur_r);
        }
        while (cur_l < query.l) {
            remove(cur_l);
            cur_l++;
        }
        while (cur_r > query.r) {
            remove(cur_r);
            cur_r--;
        }
        answers[query.id] = current_pairs;
    }

    for (int i = 0; i < q; i++) {
        cout << answers[i] << "\n";
    }

    return 0;
}


// Synced seamlessly with LeetHub Pro
// Pro features: https://bit.ly/leethubpro | Free version: https://bit.ly/leethubv4
// Get it here: https://chromewebstore.google.com/detail/bcilpkkbokcopmabingnndookdogmbna