#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define BITS 20
#define SIZE (1 << BITS)

typedef struct {
    int to, next;
    uint32_t w;
} Edge;

int main(void) {
    int n, q;
    if (scanf("%d%d", &n, &q) != 2) return 0;

    int *head = malloc((size_t)n * sizeof(*head));
    Edge *edges = malloc((size_t)(2 * (n - 1)) * sizeof(*edges));
    for (int i = 0; i < n; ++i) head[i] = -1;
    int ec = 0;
    for (int i = 0; i < n - 1; ++i) {
        int u, v;
        uint32_t w;
        scanf("%d%d%u", &u, &v, &w);
        --u;
        --v;
        edges[ec] = (Edge){v, head[u], w};
        head[u] = ec++;
        edges[ec] = (Edge){u, head[v], w};
        head[v] = ec++;
    }

    uint32_t *xor_to_root = calloc((size_t)n, sizeof(*xor_to_root));
    int *parent = malloc((size_t)n * sizeof(*parent));
    int *stack = malloc((size_t)n * sizeof(*stack));
    for (int i = 0; i < n; ++i) parent[i] = -2;
    int top = 0;
    stack[top++] = 0;
    parent[0] = -1;
    while (top) {
        int u = stack[--top];
        for (int e = head[u]; e != -1; e = edges[e].next) {
            int v = edges[e].to;
            if (v == parent[u]) continue;
            parent[v] = u;
            xor_to_root[v] = xor_to_root[u] ^ edges[e].w;
            stack[top++] = v;
        }
    }

    int64_t *freq = calloc(SIZE, sizeof(*freq));
    for (int i = 0; i < n; ++i) ++freq[xor_to_root[i]];
    for (int len = 1; len < SIZE; len <<= 1) {
        for (int start = 0; start < SIZE; start += len << 1) {
            for (int j = 0; j < len; ++j) {
                int64_t a = freq[start + j];
                int64_t b = freq[start + j + len];
                freq[start + j] = a + b;
                freq[start + j + len] = a - b;
            }
        }
    }
    for (int i = 0; i < SIZE; ++i) freq[i] *= freq[i];
    for (int len = 1; len < SIZE; len <<= 1) {
        for (int start = 0; start < SIZE; start += len << 1) {
            for (int j = 0; j < len; ++j) {
                int64_t a = freq[start + j];
                int64_t b = freq[start + j + len];
                freq[start + j] = a + b;
                freq[start + j + len] = a - b;
            }
        }
    }

    int64_t *prefix = malloc((size_t)(SIZE + 1) * sizeof(*prefix));
    prefix[0] = 0;
    for (int x = 0; x < SIZE; ++x) {
        int64_t pairs = (freq[x] / SIZE - (x == 0 ? n : 0)) / 2;
        prefix[x + 1] = prefix[x] + pairs;
    }
    for (int i = 0; i < q; ++i) {
        int k;
        scanf("%d", &k);
        printf("%lld\n", (long long)prefix[k]);
    }

    free(head);
    free(edges);
    free(xor_to_root);
    free(parent);
    free(stack);
    free(freq);
    free(prefix);
    return 0;
}
