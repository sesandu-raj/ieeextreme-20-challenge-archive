#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MOD 998244353LL

typedef struct {
    int id, next;
} Event;

static int64_t mod_pow(int64_t a, int64_t e) {
    int64_t r = 1;
    while (e) {
        if (e & 1) r = r * a % MOD;
        a = a * a % MOD;
        e >>= 1;
    }
    return r;
}

int main(void) {
    int n, m;
    if (scanf("%d%d", &n, &m) != 2) return 0;

    int *head = malloc((size_t)(m + 2) * sizeof(*head));
    Event *events = malloc((size_t)(2 * n) * sizeof(*events));
    int64_t *prob = malloc((size_t)n * sizeof(*prob));
    int64_t *absence = malloc((size_t)n * sizeof(*absence));
    for (int i = 0; i <= m + 1; ++i) head[i] = -1;

    int ec = 0;
    for (int i = 0; i < n; ++i) {
        int l, r;
        int64_t p, q;
        scanf("%d%d%lld%lld", &l, &r, &p, &q);
        prob[i] = p % MOD * mod_pow(q % MOD, MOD - 2) % MOD;
        absence[i] = (1 - prob[i] + MOD) % MOD;
        events[ec] = (Event){i, head[l]};
        head[l] = ec++;
        events[ec] = (Event){~i, head[r + 1]};
        head[r + 1] = ec++;
    }

    int forced = 0;
    int64_t product = 1, ratio_sum = 0;
    for (int cell = 1; cell <= m; ++cell) {
        for (int e = head[cell]; e != -1; e = events[e].next) {
            int raw = events[e].id;
            int add = raw >= 0;
            int id = add ? raw : ~raw;
            int64_t p = prob[id], q = absence[id];
            if (p == 0) continue;
            if (q == 0) {
                forced += add ? 1 : -1;
            } else if (add) {
                product = product * q % MOD;
                ratio_sum = (ratio_sum + p * mod_pow(q, MOD - 2)) % MOD;
            } else {
                product = product * mod_pow(q, MOD - 2) % MOD;
                ratio_sum = (ratio_sum - p * mod_pow(q, MOD - 2)) % MOD;
                if (ratio_sum < 0) ratio_sum += MOD;
            }
        }

        int64_t ans;
        if (forced >= 2) ans = 0;
        else if (forced == 1) ans = product;
        else ans = product * (1 + ratio_sum) % MOD;
        printf("%lld%c", (long long)ans, cell == m ? '\n' : ' ');
    }

    free(head);
    free(events);
    free(prob);
    free(absence);
    return 0;
}
