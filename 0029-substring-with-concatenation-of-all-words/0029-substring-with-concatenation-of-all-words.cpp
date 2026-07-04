class Solution {
public:
    vector<int> findSubstring(string s, vector<string>& words) {
        vector<int> ans;
        if (words.empty()) return ans;

        unordered_map<string, int> mp;
        for (string &w : words)
            mp[w]++;

        int wordLen = words[0].size();
        int totalWords = words.size();

        for (int i = 0; i < wordLen; i++) {
            unordered_map<string, int> seen;
            int left = i, count = 0;

            for (int j = i; j + wordLen <= s.size(); j += wordLen) {
                string word = s.substr(j, wordLen);

                if (mp.count(word)) {
                    seen[word]++;
                    count++;

                    while (seen[word] > mp[word]) {
                        string leftWord = s.substr(left, wordLen);
                        seen[leftWord]--;
                        left += wordLen;
                        count--;
                    }

                    if (count == totalWords) {
                        ans.push_back(left);

                        string leftWord = s.substr(left, wordLen);
                        seen[leftWord]--;
                        left += wordLen;
                        count--;
                    }
                } else {
                    seen.clear();
                    count = 0;
                    left = j + wordLen;
                }
            }
        }

        return ans;
    }
};

// Synced seamlessly with LeetHub Pro
// Pro features: https://bit.ly/leethubpro | Free version: https://bit.ly/leethubv4
// Get it here: https://chromewebstore.google.com/detail/bcilpkkbokcopmabingnndookdogmbna