# Clockwise Checkpoints

## Problem in one sentence

On a circular track of circumference `C`, find the largest clockwise distance between consecutive checkpoints, breaking ties by the smaller starting position.

## Key observation

Once checkpoint positions are sorted, consecutive checkpoints in that order are exactly the clockwise neighbors. Only one gap needs wraparound treatment: from the last position back to the first, the distance is `C - last + first`.

## Algorithm

1. Read the `n` distinct positions and sort them increasingly.
2. For each `i < n - 1`, calculate `gap = p[i + 1] - p[i]`. Its starting checkpoint is `p[i]`.
3. Calculate the wraparound gap `C - p[n - 1] + p[0]`, starting at `p[n - 1]`.
4. Keep the candidate with the greatest gap; on equal lengths, keep the smaller starting position.
5. Print the length and start.

If there is one checkpoint, the only segment is a full revolution of length `C`, starting at that checkpoint.

## Why it works

Sorting lists checkpoints in clockwise order from position zero. There is no checkpoint strictly between two adjacent entries, so their difference is precisely one segment. The wraparound expression accounts for the portion from the final checkpoint to `C`, then from zero to the first checkpoint. Checking all `n` such gaps examines every possible segment exactly once.

## Complexity

- Time: `O(n log n)` for sorting.
- Extra memory: `O(n)` (or `O(1)` beyond the position array).

## Edge cases

- `n = 1`: answer is `C`.
- A checkpoint at zero: the wraparound formula still works.
- Equal maximum gaps: compare the starting coordinates explicitly; do not rely on iteration order.
