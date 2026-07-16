# [Starfleet](https://www.hackerrank.com/challenges/starfleet/problem?isFullScreen=true)
## Advanced
<div class="challenge-body-html"><div class="challenge_problem_statement"><div class="msB challenge_problem_statement_body"><div class="hackdown-content"><svg style="display: none;"><defs id="MathJax_SVG_glyphs"></defs></svg><p>In a galaxy far away, there is a constant battle between the republic and the droid army. The droid army decided to launch their final attack on the republic. They have <strong>N</strong> space-fighters. </p>

<p>Initially the <strong>i</strong><sup>th</sup> fighter is located at (x<sub>i</sub>, y<sub>i</sub>). All of the space-fighters move with constant velocity <strong>V</strong> units/sec in the positive <strong>X</strong> direction. 
i.e., fighter at (x<sub>i</sub>, y<sub>i</sub>) moves to (x<sub>i</sub>+V, y<sub>i</sub>) in 1 second. 
The <strong>i</strong><sup>th</sup> space-fighter broadcasts enemy information at a frequency <strong>f<sub>i</sub></strong>.</p>

<p>The republic is not scared of the artificially intelligent droid force as they have <em>Yoda</em>. <em>Yoda</em> has a special power, at any time <strong>T</strong> he can choose a region of the droid army and block one specific frequency <strong>F</strong>. This power has one constraint; it can be applied only in the form of a two sided unbounded axis parallel rectangular box open towards the both the directions across <strong>X</strong> axis (refer image below for clarity). If a frequency (<strong>F</strong>) is blocked all the space-fighters in the region having the frequency <strong>F</strong> can’t communicate.</p>

<p><img src="https://s3.amazonaws.com/hr-assets/0/1526568097-af9b3d37ed-starfleet.png" alt="starfleet" title=""></p>

<p>Given the initial positions of the space-fighters, and their velocity, you are to answer queries of the following form:</p>

<p><strong>YU YD T</strong></p>

<p>where <strong>YU</strong>, <strong>YD</strong> are the bounds on y-axis inside which YODA can block a frequency at time <strong>T</strong>. 
In the region described by the query, after a time <strong>T</strong> seconds from the start, if <em>Yoda</em> can chose one frequency (<strong>F</strong>) he wishes to, what is the maximum number of communications he can block?</p>

<p><strong>Input Format</strong> <br>
Each test case is described as follows; the first line contains <strong>3</strong> space separated integers <strong>N</strong> - the number of space-fighters, <strong>Q</strong> - the number of queries you have to answer, and <strong>V</strong> - the velocity of the space-fighters separated by a single space. </p>

<p><strong>N</strong> lines follow, each containing <strong>3</strong> space separated integers <strong>x<sub>i</sub></strong>, <strong>y<sub>i</sub></strong>, and <strong>f<sub>i</sub></strong>, denoting the x co-ordinate, y co-ordinate and the frequency at which the <strong>i</strong><sup>th</sup> ship broadcasts respectively. Each of the next <strong>Q</strong> lines contain <strong>3</strong> space separated integers representing <strong>YU, YD, T</strong> respectively. Refer the figure for more clarity</p>

<p>Note: Points on the boundaries should be counted as well.</p>

<p><strong>Output Format</strong> <br>
For each query you are to output a single integer denoting the result.</p>

<p><strong>Constraints</strong> <br>
1 &lt;= N &lt;= 50000<br>
1 &lt;= Q &lt;= 30000<br>
1 &lt;= V &lt;= 10000<br>
-10<sup>9</sup> &lt;= x<sub>i</sub> &lt;= 10<sup>9</sup><br>
-10<sup>9</sup> &lt;= y<sub>i</sub> &lt;= 10<sup>9</sup><br>
1 &lt;= f<sub>i</sub> &lt;= 10<sup>9</sup><br>
-10<sup>9</sup> &lt;= YU &lt;= 10<sup>9</sup><br>
-10<sup>9</sup> &lt;= YD &lt;= 10<sup>9</sup><br>
1 &lt;= T &lt;= 10000<br>
YU &gt;= YD<br></p>

<p><strong>Sample Input</strong></p>

<pre><code>5 5 82
-4 1 4
-3 -2 2
-3 5 1
0 -5 2
1 -1 2
1 -1 57
-2 -5 11
5 -5 40
-1 -5 16
5 -1 93
</code></pre>

<p><strong>Sample Output</strong></p>

<pre><code>1
2
3
3
1
</code></pre>

<p><strong>Explanation</strong>
Consider the points ships in the Y-range 1 to -1, they are the (-4, 1) and (1, -1), and both operate on different frequencies, hence the most times a frequency is repeated is once.</p></div></div></div></div>