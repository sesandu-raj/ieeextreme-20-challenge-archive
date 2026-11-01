# IEEE Global Expert Selection Challenge

## Problem in one sentence

Parse a CSV-style expert registry, filter experts by the chair's selection criteria, rank eligible candidates, and print the chair followed by the required number of experts.

## Parse records by header, not by column position

The header names define the fields, and their order is not guaranteed. Read the six column names and build a map from field name to index. Parse every following row into a record containing:

- Expert ID;
- Name;
- Domain;
- Education;
- Years of Experience;
- Time Zone.

The first line contains comma-separated criteria in this order: chair ID, number needed, minimum education level, minimum years of experience, required domain, and maximum time-zone difference. CSV fields can contain spaces, so split on commas and trim whitespace rather than splitting on spaces.

## Eligibility filters

Look up the chair record first because its time zone is the reference point. Exclude the chair from the candidate pool; the chair is printed separately as the first output record.

An eligible candidate must satisfy all criteria:

- Their education is at least the requested level;
- Their experience is at least the requested minimum;
- Their domain equals the requested domain;
- Their absolute time-zone difference from the chair is no greater than the allowed difference.

Map education to an ordinal rank, for example `Associates = 0`, `Undergraduate = 1`, `Masters = 2`, `PhD = 3`, so comparisons are explicit.

## Ranking and selection

If more candidates qualify than are needed, sort by:

1. Education rank, descending;
2. Years of experience, descending;
3. Absolute time-zone distance from the chair, ascending.

The statement guarantees that these criteria do not tie simultaneously for the relevant cutoff. The output order is the chair first, then the selected records in this ranking order. Print each selected record in the same comma-separated format as the input.

## Correctness outline

Filtering removes precisely candidates that violate at least one hard requirement. Sorting applies the stated preference order lexicographically, so the first `X` eligible candidates are the required committee members. Keeping the chair separate ensures they remain first regardless of their ranking among candidates.

## Complexity

For `R <= 10,000` records, parsing and filtering are `O(R)`, sorting is `O(R log R)`, and memory is `O(R)`.

## Common pitfalls

- Assuming a fixed order of the six metadata columns.
- Splitting the names or domain on spaces.
- Using signed time-zone difference instead of absolute difference.
- Treating requested education as an exact match rather than a minimum level.
- Sorting the chair among candidates instead of always printing the chair first.
- Losing fields or changing their format while reproducing the selected records.
