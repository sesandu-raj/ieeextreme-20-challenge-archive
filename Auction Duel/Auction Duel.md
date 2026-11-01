# Auction Duel

## Problem in one sentence

Write an interactive bidder for a repeated sealed-bid auction with a shared cash budget, private item values, unknown opponent values, and either first- or second-price payments.

## Model the game state

At every `ROUND` prompt, maintain:

- Current cash for both players (also supplied by the judge);
- Auction count remaining for each good;
- Your value for every good;
- Goods won by each player so far;
- The sequence of the opponent's observed bids;
- The pricing rule and the all-copies bonus.

Only the current bid is under your control. Bids must be integers between zero and your current cash, and each response must be flushed immediately.

## Value a marginal copy

The base value of a good is not necessarily the value of one copy in isolation. If owning all `M` copies triggers bonus `B`, winning a copy that completes the set has value

`M * value * (1 + B / 100)`,

whereas a partial set has only its ordinary value so far. A useful decision score for a bid is therefore the increase in final utility if this auction is won, adjusted for the chance of completing the set. Do not spend the same budget as if every remaining copy were guaranteed.

## Choose bids according to the payment rule

### First price

The winner pays their own bid. The useful tradeoff is winning probability versus payment: bid more when the good's marginal utility is high and less when budget pressure is high. A bid above the item's marginal value is usually wasteful unless it is part of a deliberate set-completion plan.

### Second price

The winner pays the other player's bid. Bidding close to the item's marginal value is a sound baseline: your bid primarily determines whether you win, while the opponent's bid determines the payment. The finite budget still matters because a winning bid cannot exceed cash and the observed opponent bids can inform future affordability.

## Allocate the finite budget

There are at most 60 rounds, so plan across remaining opportunities rather than treating rounds independently. One practical strategy is:

1. Estimate each good's remaining marginal value, including any plausible set bonus.
2. Estimate a target spend from the remaining cash and remaining high-value auctions.
3. Convert value and target spend into a bounded bid.
4. Update the opponent model after every `RESULT`, and avoid overreacting to a single bid.

Because opponent values are a permutation of the same distinct values, observed wins and bids provide evidence about their preferences, but not certainty. Track hypotheses rather than assuming an opponent's value from one observation.

## Interactive protocol

Read `GAME`, then `VALUES`. For each `ROUND`, print exactly `BID amount`, flush, and read the corresponding `RESULT`. Terminate after `GAME_END`. Extra diagnostics on standard output can invalidate the run; send them to standard error if permitted.

## Common mistakes

- Using a first-price strategy in a second-price game, or vice versa.
- Forgetting the bonus applies only after winning all copies of that good.
- Bidding more than current cash.
- Treating opponent bids as known before submitting your simultaneous bid.
- Failing to flush output and blocking the judge.

## Testing

Build a local simulator for small games. Test ties, zero bids, exhausting cash early, last-copy bonus completion, and both payment rules. Since a single strategy is not guaranteed optimal against every bot, compare average score over multiple opponent behaviors.
