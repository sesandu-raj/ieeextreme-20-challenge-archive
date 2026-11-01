#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PLAYERS 10
static int N, P;
static int deck[13], discard[13], round_cards[13];
static unsigned char held[PLAYERS][13], done[PLAYERS];
static int finished_count;

static void reset_round(void) {
    for (int v = 1; v <= 12; ++v) {
        discard[v] += round_cards[v];
        round_cards[v] = 0;
    }
    memset(held, 0, sizeof held);
    memset(done, 0, sizeof done);
    finished_count = 0;
}

static int deck_total(void) {
    int total = 0;
    for (int v = 1; v <= 12; ++v) total += deck[v];
    return total;
}

static void ensure_deck(void) {
    if (deck_total() != 0) return;
    int count = 0;
    for (int v = 1; v <= 12; ++v) count += discard[v];
    if (count) {
        for (int v = 1; v <= 12; ++v) {
            deck[v] = discard[v];
            discard[v] = 0;
        }
    } else {
        for (int v = 1; v <= 12; ++v) deck[v] = v;
    }
}

static void mark_done(int player) {
    if (player >= 0 && player < N && !done[player]) {
        done[player] = 1;
        ++finished_count;
    }
}

static void draw_card(int player, int value) {
    if (player < 0 || player >= N || value < 1 || value > 12) return;
    ensure_deck();
    if (deck[value] > 0) --deck[value];
    round_cards[value]++;
    if (held[player][value]) mark_done(player);
    else held[player][value] = 1;
}

static void finish_action(int player) {
    if (player >= 0 && player < N) mark_done(player);
}

static char *skip_space(char *p) {
    while (*p == ' ' || *p == '\t') ++p;
    return p;
}

int main(void) {
    char line[256];
    if (!fgets(line, sizeof line, stdin)) return 0;
    if (sscanf(line, "%d %d", &N, &P) != 2 || N < 2 || N > PLAYERS || P < 0 || P >= N) return 0;
    for (int v = 1; v <= 12; ++v) deck[v] = v;

    while (fgets(line, sizeof line, stdin)) {
        char *p = skip_space(line);
        if (*p == '-' || strncmp(p, "GAME_END", 8) == 0) break;
        char *end;
        long who = strtol(p, &end, 10);
        if (end == p) continue;
        p = skip_space(end);
        if (*p == '\0' || *p == '\n' || *p == '\r') {
            if (finished_count == N) reset_round();
            if (who != P) continue;
            ensure_deck();
            int total = deck_total();
            long long good_sum = 0;
            for (int v = 1; v <= 12; ++v)
                if (!held[P][v]) good_sum += (long long)deck[v] * v;
            /* The one-step expected-value policy is the specified heuristic objective. */
            int score = 0;
            for (int v = 1; v <= 12; ++v) if (held[P][v]) score += v;
            int draw = total > 0 && good_sum > (long long)total * score;
            printf("%d\n", draw ? 1 : 0);
            fflush(stdout);
            if (draw) {
                if (!fgets(line, sizeof line, stdin)) return 0;
                int card = atoi(skip_space(line));
                draw_card(P, card);
            } else finish_action(P);
            continue;
        }

        if (finished_count == N) reset_round();
        int action = *p - '0';
        ++p;
        p = skip_space(p);
        int player = (int)who;
        if (action == 0) finish_action(player);
        else if (action == 1) {
            int card = (int)strtol(p, &end, 10);
            if (end == p) {
                card = action;
                action = 0;
            } else draw_card(player, card);
        }
    }
    return 0;
}
