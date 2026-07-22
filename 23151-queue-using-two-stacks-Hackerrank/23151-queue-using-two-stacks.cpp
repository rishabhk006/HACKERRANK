#include <iostream>
#include <stack>

using namespace std;

class QueueTwoStacks {
private:
    stack<int> s_in;  // Stack for enqueue operations
    stack<int> s_out; // Stack for dequeue and peek operations

    // Helper method to transfer elements from s_in to s_out when s_out is empty
    void transferIfNeeded() {
        if (s_out.empty()) {
            while (!s_in.empty()) {
                s_out.push(s_in.top());
                s_in.pop();
            }
        }
    }

public:
    // Query Type 1: Enqueue element x
    void enqueue(int x) {
        s_in.push(x);
    }

    // Query Type 2: Dequeue front element
    void dequeue() {
        transferIfNeeded();
        if (!s_out.empty()) {
            s_out.pop();
        }
    }

    // Query Type 3: Print front element
    int front() {
        transferIfNeeded();
        return s_out.top();
    }
};

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int q;
    if (!(cin >> q)) return 0;

    QueueTwoStacks myQueue;

    while (q--) {
        int type;
        cin >> type;

        if (type == 1) {
            int x;
            cin >> x;
            myQueue.enqueue(x);
        } else if (type == 2) {
            myQueue.dequeue();
        } else if (type == 3) {
            cout << myQueue.front() << "\n";
        }
    }

    return 0;
}


// Synced seamlessly with LeetHub Pro
// Pro features: https://bit.ly/leethubpro | Free version: https://bit.ly/leethubv4
// Get it here: https://chromewebstore.google.com/detail/bcilpkkbokcopmabingnndookdogmbna