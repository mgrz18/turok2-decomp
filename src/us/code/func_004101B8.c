/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_00400E50.c, func_00400E50.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/junk_types.h"

extern const f32 D_800C097C;
extern const f32 D_800C0980;
extern const f32 D_800C0984;
O01Vec3 func_004101B8(O01SplRec *rec, s32 n, f32 t) {
    O01Vec3 res;
    O01SplRec *p = 0;
    O01SplRec *q = 0;
    s32 i;
    f32 d0;
    f32 d1;
    f32 d2;
    f32 m0;
    f32 m1;
    f32 A;
    f32 B;
    f32 u;
    f32 s;
    f32 h2;
    f32 w;
    f32 h;
    f32 pa;
    f32 qa;
    if (n == 0) {
        res.x = 0;
        res.y = 0;
        res.z = 0;
    } else if (n == 1 || t <= rec[0].t) {
        res.x = rec[0].x;
        res.y = rec[0].y;
        res.z = rec[0].z;
    } else if (rec[n - 1].t <= t) {
        res.x = rec[n - 1].x;
        res.y = rec[n - 1].y;
        res.z = rec[n - 1].z;
    } else {
        for (i = 0; i < n - 1; i++) {
            p = &rec[i];
            q = &rec[i + 1];
            if (p->t <= t && t < q->t) {
                break;
            }
        }
        d0 = 0.0f;
        d1 = 0.0f;
        d2 = 0.0f;
        if (i > 0) {
            d0 = rec[i].arc - rec[i - 1].arc;
        }
        if (i < n - 1) {
            d1 = rec[i + 1].arc - rec[i].arc;
        }
        if (i < n - 2) {
            d2 = rec[i + 2].arc - rec[i + 1].arc;
        }
        m0 = d1;
        if (!(m0 <= d0)) {
            m0 = d0;
        }
        m1 = d2;
        if (!(m1 <= d1)) {
            m1 = d1;
        }
        A = p->arc;
        B = q->arc;
        if (p->t != q->t) {
            u = (t - p->t) / (q->t - p->t);
        } else {
            u = 0.0f;
        }
        s = A * (D_800C097C - u) + B * u + (m0 - (B - A)) * (u - 2.0f * u * u + u * u * u) + (m1 - (B - A)) * (u * u * u - u * u);
        for (i = 0; i < n - 1; i++) {
            p = &rec[i];
            q = &rec[i + 1];
            if (p->arc <= s && s < q->arc) {
                break;
            }
        }
        pa = p->arc;
        qa = q->arc;
        h = qa - pa;
        w = 0.0f;
        if (h != 0.0f) {
            w = (s - pa) / h;
        }
        h2 = h * h;
        res.x = p->x * (D_800C0980 - w) + q->x * w + (p->cx * ((D_800C0980 - w) * (D_800C0980 - w) * (D_800C0980 - w) - (D_800C0980 - w)) + q->cx * (w * w * w - w)) * h2 * D_800C0984;
        res.y = p->y * (D_800C0980 - w) + q->y * w + (p->cy * ((D_800C0980 - w) * (D_800C0980 - w) * (D_800C0980 - w) - (D_800C0980 - w)) + q->cy * (w * w * w - w)) * h2 * D_800C0984;
        res.z = p->z * (D_800C0980 - w) + q->z * w + (p->cz * ((D_800C0980 - w) * (D_800C0980 - w) * (D_800C0980 - w) - (D_800C0980 - w)) + q->cz * (w * w * w - w)) * h2 * D_800C0984;
    }
    return res;
}
