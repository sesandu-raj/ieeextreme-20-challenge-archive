# Lunar Giant

## What the screenshots specify

Each test case is a list of integers `C1, ..., Ck`. The required output is the integer produced by a program encoded in an **attached transition table**. The transition table is the actual definition of the computation; the visible statement does not define a mathematical function from the input list to the answer by itself.

## Required missing input

The screenshots in this folder show the statement and sample cases, but the referenced transition table is not present among the supplied files. The samples alone are insufficient to determine the hidden state transitions or infer outputs for arbitrary input.

Consequently, a reliable solution algorithm cannot be derived from the available materials without guessing. Obtain the transition table (or a screenshot/export of it) before implementing this challenge.

## Once the table is available

The implementation process should be:

1. Identify the table's states, symbols/values, initial state, transition rule, and output rule.
2. Determine how each list element `Ci` is consumed: as a symbol, a repetition count, a transition index, or another table-defined operation.
3. Simulate the encoded program for each testcase, resetting its state as specified between lines.
4. Match the sample outputs exactly and then test empty-like/zero values, repeated values, and the maximum value `1000`.

Do not extrapolate the transition behavior from the sample outputs alone.
