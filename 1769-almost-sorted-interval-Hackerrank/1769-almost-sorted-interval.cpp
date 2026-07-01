#include <bits/stdc++.h>

using namespace std;

string ltrim(const string &);
string rtrim(const string &);
vector<string> split(const string &);

// Binary Indexed Tree (Fenwick Tree) to manage active valid indices
struct BIT {
    int n;
    vector<int> tree;
    BIT(int n) : n(n), tree(n + 1, 0) {}
    
    void update(int i, int delta) {
        // ++i to handle 1-based indexing for BIT
        for (++i; i <= n; i += i & -i) {
            tree[i] += delta;
        }
    }
    
    int query(int i) {
        int sum = 0;
        for (++i; i > 0; i -= i & -i) {
            sum += tree[i];
        }
        return sum;
    }
};

/*
 * Complete the 'solve' function below.
 *
 * The function is expected to return a LONG_LONG.
 * The function accepts INTEGER_ARRAY arr as parameter.
 */
long long solve(vector<int> arr) {
    int n = arr.size();
    BIT bit(n);
    vector<int> stack_min;
    vector<int> stack_max;
    long long ans = 0;

    for (int i = 0; i < n; i++) {
        // If we find a smaller element, previous elements cannot be the minimum
        // for any interval ending at 'i' or beyond. We deactivate them.
        while (!stack_min.empty() && arr[stack_min.back()] > arr[i]) {
            bit.update(stack_min.back(), -1);
            stack_min.pop_back();
        }
        
        // Find the nearest strictly greater element to the left
        while (!stack_max.empty() && arr[stack_max.back()] < arr[i]) {
            stack_max.pop_back();
        }

        int prev_greater = stack_max.empty() ? -1 : stack_max.back();

        // Add current element as a valid interval start candidate
        stack_min.push_back(i);
        bit.update(i, 1);
        stack_max.push_back(i);

        // Count active candidates that are positioned strictly after prev_greater
        ans += bit.query(i) - bit.query(prev_greater);
    }
    return ans;
}

int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    string arr_count_temp;
    getline(cin, arr_count_temp);

    int arr_count = stoi(ltrim(rtrim(arr_count_temp)));

    string arr_temp_temp;
    getline(cin, arr_temp_temp);

    vector<string> arr_temp = split(rtrim(arr_temp_temp));

    vector<int> arr(arr_count);

    for (int i = 0; i < arr_count; i++) {
        int arr_item = stoi(arr_temp[i]);
        arr[i] = arr_item;
    }

    // Capture result as long long to avoid 32-bit integer overflow
    long long result = solve(arr);

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