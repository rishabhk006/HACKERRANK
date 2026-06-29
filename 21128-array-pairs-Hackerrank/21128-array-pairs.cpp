#include <bits/stdc++.h>

using namespace std;

string ltrim(const string &);
string rtrim(const string &);
vector<string> split(const string &);

// Structured query format for the offline Fenwick tree execution
struct Query {
    int val;
    int sign;
    int id;
};

struct Fenwick {
    int n;
    vector<int> tree;
    Fenwick(int n) : n(n), tree(n + 1, 0) {}
    void add(int i, int delta) {
        for (; i <= n; i += i & -i) tree[i] += delta;
    }
    int query(int i) {
        int sum = 0;
        for (; i > 0; i -= i & -i) sum += tree[i];
        return sum;
    }
};

long solve(vector<int> arr) {
    int n = arr.size();
    if (n == 0) return 0;
    
    vector<int> L(n), R(n);
    vector<int> st;
    
    for (int i = 0; i < n; ++i) {
        while (!st.empty() && arr[st.back()] < arr[i]) {
            st.pop_back();
        }
        L[i] = st.empty() ? -1 : st.back();
        st.push_back(i);
    }
    
    st.clear();
    for (int i = n - 1; i >= 0; --i) {
        while (!st.empty() && arr[st.back()] <= arr[i]) {
            st.pop_back();
        }
        R[i] = st.empty() ? n : st.back();
        st.push_back(i);
    }

    vector<int> sorted_vals = arr;
    sort(sorted_vals.begin(), sorted_vals.end());
    sorted_vals.erase(unique(sorted_vals.begin(), sorted_vals.end()), sorted_vals.end());
    
    auto get_rank = [&](long long target) {
        return upper_bound(sorted_vals.begin(), sorted_vals.end(), target) - sorted_vals.begin();
    };

    vector<vector<Query>> queries_at_pos(n + 2);
    int query_id = 0;

    for (int m = 0; m < n; ++m) {
        int l_bound = L[m] + 1;
        int r_bound = R[m] - 1;
        
        if (m - l_bound < r_bound - m) {
            for (int i = l_bound; i <= m; ++i) {
                long long max_Aj = arr[m] / arr[i];
                int rank_limit = get_rank(max_Aj);
                if (rank_limit == 0) continue;
                
                int q_l = max(m, i + 1);
                int q_r = r_bound;
                if (q_l <= q_r) {
                    queries_at_pos[q_l - 1 + 1].push_back({rank_limit, -1, query_id});
                    queries_at_pos[q_r + 1].push_back({rank_limit, 1, query_id});
                    query_id++;
                }
            }
        } else {
            for (int j = m; j <= r_bound; ++j) {
                long long max_Ai = arr[m] / arr[j];
                int rank_limit = get_rank(max_Ai);
                if (rank_limit == 0) continue;
                
                int q_l = l_bound;
                int q_r = min(m, j - 1);
                if (q_l <= q_r) {
                    queries_at_pos[q_l - 1 + 1].push_back({rank_limit, -1, query_id});
                    queries_at_pos[q_r + 1].push_back({rank_limit, 1, query_id});
                    query_id++;
                }
            }
        }
    }

    vector<long long> query_results(query_id, 0);
    Fenwick bit(sorted_vals.size());

    for (int i = 0; i < n; ++i) {
        int rk = lower_bound(sorted_vals.begin(), sorted_vals.end(), arr[i]) - sorted_vals.begin() + 1;
        bit.add(rk, 1);
        
        int current_pos = i + 1;
        for (const auto& q : queries_at_pos[current_pos]) {
            query_results[q.id] += q.sign * bit.query(q.val);
        }
    }

    long long total_pairs = 0;
    for (int i = 0; i < query_id; ++i) {
        total_pairs += query_results[i];
    }

    return total_pairs;
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    ofstream fout(getenv("OUTPUT_PATH"));

    string arr_count_temp;
    if (!getline(cin, arr_count_temp)) return 0;

    int arr_count = stoi(ltrim(rtrim(arr_count_temp)));

    string arr_temp_temp;
    getline(cin, arr_temp_temp);

    vector<string> arr_temp = split(rtrim(arr_temp_temp));

    vector<int> arr(arr_count);

    for (int i = 0; i < arr_count; i++) {
        int arr_item = stoi(arr_temp[i]);
        arr[i] = arr_item;
    }

    long result = solve(arr);

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