# [The crazy helix](https://www.hackerrank.com/challenges/helix/problem?isFullScreen=true)
## Advanced
<div class="challenge-body-html"><div class="challenge_problem_statement"><div class="msB challenge_problem_statement_body"><div class="hackdown-content"><svg style="display: none;"><defs id="MathJax_SVG_glyphs"></defs></svg><p>Natural numbers from 1 to N have been placed in an increasing order over some helix ( a circular structure ). When the helix starts rotating, it is easy to find out </p>

<ol>
<li>The position of a given number</li>
<li>The number located at a given position. </li>
</ol>

<p>The helix has numbers arranged in the following fashion:  </p>

<pre><code>[1, 2, 3, ..., N]
</code></pre>

<p>Due to some malfunction, the helix has started rotating in a weird manner. Right now, every possible contiguous interval can be rotated, and hence, locating a particular number or identifying the number at a given position is almost impossible. For example, if at some particular instant, the integer list is like this:</p>

<pre><code>[1, 2, 3, 4, 5, ..., N]
</code></pre>

<p>rotating the interval [5...N] will leave the list like this:</p>

<pre><code>[1, 2, 3, 4, N, N - 1, N - 2, ..., 5]
</code></pre>

<p>We need a software to handle this. Can you help us?</p>

<p><strong>Input Format</strong> <br>
The first line of the input consists of two space separated integers, <strong>N, Q</strong>. <em>N</em> signifies that initially our list contains numbers from 1 to N, placed in an increasing order. <em>Q</em> lines follow and contain input in one of the following formats:  </p>

<pre><code>1 A B
</code></pre>

<p>indicating that the helix rotated circularly in the interval [A..B] ( both inclusive);</p>

<pre><code>2 A
</code></pre>

<p>indicating that we are interested in knowing the current position of the number A  </p>

<pre><code>3 A
</code></pre>

<p>indicating that we are interested in knowing the number at position A.  </p>

<p><strong>Output Format</strong> <br>
For each line in the input of the form <code>2 A</code>  </p>

<p>output a line saying </p>

<pre><code>element A is at position x
</code></pre>

<p>where <em>A</em> is the number we are interested in and <em>x</em> is its current position.</p>

<p>For each line of the form <code>3 A</code></p>

<p>output a line saying </p>

<pre><code>element at position A is x
</code></pre>

<p>where <em>A</em> is the position we are interested in and <em>x</em> is the integer located at this position. </p>

<p><strong>Constraints</strong>  </p>

<p>1 ≤ <em>N, Q</em> ≤ 10<sup>5</sup> <br>
positions are 1-indexed.   </p>

<p><strong>Sample Input</strong></p>

<pre><code>5 10
1 1 3
2 3
3 3
1 3 5
1 2 4
3 1
3 5
2 4
1 5 5
2 2
</code></pre>

<p><strong>Sample Output</strong></p>

<pre><code>element 3 is at position 1
element at position 3 is 1
element at position 1 is 3
element at position 5 is 1
element 4 is at position 2
element 2 is at position 4
</code></pre>

<p><strong>Explanation</strong></p>

<p>Initially elements are placed like this:</p>

<pre><code>[1, 2, 3, 4, 5]
</code></pre>

<p>after the first rotation, they are placed like this:</p>

<pre><code>[3, 2, 1, 4, 5]
</code></pre>

<p>and that's how we get the first 2 results (first 2 lines in the output). After second rotation, they are placed like this:</p>

<pre><code>[3, 2, 5, 4, 1]
</code></pre>

<p>and third one does this:</p>

<pre><code>[3, 4, 5, 2, 1]
</code></pre>

<p>In the last rotation (1 5 5), it's easy to see that output matches the initial positions of the elements. Last rotation doesn't change the positions of the elements.</p></div></div></div></div>