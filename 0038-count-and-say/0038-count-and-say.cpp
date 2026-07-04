class Solution {
public:
    string countAndSay(int n) {
        string ans = "1";

        for (int i = 2; i <= n; i++) {
            string temp = "";

            for (int j = 0; j < ans.size(); ) {
                int cnt = 1;

                while (j + 1 < ans.size() && ans[j] == ans[j + 1]) {
                    cnt++;
                    j++;
                }

                temp += to_string(cnt);
                temp += ans[j];
                j++;
            }

            ans = temp;
        }

        return ans;
    }
};

// Synced seamlessly with LeetHub Pro
// Pro features: https://bit.ly/leethubpro | Free version: https://bit.ly/leethubv4
// Get it here: https://chromewebstore.google.com/detail/bcilpkkbokcopmabingnndookdogmbna