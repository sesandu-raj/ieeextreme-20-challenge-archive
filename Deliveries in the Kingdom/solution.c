#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int to, next;
} Edge;

static int ancestor(int u, int v, const int *tin, const int *tout) {
    return tin[u] <= tin[v] && tin[v] <= tout[u];
}

int main(void) {
    int n, m;
    if (scanf("%d%d", &n, &m) != 2) return 0;
    int *head = malloc((size_t)n * sizeof(*head));
    Edge *edges = malloc((size_t)(2 * m) * sizeof(*edges));
    for (int i = 0; i < n; ++i) head[i] = -1;
    int ec = 0;
    for (int i = 0; i < m; ++i) {
        int a, b;
        scanf("%d%d", &a, &b);
        --a;
        --b;
        edges[ec] = (Edge){b, head[a]};
        head[a] = ec++;
        edges[ec] = (Edge){a, head[b]};
        head[b] = ec++;
    }

    int *tin = calloc((size_t)n, sizeof(*tin));
    int *tout = malloc((size_t)n * sizeof(*tout));
    int *low = malloc((size_t)n * sizeof(*low));
    int *parent = malloc((size_t)n * sizeof(*parent));
    int *parent_edge = malloc((size_t)n * sizeof(*parent_edge));
    int *iter = malloc((size_t)n * sizeof(*iter));
    int *depth = malloc((size_t)n * sizeof(*depth));
    int *component = malloc((size_t)n * sizeof(*component));
    int *child_count = calloc((size_t)n, sizeof(*child_count));
    int *articulation = calloc((size_t)n, sizeof(*articulation));
    int *stack = malloc((size_t)n * sizeof(*stack));

    int timer = 0, component_id = 0;
    for (int start = 0; start < n; ++start) {
        if (tin[start]) continue;
        int top = 0;
        parent[start] = -1;
        parent_edge[start] = -1;
        depth[start] = 0;
        component[start] = component_id++;
        tin[start] = low[start] = ++timer;
        iter[start] = head[start];
        stack[top++] = start;

        while (top) {
            int u = stack[top - 1];
            if (iter[u] != -1) {
                int e = iter[u];
                iter[u] = edges[e].next;
                if (e == parent_edge[u]) continue;
                int v = edges[e].to;
                if (!tin[v]) {
                    parent[v] = u;
                    parent_edge[v] = e ^ 1;
                    depth[v] = depth[u] + 1;
                    component[v] = component[u];
                    ++child_count[u];
                    tin[v] = low[v] = ++timer;
                    iter[v] = head[v];
                    stack[top++] = v;
                } else if (tin[v] < low[u]) {
                    low[u] = tin[v];
                }
            } else {
                --top;
                tout[u] = timer;
                int p = parent[u];
                if (p >= 0) {
                    if (low[u] < low[p]) low[p] = low[u];
                    if (parent[p] >= 0 && low[u] >= tin[p])
                        articulation[p] = 1;
                } else if (child_count[u] > 1) {
                    articulation[u] = 1;
                }
            }
        }
    }

    int log = 1;
    while ((1 << log) <= n) ++log;
    int *up = malloc((size_t)log * n * sizeof(*up));
    for (int v = 0; v < n; ++v) up[v] = parent[v] < 0 ? v : parent[v];
    for (int j = 1; j < log; ++j) {
        for (int v = 0; v < n; ++v)
            up[(size_t)j * n + v] =
                up[(size_t)(j - 1) * n + up[(size_t)(j - 1) * n + v]];
    }

    int q;
    scanf("%d", &q);
    for (int qi = 0; qi < q; ++qi) {
        int source, destination, forbidden;
        scanf("%d%d%d", &source, &destination, &forbidden);
        --source;
        --destination;
        --forbidden;

        int ok = 0;
        if (source != forbidden && destination != forbidden &&
            component[source] == component[destination]) {
            if (source == destination || !articulation[forbidden] ||
                component[source] != component[forbidden]) {
                ok = 1;
            } else {
                int branch_source = -1, branch_destination = -1;
                if (ancestor(forbidden, source, tin, tout)) {
                    int steps = depth[source] - depth[forbidden] - 1;
                    int v = source;
                    for (int j = 0; j < log; ++j)
                        if ((steps >> j) & 1)
                            v = up[(size_t)j * n + v];
                    if (low[v] >= tin[forbidden]) branch_source = v;
                }
                if (ancestor(forbidden, destination, tin, tout)) {
                    int steps = depth[destination] - depth[forbidden] - 1;
                    int v = destination;
                    for (int j = 0; j < log; ++j)
                        if ((steps >> j) & 1)
                            v = up[(size_t)j * n + v];
                    if (low[v] >= tin[forbidden]) branch_destination = v;
                }
                ok = branch_source == branch_destination;
            }
        }
        puts(ok ? "Sim" : "Nao");
    }

    free(head);
    free(edges);
    free(tin);
    free(tout);
    free(low);
    free(parent);
    free(parent_edge);
    free(iter);
    free(depth);
    free(component);
    free(child_count);
    free(articulation);
    free(stack);
    free(up);
    return 0;
}
