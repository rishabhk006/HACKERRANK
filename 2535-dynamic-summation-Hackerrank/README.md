# [Dynamic Summation](https://www.hackerrank.com/challenges/dynamic-summation/problem?isFullScreen=true)
## Hard
<div class="challenge-body-html"><div class="challenge_problem_statement"><div class="msB challenge_problem_statement_body"><div class="hackdown-content"><svg style="display: none;"><defs id="MathJax_SVG_glyphs"></defs></svg><p>Given a tree of <em>N</em> nodes, where each node is uniquely numbered in between <em>[1, N]</em>. Each node also has a value which is initially 0. You need to perform following two operations in the tree.  </p>

<ol>
<li>Update Operation</li>
<li>Report Operation</li>
</ol>

<p><strong>Update Operation</strong></p>

<pre><code>U r t a b
</code></pre>

<p>Adds <code>a<sup>b</sup> + (a+1)<sup>b</sup> + (b+1)<sup>a</sup></code> to all nodes in the subtree rooted at <code>t</code>, considering that tree is rooted at <code>r</code> (see explanation for more details).  </p>

<p><strong>Report Operation</strong></p>

<pre><code>R r t m
</code></pre>

<p>Output the sum of all nodes in the subtree rooted at <code>t</code>, considering that tree is rooted at <code>r</code>. Output the sum modulo <code>m</code> (see explanation for more details).</p>

<p><strong>Input Format</strong></p>

<p>First line contains <em>N</em>, number of nodes in the tree. <br>
Next <em>N-1</em> lines contain two space separated integers <em>x</em> and <em>y</em> which denote that there is an edge between node <em>x</em> and node <em>y</em>. <br>
Next line contains <em>Q</em>, number of queries to follow. <br>
Next <em>Q</em> lines follow, each line will be either a report operation or an update operation.  </p>

<p><strong>Output Format</strong></p>

<p>For each report query output the answer in a separate line.</p>

<p><strong>Constraints</strong></p>

<p>1 ≤ <em>N</em> ≤ 100000 <br>
1 ≤ <em>Q</em> ≤ 100000 <br>
1 ≤ <em>m</em> ≤ 101 <br>
1 ≤ <em>r, t, x, y</em> ≤ <em>N</em> <br>
<em>x</em> ≠ <em>y</em> <br>
1 ≤ <em>a, b</em> ≤ 10<sup>18</sup>  </p>

<p><strong>Notes</strong></p>

<ol>
<li>There will be at most one edge between a pair of nodes.</li>
<li>There will be no loop.</li>
<li>Tree will be completely connected. </li>
</ol>

<p><strong>Sample Input</strong></p>

<pre><code>4
1 2
2 3
3 4
4
U 3 2 2 2
U 2 3 2 2
R 1 2 8
R 4 3 9
</code></pre>

<p><strong>Sample Output</strong></p>

<pre><code>2
3
</code></pre>

<p><strong>Explanation</strong></p>

<p>Initially Values in each node : [0,0,0,0] <br>
The first query is <code>U 3 2 2 2</code>. Here, tree is rooted at 3. It looks like </p>

<pre><code>    3(0)
   / \
  /   \
 2(0)  4(0)
 |
 |
 1(0)
</code></pre>

<p>For the sub tree rooted at 2 ( nodes 2 and 1 ), we add a<sup>b</sup>  + (a+1)<sup>b</sup> + (b+1)<sup>a</sup> = 2<sup>2</sup> + 3<sup>2</sup> + 3<sup>2</sup> = 22. After first update operation, nodes 1, 2, 3, and 4 will have values 22, 22, 0 and 0 respectively.  </p>

<pre><code>    3(0)
   / \
  /   \
 2(22) 4(0)
 |
 |
 1(22)
</code></pre>

<p>The second query is <code>U 2 3 2 2</code>. Here, tree is rooted at 2. It looks like</p>

<pre><code>    2(22)
   / \
  /   \
 1(22) 3(0)
       |
       |
       4(0)
</code></pre>

<p>For the sub tree rooted at 3 (nodes 3 and 4), we add a<sup>b</sup> + (a+1)<sup>b</sup> + (b+1)<sup>a</sup> = 2<sup>2</sup> + 3<sup>2</sup> + 3<sup>2</sup> = 22. After second update operation, nodes 1, 2, 3, and 4 each have values 22,22,22,22 respectively.  </p>

<pre><code>    2(22)
   / \
  /   \
 1(22) 3(22)
       |
       |
       4(22)
</code></pre>

<p>The first report query is <code>R 1 2 8</code> asks for the sum modulo 8 of the subtree rooted at 2, when the tree is rooted at 1. The tree looks like </p>

<pre><code>1(22)
 \
  \
   2*(22)
   |
   |
   3*(22)
   |
   |
   4*(22)
</code></pre>

<p>The sum of the values of nodes 2, 3 and 4 are </p>

<pre><code>(22 + 22 + 22) % 8 = 2
</code></pre>

<p>The second report query is <code>R 4 3 9</code> asks for the sum modulo 9 of the subtree rooted at 3 when the tree is rooted at 4. The tree looks like </p>

<pre><code>4(22)
 \
  \
   3*(22)
   |
   |
   2*(22)
   |
   |
   1*(22)
</code></pre>

<p>The sum of the values of nodes 3, 2 and 1 are </p>

<pre><code>(22 + 22 + 22) % 9 = 3
</code></pre>

<p><sub><strong>Time Limits:</strong> <br>
C, C++: 4s | Java and other JVM based languages: 10s | Python, Python3 = 45s | Other interpreted Language: 30s | C#, Haskell: 10s | Rest: 3 times of <a href="https://www.hackerrank.com/environment">default</a>. <br>
</sub></p></div></div></div></div>