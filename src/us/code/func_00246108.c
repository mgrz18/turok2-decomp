#include "structs/vector.h"
#include "structs/game_object.h"
#include "globals/engine.h"

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00216D3C.c, func_00216D3C.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef struct { u8 pad0[4]; CVector3 pos; } B04A; /* pos at 0x4 in Turok 2 (0x8 in Rage Wars) */
typedef struct { CAITarget a; CAITarget b; } B04C;
void AI_GetPathTargetInfo(B04A *, CAI *, sptr, CAITarget *);
void AI_GetSightTargetInfo(B04A *, CAI *, s32, CAITarget *);
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
void AI_UpdateTargets(B04A *a, CAI *b, B04C *c, s32 f1, s32 f2) {
    f32 t;
    if (b->m_TargetOverrideTime > 0.0f) {
        t = b->m_TargetOverrideTime - gFrameIncrement;
        b->m_TargetOverrideTime = t;
        if (t <= 0.0f) {
            b->m_pTargetOverride = 0;
            b->m_TargetOverrideTime = 0.0f;
        }
    }
    if (f1 != 0)
        AI_GetPathTargetInfo(a, b, b->m_pPathTarget, &c->b);
    else
        fill(&c->b, a);
    if (f2 != 0) {
        AI_GetSightTargetInfo(a, b, b->m_pSightTarget, &c->a);
        return;
    }
    fill(&c->a, a);
}
