#include "structs/game_object.h"

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0024C1B4.c, func_0024C1B4.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef struct B11Pair {
    s32 a;
    s32 b;
} B11Pair;
typedef struct B11Rec {
    u8 pad0[0x2];
    u16 type;
    u8 pad4[0x4];
    CVector3 pos;
    B11Pair unk14;
} B11Rec;
B11Rec *CAnimationState__GetEvents(s32 a);
s32 CAnimationState__GetEventCount(s32 a);
void CMtxF__TransformPoint(void *m, void *v, CVector3 *out);
void ImpactEvent_Send(CGameObjectInstance *a, CGameObjectInstance *b, u16 c, s32 d, CVector3 e, B11Pair f);
void func_0021F4E8(CGameObjectInstance *a, s32 b) {
    B11Rec *list = CAnimationState__GetEvents(b);
    s32 n = CAnimationState__GetEventCount(b);
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
