#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *read_line(void) {
    char *line = NULL;
    size_t capacity = 0;
    ssize_t length = getline(&line, &capacity, stdin);
    if (length < 0) {
        free(line);
        return NULL;
    }
    while (length > 0 && (line[length - 1] == '\n' || line[length - 1] == '\r'))
        line[--length] = '\0';
    return line;
}

static int edit_distance(const char *a, const char *b) {
    size_t n = strlen(a), m = strlen(b);
    int *prev = malloc((m + 1) * sizeof(*prev));
    int *cur = malloc((m + 1) * sizeof(*cur));
    if (!prev || !cur) {
        free(prev);
        free(cur);
        return -1;
    }
    for (size_t j = 0; j <= m; ++j) prev[j] = (int)j;
    for (size_t i = 1; i <= n; ++i) {
        cur[0] = (int)i;
        for (size_t j = 1; j <= m; ++j) {
            int deletion = prev[j] + 1;
            int insertion = cur[j - 1] + 1;
            int substitution = prev[j - 1] + (a[i - 1] != b[j - 1]);
            int best = deletion < insertion ? deletion : insertion;
            cur[j] = best < substitution ? best : substitution;
        }
        int *tmp = prev;
        prev = cur;
        cur = tmp;
    }
    int answer = prev[m];
    free(prev);
    free(cur);
    return answer;
}

static int read_best_team(int n, const char *band, char **best_name) {
    int best_distance = 0x7fffffff;
    *best_name = NULL;
    for (int i = 0; i < n; ++i) {
        char *team = read_line();
        if (!team) return 0;
        int distance = edit_distance(band, team);
        if (distance < 0) {
            free(team);
            return 0;
        }
        if (distance < best_distance) {
            free(*best_name);
            *best_name = team;
            best_distance = distance;
        } else {
            free(team);
        }
    }
    return *best_name != NULL;
}

int main(void) {
    char *line = read_line();
    if (!line) return 0;
    int n = atoi(line);
    free(line);
    if (n <= 0) return 1;

    char *opening = read_line();
    if (!opening) return 1;
    char *afc = NULL;
    if (!read_best_team(n, opening, &afc)) {
        free(opening);
        return 1;
    }
    free(opening);

    char *halftime = read_line();
    if (!halftime) {
        free(afc);
        return 1;
    }
    char *nfc = NULL;
    if (!read_best_team(n, halftime, &nfc)) {
        free(halftime);
        free(afc);
        return 1;
    }
    free(halftime);

    printf("%s x %s\n", afc, nfc);
    free(afc);
    free(nfc);
    return 0;
}
