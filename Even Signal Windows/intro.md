A sensor records a binary signal as a string 
S
S. A window is a nonempty contiguous substring. A window is stable if it contains an even number of 1 characters.


For each query 
[
l
,
r
]
[l,r], count the stable windows contained entirely in the substring 
S
[
l
…
r
]
S[l…r]. Queries are independent.


Standard input

The first line contains two integers 
N
N and 
Q
Q.

The second line contains a binary string 
S
S of length 
N
N.

Each of the next 
Q
Q lines contains two integers 
l
l and 
r
r. Positions are 1-indexed.


Standard output

For each query, print one integer: the number of stable windows contained in 
S
[
l
…
r
]
S[l…r].


Constraints and notes

1
≤
N
,
Q
≤
200000
1≤N,Q≤200000.
1
≤
l
≤
r
≤
N
1≤l≤r≤N.
A window containing zero 1 characters is stable.

Input
5 3
10110
1 5
2 4
3 3

output
7
3
0

input
4 3
0000
1 4
2 3
4 4

output
10
3
1