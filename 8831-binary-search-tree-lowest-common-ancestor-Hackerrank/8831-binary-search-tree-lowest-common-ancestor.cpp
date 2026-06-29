

/*The tree node has data, left child and right child 
class Node {
    int data;
    Node* left;
    Node* right;
};

*/
  
    Node *lca(Node *root, int v1,int v2) {
		if (root == NULL) {
        return NULL;
    }
    
    // If both values are smaller than root, LCA is in the left subtree
    if (v1 < root->data && v2 < root->data) {
        return lca(root->left, v1, v2);
    }
    
    // If both values are larger than root, LCA is in the right subtree
    if (v1 > root->data && v2 > root->data) {
        return lca(root->right, v1, v2);
    }
    
    // If they split or one matches the current root, this node is the LCA
    return root;
    }



// Synced seamlessly with LeetHub Pro
// Pro features: https://bit.ly/leethubpro | Free version: https://bit.ly/leethubv4
// Get it here: https://chromewebstore.google.com/detail/bcilpkkbokcopmabingnndookdogmbna