#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const long long MOD = 1000000007;

struct City {
    long long x;
    long long p;
};

struct BIT {
    int n;
    vector<long long> tree;

    BIT(int n) : n(n), tree(n + 1, 0) {}

    void update(int idx, long long val) {
        for (; idx <= n; idx += idx & -idx) {
            tree[idx] += val;
        }
    }

    long long query(int idx) {
        long long sum = 0;
        for (; idx > 0; idx -= idx & -idx) {
            sum += tree[idx];
        }
        return sum;
    }

    long long queryRange(int l, int r) {
        if (l > r) return 0;
        return query(r) - query(l - 1);
    }
};

void solve() {
    int n;
    if (!(cin >> n)) return;

    vector<City> cities(n);
    vector<long long> coords(n);

    for (int i = 0; i < n; ++i) {
        cin >> cities[i].x;
        coords[i] = cities[i].x;
    }

    for (int i = 0; i < n; ++i) {
        cin >> cities[i].p;
    }

    // Coordinate Compression for BIT indexing
    sort(coords.begin(), coords.end());
    coords.erase(unique(coords.begin(), coords.end()), coords.end());

    auto getCompressedIdx = [&](long long x) {
        return lower_bound(coords.begin(), coords.end(), x) - coords.begin() + 1;
    };

    // Sort cities by population in ascending order
    sort(cities.begin(), cities.end(), [](const City& a, const City& b) {
        return a.p < b.p;
    });

    int num_coords = coords.size();
    BIT bit_count(num_coords);
    BIT bit_sum(num_coords);

    long long total_cable_length = 0;

    for (int i = 0; i < n; ++i) {
        long long x = cities[i].x;
        long long p = cities[i].p;
        int idx = getCompressedIdx(x);

        // Cities to the left of x
        long long count_left = bit_count.query(idx - 1);
        long long sum_left = bit_sum.query(idx - 1);
        long long dist_left = (count_left * x - sum_left) % MOD;

        // Cities to the right of x
        long long count_right = bit_count.queryRange(idx + 1, num_coords);
        long long sum_right = bit_sum.queryRange(idx + 1, num_coords);
        long long dist_right = (sum_right - count_right * x) % MOD;

        long long total_dist = (dist_left + dist_right) % MOD;
        if (total_dist < 0) total_dist += MOD;

        long long contribution = (total_dist * (p % MOD)) % MOD;
        total_cable_length = (total_cable_length + contribution) % MOD;

        // Insert current city into Fenwick Trees
        bit_count.update(idx, 1);
        bit_sum.update(idx, x);
    }

    cout << total_cable_length << "\n";
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int t;
    if (cin >> t) {
        while (t--) {
            solve();
        }
    }

    return 0;
}


// Synced seamlessly with LeetHub Pro
// Pro features: https://bit.ly/leethubpro | Free version: https://bit.ly/leethubv4
// Get it here: https://chromewebstore.google.com/detail/bcilpkkbokcopmabingnndookdogmbna