# [Max Dot Product with 0 Insertions](https://www.geeksforgeeks.org/problems/maximize-dot-product2649/1)
## Medium
Given two arrays a[] and b[] of positive integers of size n and m respectively, where m ≤ n.&nbsp;&nbsp;You are allowed to insert zeros anywhere into the second array b so that its length becomes equal to n. 

The dot product of two arrays of equal length n is defined as:&nbsp;a[0]*b[0] + a[1]*b[1] + ... + a[n-1]*b[n-1]. 
Return the maximum possible dot product of the two arrays.

Examples :
Input: a[] = [2, 3, 1, 7, 8], b[] = [3, 6, 7]
Output: 107
Explanation: Maximum dot product is obtained after inserting 0 at the first and third positions in array b.Therefore b becomes [0, 3, 0, 6, 7]. 
Maximum dot product = 2*0 + 3*3 + 1*0 + 7*6 + 8*7 = 107. Therefore answer for this test case is 107.
Input: a[] = [1, 2, 3], b[] = [4] 
Output: 12 
Explanation: Maximum dot product is obtained after inserting 0 at the first and second positions in array b.Therefore b becomes [0, 0, 4]. 
Maximum Dot Product = 1*0 + 2*0 + 3*4 = 12. Therefore answer for this test case is 12.
Constraints:1 ≤ m ≤ n ≤ 1031 ≤ a[i], b[i] ≤ 103