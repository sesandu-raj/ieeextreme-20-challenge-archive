#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXK 8
static int K, M, initial_cash, bonus;
static int value[MAXK + 1], won[MAXK + 1], seen[MAXK + 1];
static int total_rounds;
static int parse_rule(const char *s) { return strcmp(s, "second") == 0; }

int main(void) {
    char line[512], rule[32];
    if (!fgets(line, sizeof line, stdin)) return 0;
    if (sscanf(line, "GAME %d %d %d %d %31s", &K, &M, &initial_cash, &bonus, rule) != 5 ||
        K < 1 || K > MAXK || M < 1 || M > 8) return 0;
    total_rounds = K * M;
    if (!fgets(line, sizeof line, stdin)) return 0;
    char *p = line;
    if (strncmp(p, "VALUES", 6) != 0) return 0;
    p += 6;
    for (int i = 1; i <= K; ++i) {
        while (*p == ' ' || *p == '\t') ++p;
        value[i] = (int)strtol(p, &p, 10);
    }

    while (fgets(line, sizeof line, stdin)) {
        if (strncmp(line, "GAME_END", 8) == 0 || strncmp(line, "-1", 2) == 0) break;
        int round_no, good, cash, opponent_cash;
        if (sscanf(line, "ROUND %d %d %d %d", &round_no, &good, &cash, &opponent_cash) != 4 ||
            good < 1 || good > K) continue;

        int need_after = M - won[good] - 1;
        int auctions_after = M - seen[good] - 1;
        double marginal = value[good];
        if (need_after == 0 && auctions_after == 0) {
            marginal += (double)M * value[good] * bonus / 100.0;
        } else if (need_after > 0 && auctions_after >= need_after) {
            /* The bonus premium is discounted by a heuristic chance of completing the set. */
            double completion_chance = 1.0;
            for (int i = 0; i < need_after; ++i) completion_chance *= 0.5;
            marginal += completion_chance * (double)M * value[good] * bonus / 100.0;
        }

        int remaining = total_rounds - round_no + 1;
        double average_value = 0.0;
        for (int i = 1; i <= K; ++i) average_value += value[i];
        average_value /= K;
        double cash_density = average_value > 0 && remaining > 0
            ? cash / (average_value * (double)remaining) : 1.0;
        int bid;
        if (parse_rule(rule)) {
            bid = (int)(marginal + 0.5);
        } else {
            double factor = 0.60 + 0.15 * cash_density;
            if (factor < 0.52) factor = 0.52;
            if (factor > 0.78) factor = 0.78;
            bid = (int)(marginal * factor + 0.5);
        }
        if (bid < 0) bid = 0;
        if (bid > cash) bid = cash;
        printf("BID %d\n", bid);
        fflush(stdout);

        if (!fgets(line, sizeof line, stdin)) return 0;
        int won_this, opp_bid, price;
        if (sscanf(line, "RESULT %d %d %d", &won_this, &opp_bid, &price) == 3) {
            seen[good]++;
            if (won_this) won[good]++;
        }
    }
    return 0;
}
