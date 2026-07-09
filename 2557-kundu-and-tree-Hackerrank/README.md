# [Kundu and Tree](https://www.hackerrank.com/challenges/kundu-and-tree/problem?isFullScreen=true)
## Hard
<div class="challenge-body-html"><div class="challenge_problem_statement"><div class="msB challenge_problem_statement_body"><div class="hackdown-content"><svg style="display: none;"><defs id="MathJax_SVG_glyphs"></defs></svg><p>Kundu is true tree lover. Tree is a connected graph having <em>N</em> vertices and <em>N-1</em>  edges. Today when he got a tree, he colored each edge with one of either red(<code>r</code>) or black(<code>b</code>) color. He is interested in knowing how many triplets(a,b,c) of vertices are there , such that, there is atleast one edge having red color on all the three paths i.e. from vertex <em>a</em> to <em>b</em>, vertex <em>b</em> to <em>c</em> and vertex <em>c</em> to <em>a</em> . Note that (a,b,c), (b,a,c) and all such permutations will be considered as the same triplet. </p>

<p>If the answer is greater than 10<sup>9</sup> + 7, print the answer modulo (%) 10<sup>9</sup> + 7.</p>

<p><strong>Input Format</strong> <br>
The first line contains an integer <em>N</em>, i.e., the number of vertices in tree. <br>
The next <em>N-1</em> lines represent edges:  2 space separated integers denoting an edge followed by a color of the edge. A color of an edge is denoted by a small letter of English alphabet, and it can be either red(<code>r</code>) or black(<code>b</code>).  </p>

<p><strong>Output Format</strong> <br>
Print a single number i.e. the number of triplets.  </p>

<p><strong>Constraints</strong> <br>
1 ≤ <em>N</em> ≤ 10<sup>5</sup><br> 
A node is numbered between 1 to <em>N</em>.  </p>

<p><strong>Sample Input</strong>  </p>

<pre><code>5
1 2 b
2 3 r
3 4 r
4 5 b
</code></pre>

<p><strong>Sample Output</strong>  </p>

<pre><code>4
</code></pre>

<p><strong>Explanation</strong></p>

<p>Given tree is something like this.<br>
<img src="https://s3.amazonaws.com/hr-assets/0/1526563539-7ce683027b-kundu-and-trees.jpg" alt="image" title=""></p>

<p>(2,3,4) is one such triplet because on all paths i.e 2 to 3, 3 to 4 and 2 to 4 there is atleast one edge having red color.<br>
(2,3,5), (1,3,4) and (1,3,5) are other such triplets. <br>
Note that (1,2,3) is NOT a triplet, because the path from 1 to 2 does not have an edge with red color.</p></div></div></div></div>