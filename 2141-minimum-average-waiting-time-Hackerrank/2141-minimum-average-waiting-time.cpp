#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;

struct Customer {
    long long arrival_time;
    long long cook_time;
};

struct CompareCookTime {
    bool operator()(const Customer& a, const Customer& b) {
        return a.cook_time > b.cook_time;
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    if (!(cin >> n)) return 0;

    vector<Customer> customers(n);
    for (int i = 0; i < n; i++) {
        cin >> customers[i].arrival_time >> customers[i].cook_time;
    }

    sort(customers.begin(), customers.end(), [](const Customer& a, const Customer& b) {
        return a.arrival_time < b.arrival_time;
    });

    priority_queue<Customer, vector<Customer>, CompareCookTime> pq;

    long long current_time = 0;
    long long total_waiting_time = 0;
    int index = 0;

    while (index < n || !pq.empty()) {
        if (pq.empty() && current_time < customers[index].arrival_time) {
            current_time = customers[index].arrival_time;
        }

        while (index < n && customers[index].arrival_time <= current_time) {
            pq.push(customers[index]);
            index++;
        }

        Customer current_customer = pq.top();
        pq.pop();

        current_time += current_customer.cook_time;
        total_waiting_time += (current_time - current_customer.arrival_time);
    }

    cout << total_waiting_time / n << "\n";

    return 0;
}


// Synced seamlessly with LeetHub Pro
// Pro features: https://bit.ly/leethubpro | Free version: https://bit.ly/leethubv4
// Get it here: https://chromewebstore.google.com/detail/bcilpkkbokcopmabingnndookdogmbna