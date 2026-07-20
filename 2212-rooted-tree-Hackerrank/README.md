# [Rooted Tree](https://www.hackerrank.com/challenges/rooted-tree/problem?isFullScreen=true)
## Hard
<div class="challenge-body-html"><div class="challenge_problem_statement"><div class="msB challenge_problem_statement_body"><div class="hackdown-content"><svg style="display: none;"><defs id="MathJax_SVG_glyphs"></defs></svg><p>You are given a rooted <a href="http://en.wikipedia.org/wiki/Tree_(graph_theory)">tree</a> with <em>N</em> nodes and the root of the tree, <em>R</em>, is also given. Each node of the tree contains a value, that is initially empty. You have to mantain the tree under two operations:</p>

<ol>
<li>Update Operation</li>
<li>Report Operation</li>
</ol>

<p><strong>Update Operation</strong> <br>
Each Update Operation begins with the character <code>U</code>. Character <code>U</code> is followed by 3 integers <em>T, V and K</em>. For every node which is the descendent of the node <em>T</em>, update it's value by adding <em>V + d*K</em>, where <em>V</em> and <em>K</em> are the parameters of the query and <em>d</em> is the distance of the node from <em>T</em>. Note that <em>V</em> is added to node <em>T</em>.  </p>

<p><strong>Report Operation</strong> <br>
Each Report Operation begins with the character <code>Q</code>. Character <code>Q</code> is followed by 2 integers, <em>A</em> and <em>B</em>. Output the sum of values of nodes in the path from <em>A</em> to <em>B</em> modulo <em>(10<sup>9</sup> + 7)</em>  </p>

<p><strong>Input Format</strong> <br>
The first Line consists of 3 space separated integers, <em>N E R</em>, where <em>N</em> is the number of nodes present, <em>E</em> is the total number of queries (update + report), and <em>R</em> is root of the tree.  </p>

<p>Each of the next <em>N-1</em> lines contains 2 space separated integers, <em>X</em> and <em>Y</em> (<em>X</em> and <em>Y</em> are connected by an edge).</p>

<p>Thereafter, <em>E</em> lines follows: each line can represent either the Update Operation or the Report Operation.<br></p>

<ul>
<li><em>Update Operation</em> is of the form : <em>U T V K</em>.</li>
<li><em>Report Operation</em> is of the form : <em>Q A B</em>.</li>
</ul>

<p><strong>Output Format</strong> <br>
Output the answer for every given report operation.</p>

<p><strong>Constraints</strong>  </p>

<p>1 ≤ N, E ≤ 10<sup>5</sup> <br>
1 ≤ E ≤ 10<sup>5</sup> <br>
1 ≤ R, X, Y, T, A, B ≤ N <br>
1 ≤ V, K ≤ 10<sup>9</sup> <br>
X ≠ Y</p>

<p><strong>Sample Input</strong></p>

<pre><code>7 7 1
1 2
2 3
2 4
2 5
5 6
6 7
U 5 10 2
U 4 5 3
Q 1 7
U 6 7 4
Q 2 7
Q 1 4
Q 2 4
</code></pre>

<p><strong>Sample Output</strong></p>

<pre><code>36
54
5
5
</code></pre>

<p><strong>Explanation</strong>  </p>

<ul>
<li>Values of Nodes after <code>U 5 10 2</code>: <code>[0 0 0 0 10 12 14]</code>.</li>
<li>Values of Nodes after <code>U 4 5 3</code>: <code>[0 0 0 5 10 12 14]</code>. </li>
<li>Sum of the Nodes from 1 to 7: 0 + 0 + 10 + 12 + 14 = 36.</li>
<li>Values of Nodes after <code>U 6 7 4</code>: [0 0 0 5 10 19 25].  </li>
<li>Sum of the Nodes from 2 to 7: 0 + 10 + 19 + 25 = 54. </li>
<li>Sum of the Nodes from 1 to 4: 0 + 0 + 5 = 5.</li>
<li>Sum of the Nodes from 2 to 4: 0 + 5 = 5.</li>
</ul></div></div></div></div>