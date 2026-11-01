# A Blueprint or a Schedule

## Problem in one sentence

Given a DAG of `n` laboratory modules and an oriented tree of `k` blueprint parts, construct either an orientation-preserving embedding of the tree into the DAG or a proper coloring of the DAG's underlying graph with `k - 1` colors.

## The useful dichotomy

The output is a certificate, not a decision about which option is preferable. A valid `TREE` certificate assigns every blueprint vertex to a distinct module and verifies every directed tree edge. A valid `COLOR` certificate gives different colors to the endpoints of every laboratory edge. Returning either accepted certificate is enough.

The guarantee that one certificate always exists is the central theorem behind the problem. It is what permits a constructive algorithm to search for an assembly and fall back to a schedule rather than solve both problems optimally.

## Schedule construction

When `k > 1`, a schedule has only `k - 1` colors. Process the DAG in topological order and color each module while checking the colors already used by its adjacent modules. The edge direction does not matter for this condition: every connection must have differently colored endpoints.

The challenge is that a greedy coloring can run out of colors. Therefore the constructive solution cannot treat ordinary greedy coloring as an unconditional algorithm. It must preserve the structures exposed during the greedy process (the vertices and edges that blocked the last available color); the dichotomy theorem uses those structures to construct the tree embedding.

## Assembly construction: what must be verified

For each blueprint part `a`, store its assigned module `p[a]`. A completed certificate must satisfy all of the following:

- Every `p[a]` is in `1..n`.
- No two blueprint parts use the same module.
- For each blueprint edge `a -> b`, the laboratory contains the directed edge `p[a] -> p[b]`.

Extra laboratory edges between chosen modules are allowed. The blueprint's input labels are arbitrary, so build its adjacency list and do not assume parts arrive in parent-before-child order.

## Practical solution structure

1. Read the laboratory DAG and the oriented blueprint tree.
2. Topologically order the DAG; this both confirms the acyclic structure and gives a safe order for any forward construction.
3. Run the problem's greedy certificate-building procedure. Track blocked color choices and the corresponding candidate modules rather than discarding that information.
4. If the procedure completes the embedding, print `TREE` and the mapping.
5. Otherwise, use the retained blocking structure to print `COLOR` and verify every edge has differently colored endpoints.
6. If `k = 1`, coloring is unavailable; the tree has one part, so map it to any module (`n >= 1`).

## Verification checklist

Before printing `TREE`, check injectivity and every directed blueprint edge against the laboratory adjacency structure. Before printing `COLOR`, scan all `m` connections and check unequal endpoint colors. These checks are linear and catch common direction and indexing mistakes.

## Complexity

The constraints (`n <= 20,000`, `m <= 1,000,000`, `k <= 200`) suggest a construction close to `O(n + m + k)` or `O((n + m) * k)` depending on the exact certificate procedure. Store the DAG sparsely; never build an `n x n` adjacency matrix.

> **Implementation caution:** The screenshots show the statement and guarantee, but not a full construction proof. A plain topological coloring alone is not enough to justify the fallback. Preserve and use the blocking structure required by the dichotomy proof.
