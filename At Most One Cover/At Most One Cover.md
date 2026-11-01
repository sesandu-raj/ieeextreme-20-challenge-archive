# At Most One Cover

## Problem in one sentence

There are `m` cells and independent random intervals. For every cell, compute the probability that zero or one of the intervals covering it actually exists, modulo `998244353`.

## Key observation

For a fixed cell, only the intervals containing that cell matter. Let their existence probabilities be `p1, p2, ..., pt`. The desired event consists of:

1. None of the intervals exists; or
2. Exactly one interval exists.

If `qj = 1 - pj` is the probability that interval `j` is absent, then

`answer = product(qj) + sum_j [pj * product_{i != j}(qi)]`.

Computing this independently for every cell is too slow. The set of covering intervals changes only at interval endpoints, so sweep from left to right and maintain the product expression as intervals enter and leave.

## Maintaining the probability

For each active interval, store its modular existence probability `p` and absence probability `q = 1 - p`.

When all active `q` values are nonzero modulo the prime, maintain:

- `P = product(q)`;
- `S = sum(p / q)`.

Then the answer is `P * (1 + S)`. Adding an interval multiplies `P` by `q` and adds `p/q` to `S`; removing one divides `P` by `q` and subtracts `p/q` from `S`. Division means multiplication by a modular inverse.

There is an important edge case: `q = 0` exactly when an interval exists with probability one. Such an interval has no modular inverse. Keep a separate count `forced` of active probability-one intervals, and maintain `P` and `S` only for the other intervals:

- `forced >= 2`: answer is `0`;
- `forced == 1`: the forced interval is the only one allowed to exist, so answer is `P` (all non-forced intervals must be absent);
- `forced == 0`: answer is `P * (1 + S)`.

Probability-zero intervals can be ignored, since they never exist.

## Sweep-line algorithm

For an input interval `[l, r]`, add it to the active set at cell `l` and remove it after cell `r`. A convenient event array stores an add event at `l` and a remove event at `r + 1`.

For each cell `i` from `1` to `m`:

1. Apply all events at `i`.
2. Evaluate the maintained formula and output the result for cell `i`.

Precompute modular inverses of the required nonzero `q` values, or compute them using Fermat's little theorem. As the prime is `998244353`, `q^(MOD-2)` is the inverse of `q` when `q` is nonzero.

## Correctness intuition

Independence lets the probability of any exact configuration factor into per-interval probabilities. The event “at most one exists” partitions into the disjoint cases “none exists” and “interval `j` alone exists.” The formula sums precisely those cases. The sweep maintains the same formula for exactly the intervals covering the current cell.

## Complexity and implementation notes

- Time: `O(n + m)` with constant-time modular updates per event.
- Memory: `O(n + m)` for events and output.
- Use 64-bit intermediates for modular multiplication.
- Process removals at `r + 1`, not at `r`, because both interval endpoints are inclusive.
- Handle certainty (`p = q` in the input) separately; blindly inverting `1 - p` is incorrect.
