# Even Signal Windows

## English

### Problem overview

A sensor records a binary signal as a string `S` containing only `0` and `1`. A **window** is a nonempty contiguous part of the signal. A window is **stable** when it contains an even number of `1` characters. This includes a window with no `1` characters.

For each query `[l, r]`, count the stable windows that lie completely inside `S[l…r]`. Each query is independent; it does not change the signal or affect other queries.

### Input

- The first line contains two integers, `N` and `Q`: the signal length and the number of queries.
- The second line contains the binary string `S`, of length `N`.
- Each of the next `Q` lines contains two integers `l` and `r`, describing one query. Positions are **1-indexed**, and `1 ≤ l ≤ r ≤ N`.

### Output

For each query, print one integer: the number of stable windows contained entirely within the requested substring.

### Constraints

- `1 ≤ N, Q ≤ 200,000`
- `1 ≤ l ≤ r ≤ N`
- `S` contains only `0` and `1`.

### Key observation: prefix parity

Define `P[i]` as the parity (even or odd) of the number of `1`s among the first `i` characters of `S`, with `P[0] = 0`.

For a window from position `a` through position `b`, the number of `1`s in it is even exactly when `P[a-1] = P[b]`. The parity of the number of `1`s in a segment is the XOR of the two prefix parities; it is even when that XOR is zero.

Therefore, for query `[l, r]`, consider the prefix parity values at indices `l-1, l, …, r`. Every pair of equal parity values identifies one stable window in the query substring:

- Choosing two prefix indices `i < j` corresponds to the window `S[i+1…j]`.
- That window is stable precisely when `P[i] = P[j]`.
- The window is nonempty because the pair has distinct indices.

### Counting a query

Let:

- `E` be the number of even prefix parities among `P[l-1…r]`.
- `O` be the number of odd prefix parities among `P[l-1…r]`.

Any two even values form a stable window, and any two odd values form a stable window. Thus the answer is:

`E × (E - 1) / 2 + O × (O - 1) / 2`

To answer all queries efficiently, build two prefix-count arrays over the parity sequence `P[0…N]`: one counts even parities, the other counts odd parities. The counts for indices `[l-1, r]` can then be obtained by subtracting prefix counts. This gives constant-time processing per query after linear preprocessing.

### Algorithm outline

1. Set `P[0] = 0`; scan `S` and update the parity by toggling it whenever the current character is `1`.
2. Build cumulative counts of even and odd values in `P`.
3. For each query `[l, r]`, obtain `E` and `O` for prefix indices `[l-1, r]`.
4. Print `E(E-1)/2 + O(O-1)/2`.

### Complexity and numeric type

- Preprocessing: `O(N)` time and `O(N)` memory.
- Each query: `O(1)` time.
- Total: `O(N + Q)` time.
- Use a 64-bit integer type for the answer. The number of windows can be as large as `N(N+1)/2`.

### Examples

#### Example 1

Input:

```text
5 3
10110
1 5
2 4
3 3
```

Output:

```text
7
3
0
```

For `[1, 5]`, the stable windows are the two single-character `0` windows, `11`, `101`, `011`, `110`, and `0110`, for a total of `7`.

#### Example 2

Input:

```text
4 3
0000
1 4
2 3
4 4
```

Output:

```text
10
3
1
```

Every window in an all-zero signal contains zero `1`s, so every window is stable. A substring of length `k` has `k(k+1)/2` nonempty windows.

---

## සිංහල

### ගැටලුවේ සාරාංශය

සංවේදකයක් `0` සහ `1` පමණක් අඩංගු ද්විමය සංඥාවක් `S` යන string එකක් ලෙස සටහන් කරයි. **Window** එකක් යනු සංඥාවේ අඛණ්ඩව පිහිටි, හිස් නොවන කොටසකි. එහි `1` අක්ෂර ගණන ඉරට්ටේ නම් එය **ස්ථාවර (stable)** window එකකි. `1` කිසිවක් නොමැති window එකක්ද ස්ථාවර වේ.

එක් එක් `[l, r]` විමසුම සඳහා, `S[l…r]` උප-තන්තුව තුළ සම්පූර්ණයෙන්ම අඩංගු ස්ථාවර windows ගණන සොයන්න. විමසුම් එකිනෙකින් ස්වාධීන වන අතර, එක් විමසුමක් සංඥාව හෝ අනෙක් විමසුම් වෙනස් නොකරයි.

### ආදානය

- පළමු පේළියේ `N` සහ `Q` යන පූර්ණ සංඛ්‍යා දෙක ඇත: සංඥාවේ දිග සහ විමසුම් ගණන.
- දෙවන පේළියේ දිග `N` වන ද්විමය string එක `S` ඇත.
- ඊළඟ `Q` පේළිවල එක් එක් විමසුම සඳහා `l` සහ `r` යන පූර්ණ සංඛ්‍යා දෙක ඇත. ස්ථාන **1 සිට ගණනය කරයි**; `1 ≤ l ≤ r ≤ N`.

### ප්‍රතිදානය

