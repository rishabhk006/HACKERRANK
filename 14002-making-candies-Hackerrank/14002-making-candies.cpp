#include <bits/stdc++.h>

using namespace std;

string ltrim(const string &);
string rtrim(const string &);
vector<string> split(const string &);

/*
 * Complete the 'minimumPasses' function below.
 *
 * The function is expected to return a LONG_INTEGER.
 * The function accepts following parameters:
 *  1. LONG_INTEGER m
 *  2. LONG_INTEGER w
 *  3. LONG_INTEGER p
 *  4. LONG_INTEGER n
 */

long minimumPasses(long m, long w, long p, long n) {
    if (n <= 0) return 0;
    
    unsigned __int128 M = m;
    unsigned __int128 W = w;
    unsigned __int128 P = p;
    unsigned __int128 N = n;
    
    if (M >= N / W || M * W >= N) return 1;

    unsigned __int128 candies = 0;
    unsigned __int128 passes = 0;
    unsigned __int128 min_passes = -1; // Acts as max value

    while (true) {
        unsigned __int128 current_production = M * W;
        
        // Option 1: Calculate total passes if we decide to stop buying upgrades right now
        unsigned __int128 passes_to_finish = (N > candies) ? (N - candies + current_production - 1) / current_production : 0;
        if (passes + passes_to_finish < min_passes) {
            min_passes = passes + passes_to_finish;
        }

        // If we already have enough candies to reach N, we are done
        if (candies >= N) break;

        // If we don't even have enough candies to buy 1 upgrade, skip ahead to when we can
        if (candies < P) {
            unsigned __int128 remaining_to_p = P - candies;
            unsigned __int128 skip_passes = (remaining_to_p + current_production - 1) / current_production;
            
            // Critical check: What if we reach N before reaching P? 
            // The step above (passes_to_finish) handles this, so we can safely break if we would hit N first
            if (passes + skip_passes >= min_passes) {
                break;
            }
            
            passes += skip_passes;
            candies += skip_passes * current_production;
        }

        // Upgrade phase
        unsigned __int128 total_upgrades = candies / P;
        candies %= P;

        // Balance M and W to maximize future production
        unsigned __int128 diff = (M > W) ? (M - W) : (W - M);
        if (total_upgrades <= diff) {
            if (M < W) M += total_upgrades;
            else W += total_upgrades;
        } else {
            if (M < W) M += diff;
            else W += diff;
            
            total_upgrades -= diff;
            M += total_upgrades / 2;
            W += total_upgrades - (total_upgrades / 2);
        }

        // Run exactly 1 pass with our new production rate
        passes++;
        candies += M * W;
    }

    return (long)min_passes;
}

int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    string first_multiple_input_temp;
    getline(cin, first_multiple_input_temp);

    vector<string> first_multiple_input = split(rtrim(first_multiple_input_temp));

    long m = stol(first_multiple_input[0]);

    long w = stol(first_multiple_input[1]);

    long p = stol(first_multiple_input[2]);

    long n = stol(first_multiple_input[3]);

    long result = minimumPasses(m, w, p, n);

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