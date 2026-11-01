# Two Networks, One Emergency

## Problem in one sentence

Toggle cities active or inactive and, after each toggle, report the maximum over active pairs of the sum of their distances in two different weighted trees.

## Separate the metric from the dynamic set

For two cities `u, v`, the score is

`D(u,v) = distTree1(u,v) + distTree2(u,v)`.

This is a metric, but generally not the distance function of either input tree. A standard dynamic tree-diameter structure for one tree cannot simply be run on one network and reused for the other.

## Preprocess both trees

For each tree, root it and compute:

- `depth[v]`;
- weighted root distance `rootDist[v]`;
- binary-lifting ancestors for LCA.

Then

`dist(u,v) = rootDist[u] + rootDist[v] - 2*rootDist[lca(u,v)]`.

This gives an exact pair distance in `O(log n)` and allows a slow reference implementation for testing.

## Dynamic maximum-pair challenge

The active stations form a changing vertex subset, and the operation is a toggle. A correct solution needs to maintain the maximum edge weight in the implicit complete graph whose vertices are cities and whose edge weight is `D(u,v)`, without materializing its `Theta(n^2)` edges.

The intended optimization must exploit that each summand is a tree metric. A useful way to expose that structure is the cut representation: the distance in a weighted tree is the sum of weights of the tree edges whose cuts separate the two endpoints. Thus `D` is the sum of two families of weighted cut metrics. Any data structure used for insertion and deletion must preserve the maximum separated-cut score while combining both families.

## Correctness and engineering checklist

- A pair must use two **distinct** active cities.
- Fewer than two active cities gives `-1`.
- The edge weights are nonnegative, so all pair scores are nonnegative.
- City IDs may be toggled repeatedly; maintain an explicit active flag.
- The answer can be as large as the sum of two tree diameters, so use 64-bit arithmetic.

## Validation strategy

For small random trees, precompute all `D(u,v)`, simulate toggles, and scan all active pairs after every operation. Compare this oracle with the optimized structure. Include identical trees, disjoint shapes, zero-weight edges, repeated toggles, and active sets of size zero, one, and two.

> **Source limitation:** The screenshots provide the full query definition and bounds, but no construction hint for the specialized dynamic maximum-pair data structure. The metric reduction above is a correctness foundation, not a replacement for that nontrivial data structure; do not use an `O(n)` or `O(n^2)` per-toggle scan at the stated limits.
