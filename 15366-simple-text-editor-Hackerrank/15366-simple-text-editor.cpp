#include <iostream>
#include <string>
#include <vector>
#include <stack>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int q;
    if (!(cin >> q)) return 0;

    string s = "";
    stack<string> history;

    while (q--) {
        int type;
        cin >> type;

        if (type == 1) {
            string w;
            cin >> w;
            history.push(s);
            s += w;
        } else if (type == 2) {
            int k;
            cin >> k;
            history.push(s);
            s.erase(s.length() - k);
        } else if (type == 3) {
            int k;
            cin >> k;
            cout << s[k - 1] << "\n";
        } else if (type == 4) {
            if (!history.empty()) {
                s = history.top();
                history.pop();
            }
        }
    }

    return 0;
}


// Synced seamlessly with LeetHub Pro
// Pro features: https://bit.ly/leethubpro | Free version: https://bit.ly/leethubv4
// Get it here: https://chromewebstore.google.com/detail/bcilpkkbokcopmabingnndookdogmbna