#include <iostream>
#include <vector>
#include <set>

using namespace std;

int main() {
    int q;
    if (!(cin >> q)) return 0;
    
    set<int> heapElements;
    
    while (q--) {
        int type;
        cin >> type;
        
        if (type == 1) {
            int x;
            cin >> x;
            heapElements.insert(x);
        } else if (type == 2) {
            int x;
            cin >> x;
            heapElements.erase(x);
        } else if (type == 3) {
            if (!heapElements.empty()) {
                cout << *heapElements.begin() << "\n";
            }
        }
    }
    
    return 0;
}


// Synced seamlessly with LeetHub Pro
// Pro features: https://bit.ly/leethubpro | Free version: https://bit.ly/leethubv4
// Get it here: https://chromewebstore.google.com/detail/bcilpkkbokcopmabingnndookdogmbna