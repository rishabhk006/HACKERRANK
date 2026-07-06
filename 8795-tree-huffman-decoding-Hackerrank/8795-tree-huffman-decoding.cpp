

/* 
The structure of the node is

typedef struct node {

	int freq;
    char data;
    node * left;
    node * right;
    
} node;

*/


void decode_huff(node * root, string s) {
    node *curr = root;
    for (char c : s) {
        if (c == '0') {
            curr = curr->left;
        } else {
            curr = curr->right;
        }
        
        if (!curr->left && !curr->right) {
            cout << curr->data;
            curr = root;
        }
    }
}



// Synced seamlessly with LeetHub Pro
// Pro features: https://bit.ly/leethubpro | Free version: https://bit.ly/leethubv4
// Get it here: https://chromewebstore.google.com/detail/bcilpkkbokcopmabingnndookdogmbna