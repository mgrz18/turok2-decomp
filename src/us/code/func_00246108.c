#include "structs/vector.h"
#include "structs/game_object.h"

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00216D3C.c, func_00216D3C.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

extern f32 gFrameIncrement;
typedef struct { u8 pad0[4]; CVector3 pos; } B04A; /* pos at 0x4 in Turok 2 (0x8 in Rage Wars) */
typedef struct { u8 pad0[0x78]; s32 unk78; f32 unk7C; s32 unk80; u8 pad84[4]; s32 unk88; } B04B;
typedef struct { CAITarget a; CAITarget b; } B04C;
void AI_GetPathTargetInfo(B04A *, B04B *, s32, CAITarget *);
void AI_GetSightTargetInfo(B04A *, B04B *, s32, CAITarget *);
/* A separate function in Rage Wars; inlined here in Turok 2. */
static inline void fill(CAITarget *d, B04A *a) {
    CVector3 p = a->pos;
    CVector3 z;
    z.x = 0.0f;
    z.y = 0.0f;
    z.z = 0.0f;
    d->m_Kind = 3;
    d->m_pTarget = 0;
    d->m_Angle = 0;
    d->m_vPos = p;
    d->m_vDelta = z;
    d->m_Dist = 0;
    z.y = 0.0f;
    p.y = 0.0f;
    d->m_vFlatPos = p;
    d->m_vFlatDelta = z;
    d->m_FlatDist = 0;
}
void AI_UpdateTargets(B04A *a, B04B *b, B04C *c, s32 f1, s32 f2) {
    f32 t;
    if (b->unk7C > 0.0f) {
        t = b->unk7C - gFrameIncrement;
        b->unk7C = t;
        if (t <= 0.0f) {
            b->unk78 = 0;
            b->unk7C = 0.0f;
        }
    }
    if (f1 != 0)
        AI_GetPathTargetInfo(a, b, b->unk88, &c->b);
    else
        fill(&c->b, a);
    if (f2 != 0) {
        AI_GetSightTargetInfo(a, b, b->unk80, &c->a);
        return;
    }
    fill(&c->a, a);
}
