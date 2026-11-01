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

static int distance(const char *a, const char *b) {
    size_t n = strlen(a), m = strlen(b);
    int *old = malloc((m + 1) * sizeof(*old));
    int *now = malloc((m + 1) * sizeof(*now));
    if (!old || !now) {
        free(old);
        free(now);
        return -1;
    }
    for (size_t j = 0; j <= m; ++j) old[j] = (int)j;
    for (size_t i = 1; i <= n; ++i) {
        now[0] = (int)i;
        for (size_t j = 1; j <= m; ++j) {
            int x = old[j] + 1, y = now[j - 1] + 1;
            int z = old[j - 1] + (a[i - 1] != b[j - 1]);
            now[j] = x < y ? x : y;
            if (z < now[j]) now[j] = z;
        }
        int *tmp = old;
        old = now;
        now = tmp;
    }
    int result = old[m];
    free(old);
    free(now);
    return result;
}

static int choose(int n, const char *band, char **chosen) {
    int best = 0x7fffffff;
    *chosen = NULL;
    for (int i = 0; i < n; ++i) {
        char *team = read_line();
        if (!team) return 0;
        int d = distance(band, team);
        if (d < 0) {
            free(team);
            return 0;
        }
        if (d < best) {
            free(*chosen);
            *chosen = team;
            best = d;
        } else free(team);
    }
    return *chosen != NULL;
}

int main(void) {
    char *line = read_line();
    if (!line) return 0;
    int n = atoi(line);
    free(line);
    if (n <= 0) return 1;
    char *opening = read_line(), *afc = NULL;
    if (!opening || !choose(n, opening, &afc)) {
        free(opening);
        free(afc);
        return 1;
    }
    free(opening);
    char *halftime = read_line(), *nfc = NULL;
    if (!halftime || !choose(n, halftime, &nfc)) {
        free(halftime);
        free(afc);
        free(nfc);
        return 1;
    }
    printf("%s x %s\n", afc, nfc);
    free(halftime);
    free(afc);
    free(nfc);
    return 0;
}