එක් එක් විමසුම සඳහා, අදාළ උප-තන්තුව තුළ සම්පූර්ණයෙන්ම අඩංගු ස්ථාවර windows ගණන වන එක් පූර්ණ සංඛ්‍යාවක් මුද්‍රණය කරන්න.

### සීමාවන්

- `1 ≤ N, Q ≤ 200,000`
- `1 ≤ l ≤ r ≤ N`
- `S` තුළ ඇත්තේ `0` සහ `1` පමණි.

### ප්‍රධාන අදහස: prefix parity

`P[i]` යනු `S` හි පළමු `i` අක්ෂරවල ඇති `1` ගණනේ parity එක (ඉරට්ටේද/ඔත්තේද) ලෙස ගනිමු. `P[0] = 0` ලෙස සකසන්න.

`a` සිට `b` දක්වා ඇති window එකේ `1` ගණන ඉරට්ටේ වන්නේ `P[a-1] = P[b]` වූ විට පමණි. කොටසක `1` ගණනේ parity එක prefix parity දෙකේ XOR එකයි; XOR ප්‍රතිඵලය `0` නම් එම ගණන ඉරට්ටේය.

ඒ අනුව `[l, r]` විමසුමක් සඳහා `P[l-1], P[l], …, P[r]` අගයන් සලකා බලන්න. සමාන parity ඇති අගයන් දෙකක් තෝරාගත් සෑම අවස්ථාවක්ම විමසුමේ ස්ථාවර window එකක් නිරූපණය කරයි:

- `i < j` වන prefix ස්ථාන දෙකක් තෝරාගත් විට අදාළ window එක `S[i+1…j]` වේ.
- `P[i] = P[j]` නම් එම window එකේ `1` ගණන ඉරට්ටේය.
- prefix ස්ථාන දෙක වෙනස් බැවින් window එක හිස් නොවේ.

### එක් විමසුමක් ගණනය කිරීම

`P[l-1…r]` තුළ:

- `E` යනු ඉරට්ටේ prefix parity අගයන් ගණනයි.
- `O` යනු ඔත්තේ prefix parity අගයන් ගණනයි.

ඉරට්ටේ අගයන් දෙකක් තෝරාගන්නා සෑම යුගලයක්ම එක් ස්ථාවර window එකක් ලබා දෙයි. ඔත්තේ අගයන් දෙකක් තෝරාගත් විටද එයම සිදුවේ. එබැවින් පිළිතුර:

`E × (E - 1) / 2 + O × (O - 1) / 2`

සියලු විමසුම් වේගයෙන් සැකසීමට, `P[0…N]` අගයන් සඳහා ඉරට්ටේ සහ ඔත්තේ parity ගණන් කරන prefix-count arrays දෙකක් සාදන්න. අඩු කිරීම් භාවිතයෙන් `[l-1, r]` පරාසයේ ගණන් ලබාගත හැක. එවිට preprocessing කිරීමෙන් පසු එක් විමසුමකට ස්ථාවර කාලයක් ගතවේ.

### ඇල්ගොරිතමයේ පියවර

1. `P[0] = 0` ලෙස සකසා `S` පරීක්ෂා කරන්න. වත්මන් අක්ෂරය `1` නම් parity එක මාරු කරන්න.
2. `P` අගයන් සඳහා ඉරට්ටේ සහ ඔත්තේ prefix counts සාදන්න.
3. එක් එක් `[l, r]` විමසුම සඳහා prefix ස්ථාන `[l-1, r]` තුළ ඇති `E` සහ `O` ලබාගන්න.
4. `E(E-1)/2 + O(O-1)/2` මුද්‍රණය කරන්න.

### සංකීර්ණතාව සහ සංඛ්‍යාත්මක වර්ගය

- පෙර සකස් කිරීම: `O(N)` කාලය සහ `O(N)` මතකය.
- එක් විමසුමක්: `O(1)` කාලය.
- මුළු කාල සංකීර්ණතාව: `O(N + Q)`.
- පිළිතුර සඳහා 64-bit පූර්ණ සංඛ්‍යා වර්ගයක් භාවිතා කරන්න. උපරිම window ගණන `N(N+1)/2` විය හැක.

### උදාහරණ

#### උදාහරණය 1

ආදානය:

```text
5 3
10110
1 5
2 4
3 3
```

ප්‍රතිදානය:

```text
7
3
0
```

`[1, 5]` සඳහා ස්ථාවර windows ගණන `7` කි. ඒවා වන්නේ `0` අක්ෂරය පමණක් ඇති windows දෙක, `11`, `101`, `011`, `110`, සහ `0110` ය.

#### උදාහරණය 2

ආදානය:

```text
4 3
0000
1 4
2 3
4 4
```

ප්‍රතිදානය:

```text
10
3
1
```

ශුන්‍ය පමණක් ඇති සංඥාවක සෑම window එකකම `1` ගණන ශූන්‍ය බැවින් සෑම window එකක්ම ස්ථාවර වේ. දිග `k` වන උප-තන්තුවක හිස් නොවන windows ගණන `k(k+1)/2` කි.
