# IEEEXtreme 20.0 Challenge Archive

An organized, post-contest archive of challenge statements, screenshots, solution notes, and C examples from **IEEEXtreme 20.0**.

## About IEEE and IEEEXtreme

**IEEE** (the Institute of Electrical and Electronics Engineers) is a global professional organization for technology and engineering. **IEEEXtreme** is its international, 24-hour programming competition, where student teams solve algorithmic and programming challenges released during the event.

This repository collects materials from the 20th edition, **IEEEXtreme 20.0**. It is an independent learning archive; it is not an official IEEE or IEEEXtreme repository.

## What this repository contains

Each challenge has its own directory, usually containing:

- The original problem statement as one or more screenshots;
- A challenge-named Markdown guide with the problem summary, key concepts, algorithm discussion, complexity, and pitfalls;
- `solution.c` when a C implementation is currently available.

Some challenge folders also contain original notes or other assets. The repeated **Typo Bowl** folder is retained because both copies were present in the source materials. The **Welcome to IEEEXtreme 20.0** folder contains the introductory task and example programs in C and Python.

## Challenge index

| Challenge | Main concepts | C example |
|---|---|---|
| [A Blueprint or a Schedule](A%20Blueprint%20or%20a%20Schedule/) | DAGs, oriented trees, constructive certificates | Not included |
| [At Most One Cover](At%20Most%20One%20Cover/) | Independent probabilities, modular arithmetic, sweep line | [`solution.c`](At%20Most%20One%20Cover/solution.c) |
| [Auction Duel](Auction%20Duel/) | Interactive programming, bidding, expected value | [`solution.c`](Auction%20Duel/solution.c) |
| [Clockwise Checkpoints](Clockwise%20Checkpoints/) | Sorting, circular gaps, tie-breaking | [`solution.c`](Clockwise%20Checkpoints/solution.c) |
| [Clue Duel](Clue%20Duel/) | Interactive programming, constraint tracking, deduction | [`solution.c`](Clue%20Duel/solution.c) |
| [Deliveries in the Kingdom](Deliveries%20in%20the%20Kingdom/) | Articulation points, block-cut forests, LCA | [`solution.c`](Deliveries%20in%20the%20Kingdom/solution.c) |
| [Dots and Boxes](Dots%20and%20Boxes/) | Interactive programming, grid state, game strategy | [`solution.c`](Dots%20and%20Boxes/solution.c) |
| [Encrypted Thresholds](Encrypted%20Thresholds/) | Tree path XOR, FWHT, prefix counts | [`solution.c`](Encrypted%20Thresholds/solution.c) |
| [Even Signal Windows](Even%20Signal%20Windows/) | Prefix parity, range frequency counts | [`solution.c`](Even%20Signal%20Windows/solution.c) |
| [Flip 7](Flip%207/) | Interactive programming, expected value, card counting | [`solution.c`](Flip%207/solution.c) |
| [IEEE Global Expert Selection Challenge](IEEE%20Global%20Expert%20Selection%20Challenge/) | CSV-style parsing, filtering, multi-key ranking | [`solution.c`](IEEE%20Global%20Expert%20Selection%20Challenge/solution.c) |
| [Lunar Giant](Lunar%20Giant/) | Simulation of a supplied transition table | Blocked: table not supplied |
| [Monotone Tree](Monotone%20Tree/) | Convex optimization on trees, rational values | [`solution.c`](Monotone%20Tree/solution.c) |
| [One Junction Box](One%20Junction%20Box/) | Monotone paths, non-intersecting paths, combinatorics | Not included |
| [Overclock in Night City](Overclock%20in%20Night%20City/) | Minimax game, reachability, monotone search | Not included |
| [Rooted Vector](Rooted%20Vector/) | Ancestor-closed sets, convex hulls, Minkowski sums | [`solution.c`](Rooted%20Vector/solution.c) |
| [Shroud](Shroud/) | Interactive programming, partial visibility, multi-agent movement | [`solution.c`](Shroud/solution.c) |
| [Two Networks, One Emergency](Two%20Networks,%20One%20Emergency/) | Tree metrics, dynamic maximum pair distance | Not included |
| [Typo Bowl](Typo%20Bowl/) | Levenshtein edit distance, line-based input | [`solution.c`](Typo%20Bowl/solution.c) |
| [Welcome to IEEEXtreme 20.0](Welcome%20to%20IEEEXtreme%2020.0/) | Simple output task | [`welcome.c`](Welcome%20to%20IEEEXtreme%2020.0/welcome.c), `welcome.py` |

The directory [`Typo  Bowl`](Typo%20%20Bowl/) is a second copy of the Typo Bowl materials and also includes a C source file.

## Using the C examples

The standalone examples target C11. Compile an individual source file with a C compiler such as GCC or Clang:

```sh
cc -std=c11 -O2 -Wall -Wextra "Even Signal Windows/solution.c" -o even-windows
./even-windows < input.txt
```

Replace the path with the challenge's source file. Interactive entries (Auction Duel, Clue Duel, Dots and Boxes, Flip 7, and Shroud) expect the contest judge's line-based protocol; ordinary redirected sample input is not a substitute for an interactive judge.

## Completeness and known limitations

The C files are examples and are not an official answer set. The interactive programs are baseline agents: they are intended to follow the protocol and provide playable behavior, but they are not claimed to be optimal against every opponent.

No C solution is included for:

- **A Blueprint or a Schedule**
- **Lunar Giant**
- **One Junction Box**
- **Overclock in Night City**
- **Two Networks, One Emergency**

For A Blueprint or a Schedule, One Junction Box, Overclock in Night City, and Two Networks, One Emergency, the available local materials do not include enough verified derivation to present a complete optimized implementation confidently. The Lunar Giant statement refers to an attached transition table that is absent from the source folder. `Lunar Giant/transitions.tsv` is only a placeholder; it does not contain the actual table.

The **Typo Bowl** sample is inconsistent with its stated rule: standard case-sensitive Levenshtein distance makes “New York Giants” closer to “The Weeknd” than “Minnesota Vikings,” while the screenshot's sample output selects Minnesota. The provided C example follows the stated edit-distance rule.

## Contest rules and responsible use

The archived screenshots include the original AI-use/editor labels. Those labels applied during the contest and are preserved as part of the source material. Follow the official competition rules when using any archived statement or solution; do not treat this post-contest repository as permission to use AI or external code in a future restricted contest.

## Repository layout

```text
.
├── Challenge Name/
│   ├── Screenshot*.png
│   ├── Challenge Name.md
│   └── solution.c                 # when currently available
├── Welcome to IEEEXtreme 20.0/
│   ├── Welcome.png
│   ├── Welcome.md
│   ├── welcome.c
│   └── welcome.py
└── README.md
```

## Contributions and corrections

Corrections to OCR, statement interpretation, sample inconsistencies, and solution explanations are welcome. When contributing an implementation, include the algorithm rationale, complexity, relevant edge cases, and a way to verify the result. Keep interactive protocol requirements distinct from ordinary stdin/stdout problems.
