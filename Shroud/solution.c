#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAXN 60
#define MAXU 8

static char terrain[MAXN][MAXN], paint[MAXN][MAXN];
static int R, C, U, V, T;
static int ur[MAXU], uc[MAXU];
static int enemy_r[MAXU], enemy_c[MAXU], enemy_count;
static int dr[5] = {-1, 0, 1, 0, 0};
static int dc[5] = {0, 1, 0, -1, 0};
static char *orders[5] = {"N", "E", "S", "W", "H"};

static int in_bounds(int r, int c) {
    return r >= 0 && r < R && c >= 0 && c < C;
}

static int read_line(char *s, size_t n) {
    if (!fgets(s, (int)n, stdin)) return 0;
    size_t len = strlen(s);
    while (len && (s[len - 1] == '\n' || s[len - 1] == '\r')) s[--len] = '\0';
    return 1;
}

static int cell_value(int r, int c) {
    if (!in_bounds(r, c) || terrain[r][c] == '#') return -100000;
    int value;
    if (paint[r][c] == '+') value = 0;
    else if (paint[r][c] == '-') value = 8;
    else if (terrain[r][c] == '?') value = 5;
    else value = 4;
    int unknown_near = 0, enemy_near = 0;
    for (int d = 0; d < 4; ++d) {
        int nr = r + dr[d], nc = c + dc[d];
        if (in_bounds(nr, nc)) {
            if (terrain[nr][nc] == '?') ++unknown_near;
            if (paint[nr][nc] == '-') ++enemy_near;
        }
    }
    return value + unknown_near + 2 * enemy_near;
}

int main(void) {
    char line[512];
    if (!read_line(line, sizeof line)) return 0;
    int known;
    if (sscanf(line, "GAME %d %d %d %d %d %d", &R, &C, &U, &V, &T, &known) != 6 ||
        R < 1 || R > MAXN || C < 1 || C > MAXN || U < 1 || U > MAXU) return 0;
    memset(terrain, '?', sizeof terrain);
    memset(paint, '.', sizeof paint);
    if (known) {
        if (!read_line(line, sizeof line) || strncmp(line, "MAP", 3) != 0) return 0;
        for (int r = 0; r < R; ++r) {
            if (!read_line(line, sizeof line)) return 0;
            for (int c = 0; c < C && line[c]; ++c)
                terrain[r][c] = line[c] == '#' ? '#' : '.';
        }
    }

    while (read_line(line, sizeof line)) {
        if (strncmp(line, "GAME_END", 8) == 0 || strcmp(line, "-1") == 0) break;
        int round_no, cells;
        if (sscanf(line, "ROUND %d %d", &round_no, &cells) != 2) continue;
        enemy_count = 0;
        for (int i = 0; i < U; ++i) {
            if (!read_line(line, sizeof line)) return 0;
            int id;
            if (sscanf(line, "UNIT %d %d %d", &id, &ur[i], &uc[i]) != 3) return 0;
            for (int vr = 0; vr < 2 * V + 1; ++vr) {
                if (!read_line(line, sizeof line)) return 0;
                for (int vc = 0; vc < 2 * V + 1 && line[vc]; ++vc) {
                    int r = ur[i] - V + vr, c = uc[i] - V + vc;
                    if (!in_bounds(r, c)) continue;
                    char ch = line[vc];
                    if (ch == '#') terrain[r][c] = '#';
                    else if (ch == '.' || ch == '+' || ch == '-' || ch == 'M' || ch == 'E') {
                        terrain[r][c] = '.';
                        if (ch == '+' || ch == '-') paint[r][c] = ch;
                        else if (ch == 'M') paint[r][c] = '+';
                        else if (ch == 'E') {
                            paint[r][c] = '-';
                            int duplicate = 0;
                            for (int e = 0; e < enemy_count; ++e)
                                if (enemy_r[e] == r && enemy_c[e] == c) duplicate = 1;
                            if (!duplicate && enemy_count < MAXU) {
                                enemy_r[enemy_count] = r;
                                enemy_c[enemy_count++] = c;
                            }
                        }
                        else if (paint[r][c] != '+' && paint[r][c] != '-') paint[r][c] = '.';
                    }
                }
            }
        }

        int chosen[MAXU], dest_r[MAXU], dest_c[MAXU];
        for (int i = 0; i < U; ++i) {
            double best = -1e30;
            chosen[i] = 4;
            for (int d = 0; d < 5; ++d) {
                int nr = ur[i] + dr[d], nc = uc[i] + dc[d];
                if (d == 4) { nr = ur[i]; nc = uc[i]; }
                if (!in_bounds(nr, nc) || terrain[nr][nc] == '#') continue;
                int occupied = 0;
                for (int j = 0; j < U; ++j)
                    if (j != i && ur[j] == nr && uc[j] == nc) occupied = 1;
                for (int e = 0; e < enemy_count; ++e)
                    if (enemy_r[e] == nr && enemy_c[e] == nc && d != 4) occupied = 1;
                if (occupied && d != 4) continue;
                double score = cell_value(nr, nc);
                if (d == 4) score -= 2.5;
                if (terrain[nr][nc] == '?') score -= 1.0;
                /* Greedy local expansion is a heuristic; unknown cells are explored at a modest premium. */
                for (int k = 0; k < U; ++k)
                    if (k < i && dest_r[k] == nr && dest_c[k] == nc) score -= 1000;
                if (score > best) { best = score; chosen[i] = d; }
            }
            dest_r[i] = ur[i] + dr[chosen[i]];
            dest_c[i] = uc[i] + dc[chosen[i]];
            if (chosen[i] == 4) { dest_r[i] = ur[i]; dest_c[i] = uc[i]; }
        }
        printf("ORDERS");
        for (int i = 0; i < U; ++i) printf(" %s", orders[chosen[i]]);
        putchar('\n');
        fflush(stdout);
    }
    return 0;
}
