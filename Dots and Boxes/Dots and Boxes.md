# Dots and Boxes

## Problem in one sentence

Play an interactive Dots and Boxes game from a partially drawn board, taking boxes whenever possible and avoiding moves that give the opponent easy captures.

## Parse the board

The board has `R x C` boxes, represented by `(R + 1) x (C + 1)` dots. The input drawing is a character grid of size `(2R + 1) x (2C + 1)`:

- Even rows contain dots and horizontal edges;
- Odd rows contain vertical edges and box states;
- `.` is an undrawn edge, `-` and `|` are drawn edges;
- `o` is an incomplete box and `#` is already complete and out of play.

Represent every possible edge once, and for each box store its four incident edges or its current side count. This avoids brittle character-index logic when examining moves.

## Move evaluation

For each legal undrawn edge:

1. Find the one or two adjacent boxes.
2. Count how many sides each currently has.
3. If drawing the edge completes a box (a neighboring box currently has three sides), it is a capturing move. Take it immediately; the rules grant another turn.
4. If no capture is available, prefer a move that does not raise an untouched box from two sides to three. A three-sided box can be captured by the next player.
5. If every safe move is gone, choose a move that minimizes the opponent's capture chain or preserves control of the remaining chains.

The immediate tactic is easy; endgame control is the harder part. A common strategy is to recognize chains of boxes with two unclaimed sides, avoid opening them prematurely, and when forced, choose whether to give a short chain away or sacrifice a few boxes to retain the remaining long chains.

## State and tactical search

For small boards (`R, C <= 10`), compactly encode the drawn edges and use memoized search over endgame states. The score is the difference in boxes captured. A move that captures a box leaves the same player to move; a non-capturing move passes the turn. Full-board search can still be large, so use tactical rules for early play and reserve exact search for states with few undrawn edges.

When counting boxes, count newly completed boxes after the move, not merely adjacent boxes: a single edge can complete two boxes.

## Interactive protocol

On `YOUR_TURN`, print `LINE r1 c1 r2 c2` for two adjacent dots whose edge is still open, then flush. Apply every `OPPONENT` line to the same board representation. Do not output messages outside a turn, and exit after `GAME_END`.

## Testing checklist

- Boundary edges have one adjacent box; interior edges have two.
- An edge can complete two boxes.
- A capture gives the same player another turn.
- Initially completed boxes do not count as boxes in play.
- Coordinates are zero-based, and the upper/left endpoint is used in the judge messages.
