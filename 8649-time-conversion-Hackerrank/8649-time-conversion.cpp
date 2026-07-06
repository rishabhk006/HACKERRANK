#include <bits/stdc++.h>
#include <iomanip>
#include <sstream>

using namespace std;

/*
 * Complete the 'timeConversion' function below.
 *
 * The function is expected to return a STRING.
 * The function accepts STRING s as parameter.
 */

string timeConversion(string s) {
    int hour = stoi(s.substr(0, 2));
    string minutes_seconds = s.substr(2, 6);
    string am_pm = s.substr(8, 2);
    
    if (am_pm == "AM") {
        if (hour == 12) {
            hour = 0;
        }
    } else { 
        if (hour != 12) {
            hour += 12;
        }
    }
    
    stringstream ss;
    ss << setw(2) << setfill('0') << hour;
    return ss.str() + minutes_seconds;
}

int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    string s;
    getline(cin, s);

    string result = timeConversion(s);

    fout << result << "\n";

    fout.close();

    return 0;
}


// Synced seamlessly with LeetHub Pro
// Pro features: https://bit.ly/leethubpro | Free version: https://bit.ly/leethubv4
// Get it here: https://chromewebstore.google.com/detail/bcilpkkbokcopmabingnndookdogmbna