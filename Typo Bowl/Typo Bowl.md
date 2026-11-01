# Typo Bowl

## Problem in one sentence

For each conference, choose the team name with minimum edit distance from the corresponding performance band's name.

## Edit distance

The allowed operations are inserting, deleting, or substituting one character, each with cost one. This is Levenshtein distance.

For strings `a` of length `L` and `b` of length `R`, define `dp[i][j]` as the minimum cost to transform the first `i` characters of `a` into the first `j` characters of `b`:

- `dp[0][j] = j`;
- `dp[i][0] = i`;
- `dp[i][j] = min(dp[i-1][j] + 1, dp[i][j-1] + 1, dp[i-1][j-1] + cost)`,

where `cost = 0` if `a[i-1] == b[j-1]`, otherwise `1`.

Spaces are ordinary characters, and matching is case-sensitive.

## Select each conference winner

Compute the distance from the opening-band name to every AFC team and independently from the halftime-band name to every NFC team. Keep the smallest distance. On equal distances, keep the earlier input team; using a strict `<` update (not `<=`) preserves this rule.

The AFC and NFC competitions are separate: do not compare a team from one conference with the band's name for the other conference.

## Input parsing

Names may contain spaces. Read complete lines, not whitespace-delimited tokens:

1. Read `N`.
2. Read the opening band line.
3. Read exactly `N` AFC team lines.
4. Read the halftime band line.
5. Read exactly `N` NFC team lines.

Do not trim meaningful spaces inside names; remove only line-ending characters.

## Complexity

Name lengths are at most about 100 and `N <= 16`. One Levenshtein computation costs `O(LR)` time and `O(LR)` memory, or `O(min(L,R))` memory using two rows. The total work is negligible.

## Common mistakes

- Using Hamming distance, which does not allow insertions or deletions.
- Ignoring spaces or comparing case-insensitively.
- Breaking ties by alphabetic order instead of original input order.
- Parsing names with `cin >> name` / token splitting, which loses spaces.

## Note about the provided first sample

The first screenshot's sample output selects **Minnesota Vikings** for **The Weeknd**, but standard case-sensitive Levenshtein distances give 14 to “Minnesota Vikings” and 12 to “New York Giants.” The sample output therefore conflicts with the stated edit-distance rule. The C implementation follows the rule; if the judge expects the displayed sample literally, the challenge statement needs clarification.
