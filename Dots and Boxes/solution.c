#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LIM 10
static int R, C;
static unsigned char h[LIM + 1][LIM], v[LIM][LIM + 1];
static char rows[2 * LIM + 1][2 * LIM + 4];

static int box_sides(int r, int c) {
    return h[r][c] + h[r + 1][c] + v[r][c] + v[r][c + 1];
}

static void add_line(int r1, int c1, int r2, int c2) {
    if (r1 == r2) {
        int c = c1 < c2 ? c1 : c2;
        if (r1 >= 0 && r1 <= R && c >= 0 && c < C) h[r1][c] = 1;
    } else {
        int r = r1 < r2 ? r1 : r2;
        if (r >= 0 && r < R && c1 >= 0 && c1 <= C) v[r][c1] = 1;
    }
}

static int score_edge(int horizontal, int r, int c) {
    int capture = 0, danger = 0, affected = 0;
    if (horizontal) {
        if (r > 0) { int n = box_sides(r - 1, c); ++affected; capture += n == 3; danger += n == 2; }
        if (r < R) { int n = box_sides(r, c); ++affected; capture += n == 3; danger += n == 2; }
    } else {
        if (c > 0) { int n = box_sides(r, c - 1); ++affected; capture += n == 3; danger += n == 2; }
        if (c < C) { int n = box_sides(r, c); ++affected; capture += n == 3; danger += n == 2; }
    }
    if (capture) return 10000 + capture * 100;
    /* Avoid creating a capturable box; this tactical rule is a heuristic for chain control. */
    int score = -100 * danger;
    if (!danger) score += 12;
    score -= affected;
    score += (horizontal ? (r * 7 + c * 3) : (r * 3 + c * 7)) % 5;
    return score;
}

int main(void) {
    char line[512];
    if (!fgets(line, sizeof line, stdin)) return 0;
    int first;
    if (sscanf(line, "GAME %d %d %d", &R, &C, &first) != 3 ||
        R < 1 || R > LIM || C < 1 || C > LIM) return 0;
    for (int r = 0; r <= 2 * R; ++r) {
        if (!fgets(rows[r], sizeof rows[r], stdin)) return 0;
        rows[r][strcspn(rows[r], "\r\n")] = '\0';
    }
    for (int r = 0; r <= R; ++r)
        for (int c = 0; c < C; ++c)
            h[r][c] = rows[2 * r][2 * c + 1] == '-';
    for (int r = 0; r < R; ++r)
        for (int c = 0; c <= C; ++c)
            v[r][c] = rows[2 * r + 1][2 * c] == '|';

    while (fgets(line, sizeof line, stdin)) {
        if (strncmp(line, "GAME_END", 8) == 0 || strncmp(line, "-1", 2) == 0) break;
        if (strncmp(line, "OPPONENT", 8) == 0) {
            int r1, c1, r2, c2;
            if (sscanf(line, "OPPONENT %d %d %d %d", &r1, &c1, &r2, &c2) == 4)
                add_line(r1, c1, r2, c2);
            continue;
        }
        if (strncmp(line, "YOUR_TURN", 9) != 0) continue;

        int best_score = -1000000, best_h = 1, best_r = 0, best_c = 0;
        for (int r = 0; r <= R; ++r) for (int c = 0; c < C; ++c)
            if (!h[r][c]) {
                int s = score_edge(1, r, c);
                if (s > best_score) { best_score = s; best_h = 1; best_r = r; best_c = c; }
            }
        for (int r = 0; r < R; ++r) for (int c = 0; c <= C; ++c)
            if (!v[r][c]) {
                int s = score_edge(0, r, c);
                if (s > best_score) { best_score = s; best_h = 0; best_r = r; best_c = c; }
            }
        if (best_h) printf("LINE %d %d %d %d\n", best_r, best_c, best_r, best_c + 1);
        else printf("LINE %d %d %d %d\n", best_r, best_c, best_r + 1, best_c);
        fflush(stdout);
        if (best_h) h[best_r][best_c] = 1;
        else v[best_r][best_c] = 1;
    }
    return 0;
}
