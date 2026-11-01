# Clue Duel

## Problem in one sentence

Play an interactive deduction game: infer the one hidden person, weapon, and location from your hand, your guesses, and the cards the opponent reveals.

## Represent what is known

At the start, every card is in exactly one of three places: your hand, the opponent's hand, or the solution. Your own hand is known, so none of those cards can be part of the solution. For every remaining card, track whether it is still a possible solution card.

The three solution categories are independent in shape (one person, one weapon, one location), but observations can connect them. A guess followed by `RESULT NONE` means the opponent has none of the three guessed cards; all three guessed cards must be in the solution or your own hand. A revealed card is known to be in the opponent's hand, not the solution.

## Record every observation

For each of your guesses, store the triple and the response:

- `RESULT SHOWN c`: the opponent owns `c`; eliminate it from the solution.
- `RESULT NONE`: the opponent owns none of the triple's cards. If a guessed card is not in your hand, this places a useful constraint on the opponent's hand and the solution.

When the opponent guesses, respond legally: show one matching card that is in your hand, or `NONE` only if you hold none. If several cards match, choose consistently; a deliberate policy (for example, show the least informative card) prevents revealing more than necessary.

## Select guesses and accusations

Maintain candidate sets for each category and any cross-card constraints. Prefer guesses that:

1. Include cards that are still plausible solutions;
2. Test multiple unresolved possibilities;
3. Avoid repeating a triple whose result is already known.

Accuse only when the evidence identifies one candidate in each category, or when the risk/reward calculation justifies a guess. An incorrect accusation still scores points for you, but it hands the opponent a large advantage (they receive seven points), so an unsupported accusation is usually poor play.

## Protocol and state

The judge sends `TURN YOU` or `TURN OPP GUESS ...`. Print exactly one legal response and flush it. On your turn, choose `GUESS` or `ACCUSE`; when answering a guess, choose `SHOW card` or `NONE`. Exit on `GAME_END`.

The program is restarted for each game, so all game-local memory must be maintained during the process. Parse card codes as tokens (`P...`, `W...`, `L...`) and do not confuse categories.

## Correctness and testing

Use explicit sets/bitsets for card ownership and candidate status. Test all response branches, especially guesses containing two of your cards, repeated guesses, and a guess where the opponent can disprove with multiple cards. Never print `NONE` if even one guessed card is in your hand.
