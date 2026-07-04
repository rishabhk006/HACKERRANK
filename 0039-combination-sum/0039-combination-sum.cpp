class Solution {
public:
    vector<vector<int>> ans;
    vector<int> temp;

    void solve(vector<int>& candidates, int target, int idx) {
        if (target == 0) {
            ans.push_back(temp);
            return;
        }

        if (target < 0)
            return;

        for (int i = idx; i < candidates.size(); i++) {
            temp.push_back(candidates[i]);
            solve(candidates, target - candidates[i], i);
            temp.pop_back();
        }
    }

    vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
        solve(candidates, target, 0);
        return ans;
    }
};

// Synced seamlessly with LeetHub Pro
// Pro features: https://bit.ly/leethubpro | Free version: https://bit.ly/leethubv4
// Get it here: https://chromewebstore.google.com/detail/bcilpkkbokcopmabingnndookdogmbna