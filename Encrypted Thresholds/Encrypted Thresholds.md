# Encrypted Thresholds

## Problem in one sentence

For every threshold `k`, count unordered pairs of distinct tree vertices whose XOR of edge labels on the unique path is less than `k`.

## Turn path XOR into vertex labels

Root the tree anywhere and define `x[v]` as the XOR of edge labels from the root to `v`. XOR is its own inverse, so for any two vertices:

`pathXor(u, v) = x[u] XOR x[v]`.

Compute all `x[v]` with one DFS/BFS. The tree geometry then disappears: the task is to count pairwise XOR values in a multiset of at most `n` integers.

## Build the frequency of every XOR value

All labels are below `2^20`, so each `x[v]` also fits in 20 bits. Let `freq[z]` be the number of vertices with root-XOR value `z`. The XOR convolution

`conv[t] = sum_z freq[z] * freq[z XOR t]`

counts **ordered** pairs whose XOR is `t`. Compute the convolution with the Walsh-Hadamard transform (FWHT):

1. Apply the XOR FWHT to `freq`.
2. Square every transformed coefficient.
3. Apply the inverse XOR FWHT.

The resulting `conv[t]` is exact. Convert it to unordered pairs of distinct vertices:

- For `t != 0`, `pairs[t] = conv[t] / 2`.
- For `t == 0`, remove the `n` self-pairs first: `pairs[0] = (conv[0] - n) / 2`.

All arithmetic can be done in a sufficiently wide signed integer type because the total number of ordered pairs is at most `n^2`.

## Answer thresholds

Build a prefix sum over `pairs`. A query `k` asks for the sum of frequencies at values `0 <= t < k`, so it is a prefix query ending at `k - 1`. If `k = 0`, the answer is zero.

## Correctness outline

The root-to-vertex XOR prefixes cancel on the shared portion of two root paths, leaving exactly the path between the vertices. FWHT computes the XOR convolution of the frequency array. Removing self-pairs and dividing by two converts ordered pairs into the required unordered distinct pairs. A prefix sum then applies the strict inequality `< k`.

## Complexity

Let `B = 20`. Tree traversal costs `O(n)`, FWHT costs `O(B * 2^B)`, and each query costs `O(1)` after the prefix sum. Memory is `O(2^B + n)`.

## Common pitfalls

- The threshold is strict: include XOR values through `k - 1`, not `k`.
- XOR value zero can occur for two distinct vertices with equal root-prefix XOR; do not discard all zero-XOR pairs.
- The diagonal self-pairs are the only ordered pairs to subtract before dividing the zero bucket by two.
- Use the full 20-bit domain for the transform.
