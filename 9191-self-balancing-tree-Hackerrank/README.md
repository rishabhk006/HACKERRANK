# [Self Balancing Tree](https://www.hackerrank.com/challenges/self-balancing-tree/problem?isFullScreen=true)
## Medium
<div class="challenge-body-html"><div class="challenge_problem_statement"><div class="msB challenge_problem_statement_body"><div class="hackdown-content"><svg style="display: none;"><defs id="MathJax_SVG_glyphs"></defs></svg><p>An AVL tree (Georgy Adelson-Velsky and Landis' tree, named after the inventors) is a self-balancing binary search tree.  In an AVL tree, the heights of the two child subtrees of any node differ by at most one; if at any time they differ by more than one, rebalancing is done to restore this property.</p>

<p>We define balance factor for each node as :</p>

<pre><code>balanceFactor = height(left subtree) - height(right subtree)
</code></pre>

<p>The balance factor of any node of an AVL tree is in the integer range [-1,+1]. If after any modification in the tree, the balance factor becomes less than −1 or greater than +1, the subtree rooted at this node is unbalanced, and a rotation is needed.</p>

<p><img src="https://s3.amazonaws.com/hr-challenge-images/0/1436854305-b167cc766c-AVL_Tree_Rebalancing.svg.png" title="AVL_Tree_Rebalancing.svg.png"></p>

<p>(<a href="https://en.wikipedia.org/wiki/AVL_tree">https://en.wikipedia.org/wiki/AVL_tree</a>)        </p>

<p>You are given a pointer to the root of an AVL tree. You need to insert a value into this tree and perform the necessary rotations to ensure that it remains balanced. </p>

<p><strong>Input Format</strong>  </p>

<p>You are given a function,</p>

<pre><code>node *insert(node * root,int new_val)
{


}
</code></pre>

<p>'node' is defined as :</p>

<pre><code>struct node
{
int val;            //value
struct node* left;  //left child
struct node* right; //right child
int ht;             //height of the node
} node;
</code></pre>

<p>You only need to complete the function.     </p>

<p>Note: All the values in the tree will be distinct. Height of a Null node is -1 and the height of the leaf node is 0.</p>

<p><br>
<strong>Output Format</strong>  </p>

<p>Insert the new value into the tree and return a pointer to the root of the tree. Ensure that the tree remains balanced.</p>

<p><strong>Sample Input</strong>  </p>

<pre><code>    3
  /  \
 2    4
       \
        5
</code></pre>

<p>The value to be inserted is 6.</p>

<p><strong>Sample Output</strong></p>

<pre><code>    3
  /  \
 2    5
     / \
    4   6
</code></pre>

<p><strong>Explanation</strong></p>

<p>After inserting 6 in the tree. the tree becomes:</p>

<pre><code>    3 (Balance Factor = -2)
  /  \
 2    4 (Balance Factor = -2)
       \
        5 (Balance Factor = -1)
         \
          6 (Balance Factor = 0)
</code></pre>

<p>Balance Factor of nodes 3 and 4 is no longer in the range [-1,1]. We need to perform a rotation to balance the tree. This is the right right case. We perform a single rotation to balance the tree.</p>

<p>After performing the rotation, the tree becomes : </p>

<pre><code>                              3 (Balance Factor = -1)
                            /   \
      (Balance Factor = 0) 2     5 (Balance Factor = 0)
                                / \
           (Balance Factor = 0)4   6 (Balance Factor = 0)
</code></pre></div></div></div></div>