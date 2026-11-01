#include <stdio.h>
#include <stdlib.h>

static int compare_int64(const void *pa, const void *pb) {
    long long a = *(const long long *)pa;
    long long b = *(const long long *)pb;
    return (a > b) - (a < b);
}

int main(void) {
    int n;
    long long circumference;
    if (scanf("%d %lld", &n, &circumference) != 2) return 0;

    long long *p = malloc((size_t)n * sizeof(*p));
    if (!p) return 1;
    for (int i = 0; i < n; ++i) {
        if (scanf("%lld", &p[i]) != 1) {
            free(p);
            return 1;
        }
    }

    qsort(p, (size_t)n, sizeof(*p), compare_int64);
    long long best_length = -1;
    long long best_start = circumference;

    if (n == 1) {
        best_length = circumference;
        best_start = p[0];
    } else {
        for (int i = 0; i < n; ++i) {
            int next = (i + 1) % n;
            long long length = next ? p[next] - p[i]
                                    : circumference - p[i] + p[0];
            if (length > best_length ||
                (length == best_length && p[i] < best_start)) {
                best_length = length;
                best_start = p[i];
            }
        }
    }

    printf("%lld %lld\n", best_length, best_start);
    free(p);
    return 0;
}
