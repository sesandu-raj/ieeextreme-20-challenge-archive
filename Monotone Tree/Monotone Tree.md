# Monotone Tree

## Problem in one sentence

Choose rational values `x[v]` minimizing the weighted squared error from observations `a[v]`, subject to every parent value being at most each child value.

## Conditional-cost dynamic programming

For a vertex `u`, let `F_u(t)` be the minimum cost inside the subtree of `u` when `x[u]` is fixed to `t`. If `v` is a child, its value must satisfy `x[v] >= t`, so:

`F_u(t) = w[u] * (t - a[u])^2 + sum_v min_{y >= t} F_v(y)`.

Every `F_u` is a convex, piecewise-quadratic function. Let `m[u]` be its unique unconstrained minimizer. For a child, minimizing its convex function subject to `y >= t` gives:

`argmin_{y >= t} F_v(y) = max(t, m[v])`.

This gives a simple reconstruction rule once all subtree minimizers are known:

`x[root] = m[root]`, and `x[v] = max(x[parent], m[v])`.

## Computing each minimizer

The derivative of `F_u` is monotone and piecewise linear. The node's own contribution is `2 * w[u] * (t - a[u])`. A child's contribution is flat while `t <= m[v]` (the child can remain at its own optimum); after `t` passes `m[v]`, the child is forced upward and contributes the derivative of `F_v(t)`.

Process vertices bottom-up. Represent each subtree derivative as an ordered set of linear pieces / breakpoints:

1. Start with the node's own quadratic derivative.
2. Meld in each child's profile, activating it at the child's minimizing breakpoint `m[v]`.
3. Find the unique point where the resulting derivative crosses zero; that is `m[u]`.
4. Retain the merged profile for the parent.

A meldable, small-to-large breakpoint structure avoids constructing the full subtree function repeatedly. Exact rational arithmetic is needed: the derivative's zero is a ratio of integer weighted sums, and the output requires reduced numerator and positive denominator.

## Reconstruction and fractions

After the root minimizer is found, traverse from the root and clamp each child's unconstrained minimizer upward to its parent's assigned value. Reduce each rational by `gcd(abs(numerator), denominator)`. Represent zero as `0 1`.

Use wide integer arithmetic for weighted sums and cross-products. The statement's bounds on the reduced numerator and denominator are large enough that ordinary 32-bit arithmetic is not safe.

## Why the approach is correct

Conditioning on `x[u]` separates the child subtrees. Each child independently chooses the lowest-cost value no smaller than its parent. A convex function is minimized under a lower bound either at its unconstrained minimizer or at the bound, whichever is larger. Thus the recurrence captures every feasible assignment; minimizing its convex profile gives the unique optimal value for the subtree. The top-down clamping reconstructs the global optimum.

## Complexity

The intended implementation processes each node and melds piecewise-linear profiles with a small-to-large or meldable structure, targeting roughly `O(n log n)` time and `O(n)` memory. A naive representation that copies every descendant term at every ancestor can degrade to `O(n^2)` on a chain.

## Pitfalls

- The inequality is parent `<=` child, not the reverse.
- A child's raw observation is not necessarily its final value; clamp its optimum to the parent's value.
- Do not use floating point for final output; fractions must be exact and reduced.
- Parent labels are not topological, so build child lists and obtain a traversal order.
