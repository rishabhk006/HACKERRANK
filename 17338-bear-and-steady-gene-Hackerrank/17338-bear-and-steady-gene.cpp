#include <bits/stdc++.h>

using namespace std;

string ltrim(const string &);
string rtrim(const string &);

int steadyGene(string gene) {
    int n = gene.length();
    int target = n / 4;
    
    unordered_map<char, int> counts;
    counts['A'] = 0;
    counts['C'] = 0;
    counts['G'] = 0;
    counts['T'] = 0;

    for (char c : gene) {
        counts[c]++;
    }
    
    auto isSteady = [&](const unordered_map<char, int>& current_counts) {
        for (char c : {'A', 'C', 'G', 'T'}) {
            if (current_counts.at(c) > target) {
                return false;
            }
        }
        return true;
    };
    
    if (isSteady(counts)) {
        return 0;
    }
    
    int min_len = n;
    int left = 0;
    
    for (int right = 0; right < n; right++) {
        counts[gene[right]]--;
        
        while (isSteady(counts) && left <= right) {
            min_len = min(min_len, right - left + 1);
            counts[gene[left]]++;
            left++;
        }
    }
    
    return min_len;
}

int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    string n_temp;
    getline(cin, n_temp);

    int n = stoi(ltrim(rtrim(n_temp)));

    string gene;
    getline(cin, gene);

    int result = steadyGene(gene);

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


// Synced seamlessly with LeetHub Pro
// Pro features: https://bit.ly/leethubpro | Free version: https://bit.ly/leethubv4
// Get it here: https://chromewebstore.google.com/detail/bcilpkkbokcopmabingnndookdogmbna