# One Junction Box

## Problem in one sentence

Count pairs consisting of one up/right path from the bottom-left to the top-right and one down/right path from the top-left to the bottom-right that share exactly one grid tile.

## Why direct enumeration fails

The number of monotone paths across an `N x M` grid is a binomial coefficient, so enumerating paths or path pairs is impossible. A cell-by-cell dynamic program over both paths would also take `O(NM)`, while either dimension can be as large as `10^6`.

## Combinatorial viewpoint

Every valid pair has a unique common tile, the junction. Count pairs by this tile:

1. Choose a tile `(r, c)` that both paths visit.
2. Count the choices for their portions before the junction that do not meet anywhere else.
3. Count the choices for their portions after the junction that do not meet anywhere else.
4. Multiply prefix and suffix counts, then sum over all possible junctions.

The “do not meet anywhere else” condition is the essential part. Counting arbitrary prefixes and suffixes would overcount pairs that share multiple tiles.

Monotone paths on a rectangular grid admit closed-form path counts using binomial coefficients. For the disjoint-prefix and disjoint-suffix counts, use the lattice-path reflection principle / non-intersecting-path determinant: intersecting pairs are put in bijection with pairs whose endpoints are swapped. The resulting subcounts reduce to products or differences of binomial coefficients, indexed by the junction's distances from the four corners.

## Efficient evaluation

Precompute factorials and inverse factorials modulo `998244353` up to `N + M` so that each binomial coefficient is available in `O(1)`. Then evaluate the fixed-junction contribution using the reflection-principle formula, sum contributions modulo the prime, and use symmetry between rows and columns to iterate over the smaller dimension when the formula permits it.

The implementation must avoid an `N x M` table and any loop over all cells: both dimensions can be huge. The intended solution is a one-dimensional combinatorial sum with precomputed binomial coefficients.

## Correctness outline

Each pair counted by the algorithm has exactly one common tile by construction: the two prefixes are disjoint except at the selected junction, and the two suffixes are disjoint except there. Conversely, every valid pair has one unique common tile and therefore appears in exactly one summand. Prefix and suffix choices are independent once the junction is fixed, so their counts multiply.

## Boundary cases

- If `N = 1` or `M = 1`, paths may be forced along the same row/column for multiple tiles; these cases are useful checks against an incorrectly applied interior formula.
- Include all four corner positions when considering possible junctions.
- Use modulo arithmetic for every product and sum.
- The screenshots provide the statement and samples, but not the reflection-principle derivation. Derive and verify the exact disjoint-path subcount before coding; the high-level decomposition alone is not a substitute for that formula.
