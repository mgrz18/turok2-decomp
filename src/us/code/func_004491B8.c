/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU_2/80400000_4.CPP, func_80400E50_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
typedef float f32;
struct CVector3_t;
struct CVector3_t;
typedef struct CVector3_t CVector3;
struct CVector3_t;
struct CVector3_t {
    f32 x;
    f32 y;
    f32 z;
};
struct SplineNode;
struct SplineNode {
    CVector3 p;
    CVector3 d2p;
    f32 s;
    f32 w;
    f32 d2s;
};
struct SplineNode;
typedef struct SplineNode SplineNode;
CVector3 func_004491B8(SplineNode *nodes, s32 count, f32 w) {
    CVector3 out;
    SplineNode *a;
    SplineNode *b;
    s32 i;
    f32 before;
    f32 across;
    f32 after;
    f32 inSlope;
    f32 outSlope;
    f32 u;
    f32 aw;
    f32 bw;
    f32 as;
    f32 bs;
    f32 s;
    f32 start;
    f32 h;
    f32 hi;
    f32 lo;
    f32 h2;
    a = 0;
    b = 0;
    if (count == 0) {
        out.x = 0.0f;
        out.y = 0.0f;
        out.z = 0.0f;
    } else if (count == 1 || w <= nodes[0].w) {
        out.x = nodes[0].p.x;
        out.y = nodes[0].p.y;
        out.z = nodes[0].p.z;
    } else if (nodes[count - 1].w <= w) {
        out.x = nodes[count - 1].p.x;
        out.y = nodes[count - 1].p.y;
        out.z = nodes[count - 1].p.z;
    } else {
        for (i = 0; i < count - 1; i++) {
            a = &nodes[i];
            b = &nodes[i + 1];
            if (a->w <= w && w < b->w) {
                break;
            }
        }
        across = 0.0f;
        before = 0.0f;
        after = 0.0f;
        if (i > 0) {
            before = nodes[i].s - nodes[i - 1].s;
        }
        if (i < count - 1) {
            across = nodes[i + 1].s - nodes[i].s;
        }
        if (i < count - 2) {
            after = nodes[i + 2].s - nodes[i + 1].s;
        }
        inSlope = across;
        if (!(inSlope <= before)) {
            inSlope = before;
        }
        outSlope = after;
        if (!(outSlope <= across)) {
            outSlope = across;
        }
        aw = a->w;
        bw = b->w;
        as = a->s;
        bs = b->s;
        if (aw != bw) {
            u = (w - aw) / (bw - aw);
        } else {
            u = 0.0f;
        }
        s = as * (1.0f - u) + bs * u + (inSlope - (bs - as)) * (u - (u + u) * u + u * u * u) + (outSlope - (bs - as)) * (u * u * u - u * u);
        for (i = 0; i < count - 1; i++) {
            a = &nodes[i];
            b = &nodes[i + 1];
            if (a->s <= s && s < b->s) {
                break;
            }
        }
        start = a->s;
        h = b->s - start;
        hi = 0.0f;
        if (h != 0.0f) {
            hi = (s - start) / h;
        }
        h2 = h * h;
        lo = 1.0f - hi;
        out.x = a->p.x * lo + b->p.x * hi + (a->d2p.x * (lo * lo * lo - lo) + b->d2p.x * (hi * hi * hi - hi)) * h2 * 0.16666667f;
        out.y = a->p.y * lo + b->p.y * hi + (a->d2p.y * (lo * lo * lo - lo) + b->d2p.y * (hi * hi * hi - hi)) * h2 * 0.16666667f;
        out.z = a->p.z * lo + b->p.z * hi + (a->d2p.z * (lo * lo * lo - lo) + b->d2p.z * (hi * hi * hi - hi)) * h2 * 0.16666667f;
    }
    return out;
}
