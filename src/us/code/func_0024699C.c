#include "structs/vector.h"

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0021740C.c, func_0021740C.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

f32 GameObject_GetCollisionRadius(void *);
#ifdef TARGET_PORT /* ABI (docs/port/ABI.md, abicheck struct_scalar/float_int) */
#define GameObject_TraceMove GameObject_TraceMove_f3
#endif
void GameObject_TraceMove(void *, f32, f32, f32, s32, s32, s32, s32);
void CVector3__Add(CVector3 *, CVector3 *, CVector3 *);
void CVector3__Subtract(CVector3 *, CVector3 *, CVector3 *);
void CVector3__Scale(CVector3 *, CVector3 *, f32);
void CVector3__Normalize(CVector3 *);
typedef struct { u8 pad0[4]; CVector3 pos; } B04Ob; /* pos at 0x4 in Turok 2 */
void AI_PushAway(B04Ob *a, s32 unused, B04Ob *b, B04Ob *c, s32 e, s32 f) {
    CVector3 d;
    CVector3 r;
    if (c != ((void *)0)) {
        CVector3__Subtract(&d, &b->pos, &c->pos);
        d.y = 0;
        CVector3__Normalize(&d);
    } else {
        d.x = 0;
        d.y = 0;
        d.z = 0;
    }
    CVector3__Scale(&d, &d, GameObject_GetCollisionRadius(b) + GameObject_GetCollisionRadius(a) + 51.199997f);
    CVector3__Add(&r, &b->pos, &d);
    GameObject_TraceMove(b, r.x, r.y, r.z, e, f, 0, 0);
}
