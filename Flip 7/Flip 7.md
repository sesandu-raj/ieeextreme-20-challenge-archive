# Flip 7

## Problem in one sentence

At each interactive turn, choose whether Mattia should draw one more card or stop, maximizing the expected score of this round only.

## Track the round precisely

For each card value `v` from 1 to 12, the full deck contains `v` copies. Maintain:

- The values Mattia has drawn in this round (for duplicate/bust detection);
- All cards revealed by every player this round (these are unavailable);
- The current round total for Mattia;
- The deck and discard composition, so a reshuffle can be handled correctly.

Cards on the table remain unavailable until the round ends. Cards from completed rounds are discarded and can return if the draw pile runs out.

## Expected value of drawing

Suppose Mattia currently has score `s` and there are `D` equally likely cards available in the active draw pile. For each possible next card:

- If its value has already appeared in Mattia's hand this round, it causes a bust and the round score becomes `0`.
- Otherwise the new score is `s + v`, and the challenge says Mattia evaluates drawing once and then stopping.

Thus:

`EV(draw) = sum(outcome score for each remaining physical card) / D`.

Stop gives exactly `s`. Draw if and only if `EV(draw) > s`; on equality, stop. The strict comparison is important.

For one-value counts, if `bad` is the number of remaining cards whose values Mattia already has and `sumGood` is the sum of values of all other remaining cards, then `EV(draw) = sumGood / D`. Bust cards contribute zero. This simplification makes each decision constant-time once the counts are maintained.

## Protocol and updating state

The judge sends `N P`, then events. On other players' draws, decrement that value from the available deck and mark it unavailable for the current round. On Mattia's draw, print `1`, flush, read the drawn value, and update score/duplicate tracking. On a stop prompt, print `0`.

When all players finish a round, clear current-round hand/score state and move tabled cards into the discard pool. If the draw pile empties, rebuild it from the discard pool exactly as the statement specifies. Terminate on `-1`.

## Common errors

- Counting only the cards Mattia drew when removing cards from availability; opponents' exposed cards are removed too.
- Treating duplicate cards belonging to another player as Mattia busts. Only a repeated value in Mattia's own current-round hand busts Mattia.
- Forgetting the reshuffle rule.
- Comparing expected value with `>=` instead of strict `>`.
- Failing to flush after every `0` or `1`.
