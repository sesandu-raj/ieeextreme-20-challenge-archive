# Shroud

## Problem in one sentence

Build an interactive agent for a simultaneous-movement territory game on a symmetric grid with limited visibility and multiple units.

## Parse and maintain a persistent game state

The process receives a `GAME` header, optionally the full map, and then one `ROUND` observation at a time. For every unit, the observation includes its location and a square view of radius `V`. Maintain:

- The best-known wall/open status for every cell;
- Your current unit positions and opponent sightings;
- Known paint ownership and the round/cell count;
- Cells that have not yet been observed.

The executable restarts for every game, but continues across rounds within that game, so store state in memory until `GAME_END`.

## Strategic objective

The score depends on the difference in painted open cells, not merely on reaching a target. A practical agent balances:

- **Expansion:** move toward open, unclaimed cells;
- **Defense:** reclaim valuable cells painted by the opponent;
- **Exploration:** reveal unknown territory when no known high-value move is available;
- **Coordination:** keep units from wasting turns targeting the same cell or bottleneck.

Choose destinations first, then route each unit toward its destination using shortest paths over known-open cells. If the map is unknown, treat unseen cells as traversable with an uncertainty penalty and revise the route as new observations arrive.

## Simultaneous movement and collision rules

Moves are simultaneous. Two units trying to enter the same cell hold; two units swapping cells hold; a unit entering a cell whose occupant holds also holds. Therefore independently computed shortest paths can conflict.

Before printing orders:

1. Compute one proposed next cell per unit.
2. Detect duplicate destinations and swaps.
3. Resolve conflicts by changing a lower-priority unit to a safe alternative or hold.
4. Recheck dependencies on units that are now holding.

The opponent's current units are also obstacles for predicting immediate collisions, although uncertain future actions should not be treated as known.

## Protocol and reliability

For each `ROUND`, output exactly `ORDERS d0 ... d(U-1)`, one move in `N/E/S/W/H` per unit, and flush. Invalid output, crashing, or missing a deadline forfeits the game. Do not emit debug text on standard output.

## Testing

Test known and unknown maps, map boundaries, collisions, swaps, units entering blocked cells, simultaneous paint overwrites, and the final round. A conservative valid agent with consistent parsing is better than a sophisticated agent that occasionally breaks protocol.
