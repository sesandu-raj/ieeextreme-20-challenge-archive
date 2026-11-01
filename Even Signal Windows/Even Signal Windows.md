# Even Signal Windows

## Problem in one sentence

For each substring query `[l, r]` of a binary string, count its nonempty contiguous windows containing an even number of `1`s.

## Prefix parity turns windows into equal-value pairs

Define `P[0] = 0`, and for `i > 0`, let `P[i]` be the parity of the number of `1`s in the first `i` characters:

`P[i] = P[i - 1] XOR (S[i] == '1')`.

A window `S[a..b]` contains an even number of ones exactly when `P[a - 1] == P[b]`. Therefore every even window corresponds to a pair of equal prefix parities at indices `i < j`.

For a query `[l, r]`, the relevant prefix indices are exactly `l - 1, l, ..., r`. If this range contains `E` even parity values and `O` odd parity values, then:

`answer = E * (E - 1) / 2 + O * (O - 1) / 2`.

Each pair is counted once; its two indices define the window from the earlier index plus one through the later index.

## Preprocess for constant-time queries

Build two prefix-count arrays over parity positions `0..N`, for example:

- `evenCount[i]`: number of even `P[j]` among indices `0..i`;
- `oddCount[i]`: number of odd `P[j]` among indices `0..i`.

For a query, subtract the counts before index `l - 1` from the counts through index `r` to get `E` and `O`. Be consistent about inclusive indices; a half-open prefix-count convention is often easier to implement.

## Correctness outline

The number of ones in `S[a..b]` is the XOR of `P[a-1]` and `P[b]`, so it is even exactly when the two values match. The query contains all possible endpoint prefix indices from `l-1` through `r`. Choosing any two equal parity values in that range produces exactly one nonempty window fully inside the query, and every valid window produces one such pair.

## Complexity and numeric type

- Preprocessing: `O(N)` time and `O(N)` memory.
- Each query: `O(1)`.
- Total: `O(N + Q)`.

The largest answer is `N(N+1)/2`, so use a 64-bit integer for the pair products.

## Pitfalls

- Include `P[l - 1]`; omitting it loses windows that start at `l`.
- Include `P[r]`; it represents windows ending at `r`.
- Equal parity can be even-even **or** odd-odd.
- A query of length one containing `1` has answer zero; a query containing `0` has answer one.
