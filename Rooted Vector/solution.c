#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef long long LL;
typedef __int128 I128;

typedef struct { LL x, y; } Pt;
typedef struct { Pt *pts; int n; } Poly;

static inline I128 cross3(Pt o, Pt a, Pt b) {
    return (I128)(a.x - o.x) * (b.y - o.y) - (I128)(a.y - o.y) * (b.x - o.x);
}

static inline int pt_eq(Pt a, Pt b) {
    return a.x == b.x && a.y == b.y;
}

static int cmp_pt(const void *a, const void *b) {
    Pt pa = *(const Pt *)a;
    Pt pb = *(const Pt *)b;
    if (pa.x != pb.x) return pa.x < pb.x ? -1 : 1;
    if (pa.y != pb.y) return pa.y < pb.y ? -1 : 1;
    return 0;
}

static Poly convex_hull(Pt *pts, int m) {
    Poly out = {0};
    if (m <= 0) {
        out.pts = NULL;
        out.n = 0;
        return out;
    }
    qsort(pts, m, sizeof(Pt), cmp_pt);
    Pt *uniq = malloc(sizeof(Pt) * m);
    int k = 0;
    for (int i = 0; i < m; ++i) {
        if (k == 0 || !pt_eq(uniq[k - 1], pts[i])) {
            uniq[k++] = pts[i];
        }
    }
    if (k == 1) {
        out.pts = malloc(sizeof(Pt));
        out.pts[0] = uniq[0];
        out.n = 1;
        free(uniq);
        return out;
    }
    Pt *res = malloc(sizeof(Pt) * (2 * k + 5));
    int len = 0;
    for (int i = 0; i < k; ++i) {
        while (len >= 2 && cross3(res[len - 2], res[len - 1], uniq[i]) <= 0) {
            --len;
        }
        res[len++] = uniq[i];
    }
    int lower = len;
    for (int i = k - 2; i >= 0; --i) {
        while (len >= lower + 1 && cross3(res[len - 2], res[len - 1], uniq[i]) <= 0) {
            --len;
        }
        res[len++] = uniq[i];
    }
    if (len > 1 && pt_eq(res[0], res[len - 1])) {
        --len;
    }
    out.pts = malloc(sizeof(Pt) * len);
    out.n = len;
    for (int i = 0; i < len; ++i) out.pts[i] = res[i];
    free(res);
    free(uniq);
    return out;
}

static Poly translate_poly(Poly p, Pt delta) {
    Poly out = {malloc(sizeof(Pt) * p.n), p.n};
    for (int i = 0; i < p.n; ++i) {
        out.pts[i].x = p.pts[i].x + delta.x;
        out.pts[i].y = p.pts[i].y + delta.y;
    }
    return out;
}

static Poly minkowski_sum(Poly a, Poly b) {
    Poly out = {0};
    if (a.n == 0 || b.n == 0) return out;
    if (a.n == 1) return translate_poly(b, a.pts[0]);
    if (b.n == 1) return translate_poly(a, b.pts[0]);

    Pt *all = malloc(sizeof(Pt) * (size_t)a.n * (size_t)b.n);
    int idx = 0;
    for (int i = 0; i < a.n; ++i) {
        for (int j = 0; j < b.n; ++j) {
            all[idx++] = (Pt){a.pts[i].x + b.pts[j].x, a.pts[i].y + b.pts[j].y};
        }
    }

    Poly hull = convex_hull(all, idx);
    free(all);
    return hull;
}

static void free_poly(Poly p) {
    free(p.pts);
}

static void print_i128(I128 x) {
    if (x < 0) {
        putchar('-');
        x = -x;
    }
    char buf[64];
    int len = 0;
    do {
        buf[len++] = (char)('0' + (x % 10));
        x /= 10;
    } while (x > 0);
    while (len-- > 0) putchar(buf[len]);
}

int main(void) {
    int n;
    if (scanf("%d", &n) != 1) return 0;

    int *children = calloc((size_t)n + 1, sizeof(int));
    int *head = calloc((size_t)n + 1, sizeof(int));
    int *next = calloc((size_t)n + 1, sizeof(int));
    int *cnt = calloc((size_t)n + 1, sizeof(int));

    Pt *vec = malloc(sizeof(Pt) * (n + 1));
    int *parent = malloc(sizeof(int) * (n + 1));
    for (int v = 1; v <= n; ++v) {
        int p;
        LL a, b;
        scanf("%d %lld %lld", &p, &a, &b);
        parent[v] = p;
        vec[v].x = a;
        vec[v].y = b;
        if (p != 0) {
            next[v] = head[p];
            head[p] = v;
            cnt[p]++;
        }
    }

    Poly *polys = calloc((size_t)n + 1, sizeof(Poly));
    int *order = malloc(sizeof(int) * (n + 1));
    int ordsz = 0;
    int st[n + 1];
    int top = 0;
    st[top++] = 1;
    while (top > 0) {
        int u = st[--top];
        order[ordsz++] = u;
        for (int v = head[u]; v != 0; v = next[v]) {
            st[top++] = v;
        }
    }

    for (int i = ordsz - 1; i >= 0; --i) {
        int u = order[i];
        Poly cur = {malloc(sizeof(Pt)), 1};
        cur.pts[0] = vec[u];

        for (int v = head[u]; v != 0; v = next[v]) {
            Poly child = polys[v];
            Pt *tmp = malloc(sizeof(Pt) * (child.n + 2));
            for (int k = 0; k < child.n; ++k) tmp[k] = child.pts[k];
            tmp[child.n] = (Pt){0, 0};
            Poly union_poly = convex_hull(tmp, child.n + 1);
            free(tmp);
            Poly merged = minkowski_sum(cur, union_poly);
            free_poly(cur);
            free_poly(union_poly);
            cur = merged;
        }
        polys[u] = cur;
    }

    I128 ans = 0;
    for (int i = 0; i < polys[1].n; ++i) {
        I128 val = (I128)polys[1].pts[i].x * polys[1].pts[i].x + (I128)polys[1].pts[i].y * polys[1].pts[i].y;
        if (val > ans) ans = val;
    }

    print_i128(ans);
    putchar('\n');

    free(children);
    free(head);
    free(next);
    free(cnt);
    free(vec);
    free(parent);
    free(order);
    for (int v = 1; v <= n; ++v) {
        free(polys[v].pts);
    }
    free(polys);
    return 0;
}
