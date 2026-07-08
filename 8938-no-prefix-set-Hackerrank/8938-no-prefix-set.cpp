#include <bits/stdc++.h>

using namespace std;

string ltrim(const string &);
string rtrim(const string &);

struct TrieNode {
    TrieNode* children[10];
    bool isEndOfWord;

    TrieNode() {
        isEndOfWord = false;
        for (int i = 0; i < 10; i++) {
            children[i] = nullptr;
        }
    }
};

void noPrefix(vector<string> words) {
    TrieNode* root = new TrieNode();

    for (const string& word : words) {
        TrieNode* curr = root;
        bool isNewNodeCreated = false;

        for (char ch : word) {
            int idx = ch - 'a';
            
            if (curr->children[idx] == nullptr) {
                curr->children[idx] = new TrieNode();
                isNewNodeCreated = true;
            }
            
            curr = curr->children[idx];

            if (curr->isEndOfWord) {
                cout << "BAD SET\n" << word << endl;
                return;
            }
        }

        if (!isNewNodeCreated) {
            cout << "BAD SET\n" << word << endl;
            return;
        }

        curr->isEndOfWord = true;
    }

    cout << "GOOD SET" << endl;
}

int main()
{
    string n_temp;
    getline(cin, n_temp);

    int n = stoi(ltrim(rtrim(n_temp)));

    vector<string> words(n);

    for (int i = 0; i < n; i++) {
        string words_item;
        getline(cin, words_item);

        words[i] = words_item;
    }

    noPrefix(words);

    return 0;
}

string ltrim(const string &str) {
    string s(str);

    s.erase(
        s.begin(),
        find_if(s.begin(), s.end(), [](unsigned char ch) { return !isspace(ch); })
    );

    return s;
}

string rtrim(const string &str) {
    string s(str);

    s.erase(
        find_if(s.rbegin(), s.rend(), [](unsigned char ch) { return !isspace(ch); }).base(),
        s.end()
    );

    return s;
}


// Synced seamlessly with LeetHub Pro
// Pro features: https://bit.ly/leethubpro | Free version: https://bit.ly/leethubv4
// Get it here: https://chromewebstore.google.com/detail/bcilpkkbokcopmabingnndookdogmbna