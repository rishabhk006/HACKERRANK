#include <bits/stdc++.h>

using namespace std;

struct Query {
    int l, r, id;
    int block_idx;
    
    bool operator<(const Query& other) const {
        if (block_idx != other.block_idx) {
            return block_idx < other.block_idx;
        }
        return (block_idx & 1) ? (r < other.r) : (r > other.r);
    }
};

vector<int> solve(int v, vector<vector<int>> ships, vector<vector<int>> queries) {
    int n = ships.size();
    int q = queries.size();
    
    sort(ships.begin(), ships.end(), [](const vector<int>& a, const vector<int>& b) {
        return a[1] < b[1];
    });
    
    vector<int> y_coords(n);
    for (int i = 0; i < n; i++) {
        y_coords[i] = ships[i][1];
    }
    
    vector<int> all_freqs(n);
    for (int i = 0; i < n; i++) {
        all_freqs[i] = ships[i][2];
    }
    sort(all_freqs.begin(), all_freqs.end());
    all_freqs.erase(unique(all_freqs.begin(), all_freqs.end()), all_freqs.end());
    
    vector<int> ship_freq_idx(n);
    for (int i = 0; i < n; i++) {
        ship_freq_idx[i] = lower_bound(all_freqs.begin(), all_freqs.end(), ships[i][2]) - all_freqs.begin();
    }
    
    int block_size = max(1, (int)(n / sqrt(q)));
    vector<Query> mo_queries;
    vector<int> ans(q, 0);
    
    for (int i = 0; i < q; i++) {
        int yd = queries[i][1];
        int yu = queries[i][0];
        
        int l = lower_bound(y_coords.begin(), y_coords.end(), yd) - y_coords.begin();
        int r = upper_bound(y_coords.begin(), y_coords.end(), yu) - y_coords.begin() - 1;
        
        if (l <= r) {
            mo_queries.push_back({l, r, i, l / block_size});
        } else {
            ans[i] = 0;
        }
    }
    
    sort(mo_queries.begin(), mo_queries.end());
    
    vector<int> freq_count(all_freqs.size(), 0);
    vector<int> max_freq_bucket(n + 1, 0); 
    max_freq_bucket[0] = all_freqs.size();
    
    int current_max = 0;
    
    auto add = [&](int idx) {
        int f = ship_freq_idx[idx];
        int old_cnt = freq_count[f];
        int new_cnt = old_cnt + 1;
        
        freq_count[f] = new_cnt;
        max_freq_bucket[old_cnt]--;
        max_freq_bucket[new_cnt]++;
        
        if (new_cnt > current_max) {
            current_max = new_cnt;
        }
    };
    
    auto remove = [&](int idx) {
        int f = ship_freq_idx[idx];
        int old_cnt = freq_count[f];
        int new_cnt = old_cnt - 1;
        
        freq_count[f] = new_cnt;
        max_freq_bucket[old_cnt]--;
        max_freq_bucket[new_cnt]++;
        
        if (old_cnt == current_max && max_freq_bucket[old_cnt] == 0) {
            current_max = new_cnt;
        }
    };
    
    int curr_l = 0, curr_r = -1;
    for (const auto& qry : mo_queries) {
        while (curr_l > qry.l) add(--curr_l);
        while (curr_r < qry.r) add(++curr_r);
        while (curr_l < qry.l) remove(curr_l++);
        while (curr_r > qry.r) remove(curr_r--);
        
        ans[qry.id] = current_max;
    }
    
    return ans;
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, q, v;
    if (!(cin >> n >> q >> v)) return 0;

    vector<vector<int>> ships(n, vector<int>(3));
    for (int i = 0; i < n; i++) {
        cin >> ships[i][0] >> ships[i][1] >> ships[i][2];
    }

    vector<vector<int>> queries(q, vector<int>(3));
    for (int i = 0; i < q; i++) {
        cin >> queries[i][0] >> queries[i][1] >> queries[i][2];
    }

    vector<int> result = solve(v, ships, queries);

    for (size_t i = 0; i < result.size(); i++) {
        cout << result[i] << "\n";
    }

    return 0;
}


// Synced seamlessly with LeetHub Pro
// Pro features: https://bit.ly/leethubpro | Free version: https://bit.ly/leethubv4
// Get it here: https://chromewebstore.google.com/detail/bcilpkkbokcopmabingnndookdogmbna