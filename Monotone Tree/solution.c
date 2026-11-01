#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int left, right, size, priority;
    int64_t num, den;
    int64_t delta, weighted;
    int64_t sum_delta, sum_weighted;
} Node;

static Node *nodes;
static int node_count;
static int *work;
static int work_cap;

static int64_t gcd64(int64_t a, int64_t b) {
    if (a < 0) a = -a;
    while (b) {
        int64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

static int compare_key(int64_t an, int64_t ad, int64_t bn, int64_t bd) {
    __int128 lhs = (__int128)an * bd;
    __int128 rhs = (__int128)bn * ad;
    return lhs < rhs ? -1 : lhs > rhs;
}

static void pull(int x) {
    Node *n = &nodes[x];
    n->size = 1;
    n->sum_delta = n->delta;
    n->sum_weighted = n->weighted;
    if (n->left >= 0) {
        n->size += nodes[n->left].size;
        n->sum_delta += nodes[n->left].sum_delta;
        n->sum_weighted += nodes[n->left].sum_weighted;
    }
    if (n->right >= 0) {
        n->size += nodes[n->right].size;
        n->sum_delta += nodes[n->right].sum_delta;
        n->sum_weighted += nodes[n->right].sum_weighted;
    }
}

static void split(int root, int64_t num, int64_t den, int *a, int *b) {
    if (root < 0) {
        *a = *b = -1;
        return;
    }
    if (compare_key(nodes[root].num, nodes[root].den, num, den) <= 0) {
        *a = root;
        split(nodes[root].right, num, den, &nodes[root].right, b);
        pull(root);
    } else {
        *b = root;
        split(nodes[root].left, num, den, a, &nodes[root].left);
        pull(root);
    }
}

static int insert_node(int root, int item) {
    if (root < 0) return item;
    int cmp = compare_key(nodes[item].num, nodes[item].den,
                          nodes[root].num, nodes[root].den);
    if (cmp == 0) {
        nodes[root].delta += nodes[item].delta;
        nodes[root].weighted += nodes[item].weighted;
        pull(root);
        return root;
    }
    if (cmp < 0) {
        nodes[root].left = insert_node(nodes[root].left, item);
        if (nodes[nodes[root].left].priority > nodes[root].priority) {
            int top = nodes[root].left;
            nodes[root].left = nodes[top].right;
            nodes[top].right = root;
            pull(root);
            pull(top);
            return top;
        }
    } else {
        nodes[root].right = insert_node(nodes[root].right, item);
        if (nodes[nodes[root].right].priority > nodes[root].priority) {
            int top = nodes[root].right;
            nodes[root].right = nodes[top].left;
            nodes[top].left = root;
            pull(root);
            pull(top);
            return top;
        }
    }
    pull(root);
    return root;
}

static uint32_t random_state = 712367821u;
static uint32_t next_random(void) {
    random_state ^= random_state << 13;
    random_state ^= random_state >> 17;
    random_state ^= random_state << 5;
    return random_state;
}

static int make_event(int64_t num, int64_t den, int64_t delta,
                      int64_t weighted) {
    int id = node_count++;
    nodes[id] = (Node){-1, -1, 1, (int)next_random(), num, den,
                       delta, weighted, delta, weighted};
    return id;
}

static void move_nodes(int root, int *destination) {
    if (root < 0) return;
    int top = 0;
    work[top++] = root;
    while (top) {
        int x = work[--top];
        int left = nodes[x].left, right = nodes[x].right;
        if (left >= 0) work[top++] = left;
        if (right >= 0) work[top++] = right;
        nodes[x].left = nodes[x].right = -1;
        nodes[x].size = 1;
        nodes[x].sum_delta = nodes[x].delta;
        nodes[x].sum_weighted = nodes[x].weighted;
        *destination = insert_node(*destination, x);
    }
}

static void find_minimum(int root, int64_t a, int64_t weight,
                         int64_t *out_num, int64_t *out_den,
                         int64_t *active_delta, int64_t *active_weighted) {
    int cur = root;
    int64_t before_delta = 0, before_weighted = 0;
    while (cur >= 0) {
        int left = nodes[cur].left;
        int64_t ld = left < 0 ? 0 : nodes[left].sum_delta;
        int64_t lb = left < 0 ? 0 : nodes[left].sum_weighted;
        __int128 lhs = (__int128)(weight + before_delta + ld) * nodes[cur].num;
        __int128 rhs = (__int128)(weight * a + before_weighted + lb) *
                       nodes[cur].den;
        if (lhs >= rhs) {
            cur = left;
        } else {
            before_delta += ld + nodes[cur].delta;
            before_weighted += lb + nodes[cur].weighted;
            cur = nodes[cur].right;
        }
    }

    __int128 num = (__int128)weight * a + before_weighted;
    int64_t den = weight + before_delta;
    int64_t reduced_num = (int64_t)num;
    int64_t g = gcd64(reduced_num, den);
    *out_num = reduced_num / g;
    *out_den = den / g;
    *active_delta = before_delta;
    *active_weighted = before_weighted;
}

int main(void) {
    int n;
    if (scanf("%d", &n) != 1) return 0;
    int *first_child = malloc((size_t)n * sizeof(*first_child));
    int *next_sibling = malloc((size_t)n * sizeof(*next_sibling));
    int *parent_node = malloc((size_t)n * sizeof(*parent_node));
    int64_t *value = malloc((size_t)n * sizeof(*value));
    int64_t *weight = malloc((size_t)n * sizeof(*weight));
    int *order = malloc((size_t)n * sizeof(*order));
    int *stack = malloc((size_t)n * sizeof(*stack));
    int *profile = malloc((size_t)n * sizeof(*profile));
    int64_t *best_num = malloc((size_t)n * sizeof(*best_num));
    int64_t *best_den = malloc((size_t)n * sizeof(*best_den));
    nodes = malloc((size_t)n * sizeof(*nodes));
    work = malloc((size_t)n * sizeof(*work));
    work_cap = n;

    for (int i = 0; i < n; ++i) first_child[i] = -1;
    for (int v = 0; v < n; ++v) {
        int parent;
        scanf("%d%lld%lld", &parent, &value[v], &weight[v]);
        if (v > 0) {
            --parent;
            parent_node[v] = parent;
            next_sibling[v] = first_child[parent];
            first_child[parent] = v;
        } else {
            parent_node[v] = -1;
            next_sibling[v] = -1;
        }
        profile[v] = -1;
    }

    int order_count = 0, top = 0;
    stack[top++] = 0;
    while (top) {
        int u = stack[--top];
        order[order_count++] = u;
        for (int v = first_child[u]; v >= 0; v = next_sibling[v])
            stack[top++] = v;
    }

    for (int oi = order_count - 1; oi >= 0; --oi) {
        int u = order[oi];
        int largest = -1;
        for (int v = first_child[u]; v >= 0; v = next_sibling[v]) {
            if (largest < 0 ||
                (profile[v] >= 0 && nodes[profile[v]].size >
                                      (profile[largest] < 0 ? 0 :
                                       nodes[profile[largest]].size)))
                largest = v;
        }

        int root = largest < 0 ? -1 : profile[largest];
        for (int v = first_child[u]; v >= 0; v = next_sibling[v]) {
            if (v != largest && profile[v] >= 0)
                move_nodes(profile[v], &root);
        }

        int64_t active_delta, active_weighted;
        find_minimum(root, value[u], weight[u], &best_num[u], &best_den[u],
                     &active_delta, &active_weighted);

        int left, right;
        split(root, best_num[u], best_den[u], &left, &right);
        int64_t slope = weight[u] + (left < 0 ? 0 : nodes[left].sum_delta);
        __int128 weighted_num = (__int128)slope * best_num[u];
        if (weighted_num % best_den[u] != 0) return 1;
        int64_t weighted = (int64_t)(weighted_num / best_den[u]);
        int event = make_event(best_num[u], best_den[u], slope, weighted);
        profile[u] = insert_node(right, event);
    }

    int64_t *answer_num = malloc((size_t)n * sizeof(*answer_num));
    int64_t *answer_den = malloc((size_t)n * sizeof(*answer_den));
    answer_num[0] = best_num[0];
    answer_den[0] = best_den[0];
    for (int oi = 1; oi < order_count; ++oi) {
        int v = order[oi];
        int parent = parent_node[v];
        if (compare_key(best_num[v], best_den[v],
                        answer_num[parent], answer_den[parent]) < 0) {
            answer_num[v] = answer_num[parent];
            answer_den[v] = answer_den[parent];
        } else {
            answer_num[v] = best_num[v];
            answer_den[v] = best_den[v];
        }
    }

    for (int i = 0; i < n; ++i)
        printf("%lld %lld\n", (long long)answer_num[i],
               (long long)answer_den[i]);

    free(first_child);
    free(next_sibling);
    free(parent_node);
    free(value);
    free(weight);
    free(order);
    free(stack);
    free(profile);
    free(best_num);
    free(best_den);
    free(answer_num);
    free(answer_den);
    free(nodes);
    free(work);
    return 0;
}
