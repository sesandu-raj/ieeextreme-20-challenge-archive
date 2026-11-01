# Deliveries in the Kingdom

## Problem in one sentence

For each query `(source, destination, forbidden)`, decide whether the two endpoints remain connected after deleting the forbidden vertex from an undirected graph.

## Why a normal path search is too slow

A BFS or DFS per delivery is correct, but up to `10^5` queries on a graph with `N + M <= 10^5` makes that approach quadratic in the worst case. The graph is static, so preprocess its articulation structure once and answer each deletion query from that structure.

## Block-cut forest

Run Tarjan's algorithm to find articulation points and biconnected components. Build the block-cut forest:

- One node for each original articulation vertex;
- One node for each biconnected component;
- Connect an articulation node to every component containing it.

Non-articulation vertices belong to one component node. The block-cut representation captures whether removing an articulation separates two vertices: they are disconnected exactly when their routes pass through different branches of that articulation node.

Root every tree in the forest and preprocess depths, parents, and binary-lifting ancestors. This supports locating the first branch from a forbidden articulation `x` toward a queried vertex `v`.

## Answering a query

1. If `source == forbidden` or `destination == forbidden`, answer `Nao`: Pedro may not even start or finish at the blocked city.
2. If `source == destination` and that city is not forbidden, answer `Sim`; no rail needs to be traversed.
3. If `forbidden` is not an articulation point, its removal cannot disconnect the other vertices; answer `Sim`.
4. Otherwise, determine which connected component of the block-cut tree after removing node `forbidden` contains each endpoint. If the branch identifiers differ, answer `Nao`; if they match, answer `Sim`.

To find a branch, use the rooted tree:

- If the endpoint is in a child subtree of `forbidden`, the first child on the path identifies that branch.
- Otherwise the endpoint lies on the parent side, which is one distinct branch.

Binary lifting finds the child directly below `forbidden` in `O(log N)`.

## Complexity

- Tarjan decomposition and forest construction: `O(N + M)`.
- Binary-lifting preprocessing: `O(N log N)`.
- Each query: `O(log N)`.

## Implementation cautions

- The graph may be disconnected, so process every DFS component and build a forest, not just one tree.
- A forbidden endpoint is disallowed even if source equals destination.
- Use `Sim` and `Nao` exactly, with the capitalization shown.
- Handle isolated vertices and non-articulation forbidden vertices.
