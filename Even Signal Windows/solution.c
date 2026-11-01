#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    int n, q;
    if (scanf("%d %d", &n, &q) != 2) return 0;

    char *s = malloc((size_t)n + 5);
    long long *even = calloc((size_t)n + 2, sizeof(*even));
    long long *odd = calloc((size_t)n + 2, sizeof(*odd));
    if (!s || !even || !odd) {
        free(s);
        free(even);
        free(odd);
        return 1;
    }
    if (scanf("%s", s) != 1) {
        free(s);
        free(even);
        free(odd);
        return 1;
    }

    int parity = 0;
    for (int i = 0; i <= n; ++i) {
        if (i > 0 && s[i - 1] == '1') parity ^= 1;
        even[i] = (i ? even[i - 1] : 0) + (parity == 0);
        odd[i] = (i ? odd[i - 1] : 0) + (parity == 1);
    }

    for (int qi = 0; qi < q; ++qi) {
        int l, r;
        if (scanf("%d %d", &l, &r) != 2) break;
        int left = l - 1, right = r;
        long long e = even[right] - (left ? even[left - 1] : 0);
        long long o = odd[right] - (left ? odd[left - 1] : 0);
        printf("%lld\n", e * (e - 1) / 2 + o * (o - 1) / 2);
    }

    free(s);
    free(even);
    free(odd);
    return 0;
}
