# [Almost Equal - Advanced](https://www.hackerrank.com/challenges/almost-equal-advanced/problem?isFullScreen=true)
## Expert
<div class="challenge-body-html"><div class="challenge_problem_statement"><div class="msB challenge_problem_statement_body"><div class="hackdown-content"><svg style="display: none;"><defs id="MathJax_SVG_glyphs"></defs></svg><p>A Sumo wrestling championship is scheduled to be held this winter in the <em>HackerCity</em> where <em>N</em> wrestlers from different parts of the world are going to participate. The rules state that two wrestlers can fight against each other if and only if the difference in their height is less than or equal to K, <br>
(i.e) wrestler A and wrestler B can fight if and only if <em>|height(A)-height(B)|&lt;=K</em>.  </p>

<p></p>

<p>Given an array <em>H[]</em>, where <em>H[i]</em> represents the height of the <em>i<sup>th</sup></em> fighter, for a given l, r where <code>0 &lt;= l &lt;= r &lt; N</code>, can you count the number of pairs of fighters between l and r (both inclusive) who qualify to play a game?</p>

<p><strong>Input Format</strong> <br>
The first line contains an integer <em>N</em> and <em>K</em>  separated by a single space representing the number of Sumo wrestlers who are going to participate and the height difference K. <br>
The second line contains <em>N</em> integers separated by a single space, representing their heights <em>H[0] H[1] ... H[N - 1]</em>. <br>
The third line contains <em>Q</em>, the number of queries. This is followed by <em>Q</em> lines each having two integers <em>l</em> and <em>r</em> separated by a space.  </p>

<p><strong>Output Format</strong> <br>
For each query Q, output the corresponding value of the number of pairs of fighters for whom the absolute difference of height is not greater that <em>K</em>.  </p>

<p><strong>Constraints</strong> <br>
1 &lt;= N &lt;= 100000 <br>
0 &lt;= K &lt;= 10<sup>9</sup> <br>
0 &lt;= H[i] &lt;= 10<sup>9</sup> <br>
1 &lt;= Q &lt;= 100000 <br>
0 &lt;= l &lt;= r &lt; N  </p>

<p><strong>Sample Input</strong></p>

<pre>5 2
1 3 4 3 0
3
0 1
1 3
0 4
</pre>

<p><strong>Sample Output</strong>  </p>

<pre>1
3
6
</pre>

<p><strong>Explanation</strong> <br>
Query #0: Between 0 and 1 we have i,j as (0,1) and |H[0]-H[1]|=2 therefore output is 1. <br>
Query #1: The pairs (H[1],H[2]) (H[1],H[3]) and (H[2],H[3]) are the pairs such that |H[i]-H[j]| &lt;=2. Hence output is 3. <br>
Query #2: Apart from those in Query #1, we have (H[0],H[1]), (H[0], H[3]), (H[0], H[4]), hence 6.  </p>

<p><strong>Timelimits</strong></p>

<p>Timelimits are given <a href="https://hr-assets.s3.amazonaws.com/7bb46cae_challenge_assets/checker_limits/1361/limits.json">here</a></p></div></div></div></div>