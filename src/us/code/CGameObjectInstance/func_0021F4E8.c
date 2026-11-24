#include "structs/game_object.h"

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0024C1B4.c, func_0024C1B4.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef struct AnimEventPair {
    s32 a;
    s32 b;
} AnimEventPair; /* an animation event's value (structs/particle.h CEventValue's two words) */
typedef struct B11Rec {
    u8 pad0[0x2];
    u16 type;
    u8 pad4[0x4];
    CVector3 pos;
    AnimEventPair unk14;
} B11Rec;
void *CAnimationState__GetEvents(void *channel);
s32 CAnimationState__GetEventCount(void *channel);
void CMtxF__TransformPoint(void *m, void *in, void *out);
void ImpactEvent_Send(void *src, void *dst, u16 effect, s32 id, CVector3 pos, AnimEventPair value);
void CGameObjectInstance__SendAnimImpactEvents(CGameObjectInstance *a, s32 b) {
    B11Rec *list = CAnimationState__GetEvents((void *) (sptr) b);
    s32 n = CAnimationState__GetEventCount((void *) (sptr) b);
    s32 i;
    s32 t;
    CVector3 v;
    for (i = 0; i < n; i++) {
        t = list[i].type;
        if (t < 8) {
            if (t >= 6) {
                CMtxF__TransformPoint(a->m_mfOrient, &list[i].pos, &v);
                ImpactEvent_Send(a, a, list[i].type, -1, v, list[i].unk14);
            }
        }
    }
}
