#include "structs/game_object.h"

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0024BCF8.c, func_0024BCF8.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

f32 func_002982F0(f32 x);
f32 func_002974C0(f32 x);
typedef struct B11Pt {
    f32 x;
    f32 y;
} B11Pt;
typedef struct B11Box {
    u16 unk0;
    u16 flags2;
    B11Pt *p4;
    B11Pt *p8;
    B11Pt *pC;
} B11Box;
void CGameRegion__GetNormalRaw(f32 *out, B11Box *box);
s32 func_0021F080(CGameObjectInstance *a) {
    B11Box *b = (B11Box *)a->header.m_pCurrentRegion;
    f32 t;
    f32 hi;
    f32 t2;
    f32 lo;
    f32 s;
    f32 c;
    f32 buf[3];
    if (b == 0) {
        return 0;
    }
    t = b->p8->y;
    if (!(b->p4->y <= t)) {
        t = b->p4->y;
    }
    hi = b->pC->y;
    if (!(t <= hi)) {
        hi = t;
    }
    t2 = b->p8->y;
    if (!(t2 <= b->p4->y)) {
        t2 = b->p4->y;
    }
    lo = b->pC->y;
    if (!(lo <= t2)) {
        lo = t2;
    }
    if (hi < a->header.m_vPos.y) {
        return 0;
    }
    if (a->header.m_vPos.y < lo) {
        return 0;
    }
    s = func_002982F0(a->m_RotY);
    c = func_002974C0(a->m_RotY);
    CGameRegion__GetNormalRaw(buf, b);
    return buf[0] * -s + buf[2] * -c < 0.0f;
}
