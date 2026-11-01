#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum { F_ID, F_NAME, F_DOMAIN, F_EDUCATION, F_EXPERIENCE, F_TIMEZONE, F_COUNT };

typedef struct {
    char *fields[F_COUNT];
    int education;
    int experience;
    int timezone;
    int original_order;
    char *raw;
} Expert;

static char *line_read(void) {
    char *line = NULL;
    size_t cap = 0;
    ssize_t n = getline(&line, &cap, stdin);
    if (n < 0) {
        free(line);
        return NULL;
    }
    while (n > 0 && (line[n - 1] == '\n' || line[n - 1] == '\r'))
        line[--n] = '\0';
    return line;
}

static char *trim(char *s) {
    while (*s == ' ' || *s == '\t') ++s;
    size_t n = strlen(s);
    while (n && (s[n - 1] == ' ' || s[n - 1] == '\t')) s[--n] = '\0';
    return s;
}

/* The challenge describes a flat CSV without quoted commas. */
static int split_csv(char *line, char **fields, int expected) {
    int count = 0;
    char *start = line;
    for (char *p = line;; ++p) {
        if (*p == ',' || *p == '\0') {
            char end = *p;
            *p = '\0';
            if (count >= expected) return -1;
            fields[count++] = trim(start);
            if (end == '\0') break;
            start = p + 1;
        }
    }
    return count;
}

static int education_rank(const char *s) {
    if (strcmp(s, "PhD") == 0) return 3;
    if (strcmp(s, "Masters") == 0) return 2;
    if (strcmp(s, "Undergraduate") == 0) return 1;
    if (strcmp(s, "Associates") == 0) return 0;
    return -1;
}

static int compare_selected(const void *pa, const void *pb) {
    const Expert *a = *(const Expert *const *)pa;
    const Expert *b = *(const Expert *const *)pb;
    if (a->education != b->education) return b->education - a->education;
    if (a->experience != b->experience) return b->experience - a->experience;
    return strcmp(a->fields[F_ID], b->fields[F_ID]);
}

static void print_expert(const Expert *e, const int column[F_COUNT]) {
    for (int c = 0; c < F_COUNT; ++c) {
        int field = 0;
        while (field < F_COUNT && column[field] != c) ++field;
        if (field == F_COUNT) return;
        if (c) fputs(", ", stdout);
        fputs(e->fields[field], stdout);
    }
    putchar('\n');
}

int main(void) {
    char *criteria_line = line_read();
    char *header_line = line_read();
    if (!criteria_line || !header_line) {
        free(criteria_line);
        free(header_line);
        return 1;
    }

    char *criteria[6];
    int criteria_count = split_csv(criteria_line, criteria, 6);
    if (criteria_count != 6) {
        free(criteria_line);
        free(header_line);
        return 1;
    }
    int needed = atoi(criteria[1]);
    int minimum_education = education_rank(criteria[2]);
    int minimum_experience = atoi(criteria[3]);
    int max_timezone_difference = atoi(criteria[5]);
    if (needed < 0 || minimum_education < 0 || max_timezone_difference < 0) {
        free(criteria_line);
        free(header_line);
        return 1;
    }

    char *headers[F_COUNT];
    int header_count = split_csv(header_line, headers, F_COUNT);
    if (header_count != F_COUNT) {
        free(criteria_line);
        free(header_line);
        return 1;
    }
    int column[F_COUNT];
    const char *names[F_COUNT] = {
        "Expert Id", "Name", "Domains", "Education",
        "Years of Experience", "Time Zone"
    };
    for (int f = 0; f < F_COUNT; ++f) {
        column[f] = -1;
        for (int c = 0; c < F_COUNT; ++c)
            if (strcmp(headers[c], names[f]) == 0) column[f] = c;
        if (column[f] < 0) {
            free(criteria_line);
            free(header_line);
            return 1;
        }
    }

    Expert *experts = NULL;
    size_t count = 0, capacity = 0;
    char *raw;
    while ((raw = line_read()) != NULL) {
        if (count == capacity) {
            size_t new_capacity = capacity ? capacity * 2 : 32;
            Expert *grown = realloc(experts, new_capacity * sizeof(*experts));
            if (!grown) {
                free(raw);
                goto fail;
            }
            experts = grown;
            capacity = new_capacity;
        }
        Expert *e = &experts[count];
        memset(e, 0, sizeof(*e));
        e->raw = raw;
        char *parsed[F_COUNT];
        if (split_csv(raw, parsed, F_COUNT) != F_COUNT) goto fail;
        for (int f = 0; f < F_COUNT; ++f) e->fields[f] = parsed[column[f]];
        e->education = education_rank(e->fields[F_EDUCATION]);
        e->experience = atoi(e->fields[F_EXPERIENCE]);
        e->timezone = atoi(e->fields[F_TIMEZONE]);
        e->original_order = (int)count;
        if (e->education < 0) goto fail;
        ++count;
    }

    Expert *chair = NULL;
    for (size_t i = 0; i < count; ++i)
        if (strcmp(experts[i].fields[F_ID], criteria[0]) == 0) {
            chair = &experts[i];
            break;
        }
    if (!chair) goto fail;

    Expert **eligible = malloc((count ? count : 1) * sizeof(*eligible));
    if (!eligible) goto fail;
    size_t eligible_count = 0;
    for (size_t i = 0; i < count; ++i) {
        Expert *e = &experts[i];
        int tz_difference = abs(e->timezone - chair->timezone);
        if (e == chair ||
            e->education < minimum_education ||
            e->experience < minimum_experience ||
            strcmp(e->fields[F_DOMAIN], criteria[4]) != 0 ||
            tz_difference > max_timezone_difference)
            continue;
        eligible[eligible_count++] = e;
    }

    /* Select using eligibility precedence, including proximity to the chair. */
    for (size_t i = 0; i < eligible_count; ++i) {
        for (size_t j = i + 1; j < eligible_count; ++j) {
            Expert *a = eligible[i], *b = eligible[j];
            int da = abs(a->timezone - chair->timezone);
            int db = abs(b->timezone - chair->timezone);
            int better = b->education > a->education ||
                (b->education == a->education && b->experience > a->experience) ||
                (b->education == a->education && b->experience == a->experience && db < da);
            if (better) {
                eligible[i] = b;
                eligible[j] = a;
            }
        }
    }
    if ((size_t)needed < eligible_count) eligible_count = (size_t)needed;

    /* The required output ordering is education, experience, then ID. */
    qsort(eligible, eligible_count, sizeof(*eligible), compare_selected);
    print_expert(chair, column);
    for (size_t i = 0; i < eligible_count; ++i)
        print_expert(eligible[i], column);

    free(eligible);
    for (size_t i = 0; i < count; ++i) free(experts[i].raw);
    free(experts);
    free(criteria_line);
    free(header_line);
    return 0;

fail:
    if (experts) {
        for (size_t i = 0; i < count; ++i) free(experts[i].raw);
    }
    free(experts);
    free(criteria_line);
    free(header_line);
    return 1;
}
