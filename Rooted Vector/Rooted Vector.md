# Rooted Vector

## Problem in one sentence

Choose an ancestor-closed set containing the root, sum its two-dimensional vertex vectors, and maximize the squared Euclidean length of the resulting sum.

## Feasible sets form a recursive family

For vertex `u`, let `F[u]` be the set of all sum-vectors obtainable by selecting a valid subset of `u`'s subtree that contains `u`. If `v[u] = (a[u], b[u])`, then

`F[u] = v[u] + sum over children v of ( { (0,0) } union F[v] )`.

For each child, either select nothing from that child subtree, or select a valid rooted subset starting at the child. Choices in separate child subtrees are independent, which makes the combination a Minkowski sum.

## Convex hull dynamic programming

The objective `A^2 + B^2` is convex. A convex function reaches its maximum over a convex polygon at a vertex. Therefore it is enough to maintain the convex hull of each feasible vector set; interior feasible points cannot improve the maximum.

Process the tree bottom-up:

1. Start the polygon for `u` at the single point `v[u]`.
2. For each child polygon `P`, form `conv({(0,0)} union P)` to represent “skip this child” or “take a rooted subset below it.”
3. Minkowski-sum the current polygon with that child polygon.
4. After the root polygon is built, evaluate `A^2 + B^2` at each hull vertex and take the maximum.

Convex hulls of point sets and Minkowski sums of convex polygons are standard computational-geometry operations. Keep vertices in counterclockwise order; a polygon Minkowski sum can be computed by merging the two lists of edge vectors.

## Why discarding interior points is safe

The set of attainable sums may be non-convex, but its convex hull has the same maximum for any convex objective: every attainable point lies inside the hull, and a convex function's maximum on a compact polygon is attained at a boundary extreme point. The hull vertices used by the DP represent sums of valid choices at each combination step, so the final maximum is achievable by a valid subset.

## Complexity and numeric range

With at most `5,000` vertices, a straightforward bottom-up polygon implementation can use up to quadratic total work in a worst-shaped tree; avoid repeatedly rebuilding hulls from every descendant point. Minkowski sums are linear in the polygon sizes and are preferable to enumerating subsets.

Each coordinate sum can reach about `5 * 10^9`, and its square may exceed signed 64-bit range. Use 128-bit or arbitrary-precision integers when evaluating `A*A + B*B`.

## Edge cases

- The root is mandatory even if its vector points away from the origin.
- A child may be skipped entirely; descendants cannot be selected without that child.
- Negative coordinates are valid and can make excluding a subtree beneficial.
- Parent labels are not topological; build the tree and process a traversal in reverse order.
