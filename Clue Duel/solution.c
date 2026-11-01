#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXCARD 41
static int sizes[3], hand[3][MAXCARD + 1], possible[3][MAXCARD + 1];
static unsigned char guessed[MAXCARD + 1][MAXCARD + 1][MAXCARD + 1];
static int last_guess[3];

static int card_category(const char *s) {
    if (!s || !s[0]) return -1;
    if (s[0] == 'P') return 0;
    if (s[0] == 'W') return 1;
    if (s[0] == 'L') return 2;
    return -1;
}

static int card_number(const char *s) {
    if (card_category(s) < 0) return 0;
    char *end;
    long n = strtol(s + 1, &end, 10);
    if (end == s + 1 || *end != '\0' || n < 1 || n > MAXCARD) return 0;
    return (int)n;
}

static void eliminate(int cat, int num) {
    if (cat >= 0 && cat < 3 && num > 0 && num <= sizes[cat]) possible[cat][num] = 0;
}

static int candidate_count(int cat) {
    int n = 0;
    for (int i = 1; i <= sizes[cat]; ++i) n += possible[cat][i] != 0;
    return n;
}

int main(void) {
    char line[4096];
    if (!fgets(line, sizeof line, stdin)) return 0;
    int turns;
    if (sscanf(line, "GAME %d %d %d %d", &sizes[0], &sizes[1], &sizes[2], &turns) != 4 ||
        sizes[0] < 1 || sizes[0] > MAXCARD || sizes[1] < 1 || sizes[1] > MAXCARD ||
        sizes[2] < 1 || sizes[2] > MAXCARD) return 0;
    if (!fgets(line, sizeof line, stdin)) return 0;
    char *tok = strtok(line, " \t\r\n");
    if (!tok || strcmp(tok, "HAND") != 0) return 0;
    while ((tok = strtok(NULL, " \t\r\n")) != NULL) {
        int cat = card_category(tok), num = card_number(tok);
        if (cat >= 0 && num > 0 && num <= sizes[cat]) {
            hand[cat][num] = 1;
            possible[cat][num] = 0;
        }
    }
    for (int cat = 0; cat < 3; ++cat)
        for (int n = 1; n <= sizes[cat]; ++n)
            if (!hand[cat][n]) possible[cat][n] = 1;

    while (fgets(line, sizeof line, stdin)) {
        if (strncmp(line, "GAME_END", 8) == 0 || strncmp(line, "-1", 2) == 0) break;
        if (strncmp(line, "RESULT SHOWN ", 13) == 0) {
            char card[32];
            if (sscanf(line + 13, "%31s", card) == 1) {
                int cat = card_category(card), num = card_number(card);
                eliminate(cat, num);
            }
            continue;
        }
        if (strncmp(line, "RESULT NONE", 11) == 0) {
            /* With no disproof, every guessed non-hand card is the corresponding solution card. */
            for (int cat = 0; cat < 3; ++cat) {
                int num = last_guess[cat];
                if (num > 0 && num <= sizes[cat] && !hand[cat][num]) {
                    for (int n = 1; n <= sizes[cat]; ++n) possible[cat][n] = (n == num);
                }
            }
            continue;
        }
        if (strncmp(line, "TURN OPP GUESS ", 15) == 0) {
            char a[32], b[32], c[32];
            if (sscanf(line + 15, "%31s %31s %31s", a, b, c) != 3) continue;
            char *cards[3] = {a, b, c};
            int shown = 0;
            for (int i = 0; i < 3; ++i) {
                int cat = card_category(cards[i]), num = card_number(cards[i]);
                if (cat >= 0 && num > 0 && num <= sizes[cat] && hand[cat][num]) {
                    printf("SHOW %s\n", cards[i]);
                    fflush(stdout);
                    shown = 1;
                    break;
                }
            }
            if (!shown) { puts("NONE"); fflush(stdout); }
            continue;
        }
        if (strncmp(line, "TURN YOU", 8) != 0) continue;

        int pick[3] = {0, 0, 0};
        for (int cat = 0; cat < 3; ++cat) {
            int count = candidate_count(cat);
            if (count == 1) {
                for (int n = 1; n <= sizes[cat]; ++n)
                    if (possible[cat][n]) pick[cat] = n;
            } else {
                for (int n = 1; n <= sizes[cat]; ++n)
                    if (possible[cat][n]) { pick[cat] = n; break; }
            }
            if (!pick[cat]) pick[cat] = 1;
        }
        if (candidate_count(0) == 1 && candidate_count(1) == 1 && candidate_count(2) == 1) {
            printf("ACCUSE P%d W%d L%d\n", pick[0], pick[1], pick[2]);
            fflush(stdout);
            continue;
        }
        /* Guessing unresolved candidates is a heuristic; a new triple is preferred for information. */
        int found = 0;
        for (int p = 1; p <= sizes[0] && !found; ++p) if (possible[0][p])
            for (int w = 1; w <= sizes[1] && !found; ++w) if (possible[1][w])
                for (int l = 1; l <= sizes[2]; ++l) if (possible[2][l] && !guessed[p][w][l]) {
                    pick[0] = p; pick[1] = w; pick[2] = l;
                    found = 1;
                    break;
                }
        guessed[pick[0]][pick[1]][pick[2]] = 1;
        last_guess[0] = pick[0];
        last_guess[1] = pick[1];
        last_guess[2] = pick[2];
        printf("GUESS P%d W%d L%d\n", pick[0], pick[1], pick[2]);
        fflush(stdout);
    }
    return 0;
}
