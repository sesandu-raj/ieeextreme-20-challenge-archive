# Overclock in Night City

## Problem in one sentence

Find the smallest positive base limit `X` that lets David force all squads to zero against an opponent who doubles one squad after each nonterminal turn.

## Formalize one round

Let `k` be the number of David turns already taken, starting at zero. On his turn he selects one nonempty squad of size `z` and removes any `d` with

`1 <= d <= min(z, X + k)`.

If that empties every squad, David wins immediately. Otherwise Smasher selects any squad and doubles it. An empty squad stays empty. Both players choose optimally.

This is a minimax reachability game, not a simulation with a fixed greedy opponent. In particular, testing only “Smasher always doubles the squad David just hit” or “David always hits the largest squad” is not a proof: Smasher can redirect reinforcements, and David can choose a partial removal.

## Monotonicity and search

The answer is monotone in `X`: a David strategy available at a smaller `X` remains available at a larger one, because he can always remove the same number of soldiers. This suggests binary search once there is a correct decision procedure for a fixed `X`.

For a fixed `X`, the state is the multiset/vector of squad sizes and the turn index `k`. The decision order is:

1. David chooses a squad and a legal removal.
2. If all squads are zero, return winning.
3. Smasher chooses a squad to double.
4. Continue at `k + 1`.

The decision procedure must characterize whether David has a strategy that reaches the all-zero state in finite time despite possible infinite play. A bounded simulation is not sufficient: an arbitrarily large squad can be created by repeated doubling, and “not finished yet” does not prove that David will never finish.

## What makes the intended solution hard

The input can contain up to `10^6` squads, each initially up to `10^9`. A direct game-tree search over squad vectors is intractable, and explicit sizes can grow exponentially. The useful solution must compress the state into a structural condition on squad sizes and the increasing per-turn limit, then evaluate that condition after sorting/aggregating the initial sizes. Keep the universal quantifier over Smasher's choices in the derivation.

## Sanity checks from the examples

- One squad of size `5` needs `X = 2`, not `1`: with `X = 1`, even attacking maximally lets doubling outpace removal.
- For sizes `5, 1, 5`, the answer is `4`. With `X = 3`, Smasher can repeatedly preserve a growing squad; with `X = 4`, David can force termination.
- These examples are strong tests for off-by-one errors in `X + k` and for strategies that assume only one squad matters.

## Implementation notes

Use a wide integer type for expressions involving `X + k` and squad sizes. Do not construct a huge simulation horizon. Before implementing, derive the compressed fixed-`X` criterion from the minimax game and prove both directions: the criterion must give David a forcing strategy when true and a Smasher survival strategy when false.

> **Source limitation:** The screenshots include the complete rules and examples but no solution hint. This guide records the minimax structure and the necessary monotonicity reduction; it intentionally does not invent an unverified closed-form test.
