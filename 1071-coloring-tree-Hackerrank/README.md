# [Coloring Tree](https://www.hackerrank.com/challenges/coloring-tree/problem?isFullScreen=true)
## Hard
<div class="challenge-body-html"><div class="challenge_problem_statement"><div class="msB challenge_problem_statement_body"><div class="hackdown-content"><svg style="display: none;"><defs id="MathJax_SVG_glyphs"></defs></svg><p>You are given a tree with <strong>N</strong> nodes with every node being colored. A color is represented by an integer ranging from 1 to 10<sup>9</sup>. Can you find the number of distinct colors available in a subtree rooted at the node <strong>s</strong>? </p>

<p><strong>Input Format</strong> <br>
The first line contains three space separated integers representing the number of nodes in the tree (<strong>N</strong>), number of queries to answer (<strong>M</strong>) and the root of the tree. </p>

<p>In each of the next N-1 lines, there are two space separated integers(a b) representing an edge from node a to Node b and vice-versa.   </p>

<p>N lines follow: N+i<sup>th</sup> line contains the color of the i<sup>th</sup> node.</p>

<p>M lines follow: Each line containg a single integer s.</p>

<p><strong>Output Format</strong> <br>
Output exactly M lines, each line containing the output of the i<sub>th</sub> query.</p>

<p><strong>Constraints</strong> <br>
0 &lt;= M &lt;= 10<sup>5</sup><br>
1 &lt;= N &lt;= 10<sup>5</sup><br>
1 &lt;= root &lt;= N<br>
1 &lt;= color of the Node &lt;= 10<sup>9</sup><br></p>

<p><strong>Example</strong></p>

<p><strong>Sample Input</strong></p>

<pre><code>4 2 1
1 2
2 4
2 3
10
20
20
30
1
2
</code></pre>

<p><strong>Sample Output</strong></p>

<pre><code>3
2
</code></pre>

<p><strong>Explanation</strong></p>

<p>Query 1-Subtree rooted at 1 is the entire tree and colors used are 10 20 20 30 , so the answer is 3(10,20 and 30)</p>

<p>Query 2-Subtree rooted at 2 contains color 20 20 30, so the answer is 2(20 and 30)</p></div></div></div></div>