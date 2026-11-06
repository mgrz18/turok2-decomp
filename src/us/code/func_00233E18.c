#include "structs/vector.h"

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_002A41D8.c, func_002A41D8.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef struct V16 {
    f32 x, y, z, w;
} V16;
typedef struct Ent41D8 {
    u8 pad0[0x10];
    CVector3 pos;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
} Ent41D8;
typedef struct Out41D8 {
    V16 a;
    V16 b;
    V16 c;
    V16 d;
} Out41D8;
extern f32 D_800A646C;
void CVector3__Subtract(void *out, void *a, void *b);
void CVector3__Scale(void *out, void *a, f32 s);
void CVector3__Cross(void *out, void *a, void *b);
void CVector3__Normalize(void *v);
void CMtxF__Identity(void *m);
void CMtxF__RotateVector(u8 *p, void *a, void *b);
void Trail_ComputeNodeFrame(Ent41D8 *e0, Ent41D8 *e1, u8 *arg2, Out41D8 *out) {
    V16 v10, v20, v30, v40, v50;
    f32 ty, tz;
    CVector3__Subtract(&v20, &e0->pos, &e1->pos);
    CVector3__Normalize(&v20);
    *(s32 *)&v10.x = 0;
    *(s32 *)&v10.y = 0;
    v10.z = D_800A646C;
    CMtxF__RotateVector(arg2 + 0x18C, &v10, &v30);
    ty = -v30.y;
    tz = -v30.z;
    v30.y = ty;
    v30.z = tz;
    CVector3__Scale(&v40, &v30, v30.x * v20.x + ty * v20.y + tz * v20.z);
    CVector3__Subtract(&v40, &v20, &v40);
    CVector3__Normalize(&v40);
    CVector3__Cross(&v50, &v30, &v40);
    CVector3__Normalize(&v50);
    CVector3__Scale(&v50, &v50, e0->unk1C);
    CVector3__Scale(&v40, &v40, e0->unk20);
    CVector3__Scale(&v30, &v30, e0->unk24);
    CMtxF__Identity(out);
    out->a.x = v50.x;
    out->a.y = v50.y;
    out->a.z = v50.z;
    out->b.x = v40.x;
    out->b.y = v40.y;
    out->b.z = v40.z;
    out->c.x = v30.x;
    out->c.y = v30.y;
    out->c.z = v30.z;
    out->d.x = e0->pos.x;
    out->d.y = e0->pos.y;
    out->d.z = e0->pos.z;
}
