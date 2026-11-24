#include "structs/vector.h"

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0028CE70.c, func_0028CE70.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "common.h"
#include "structs/world.h"
#include "structs/vector.h"
/* protos/scene.h's prototypes of what this file calls (the module defines them all; the
 * merged file declares them from the definitions). */
struct CCamera;
struct CGameObjectInstance;
struct CScene;
struct CSceneRecord;
struct LevelEvent;
void CScene__StartRumbles(struct CScene *, sptr, s32, CVector3, f32, f32);
s32 BinaryRange(void *array, s32 count, u32 key, s32 *first, s32 *last);
void Rumble_Start(sptr a, u8 *p, CVector3 v, f32 f, f32 g, sptr h);
void CScene__StartRumbles(CScene *lv, sptr a1, s32 id, CVector3 v, f32 f6, f32 f7) {
    s32 lo;
    s32 hi;
    s32 i;
    s32 *t = lv->m_pRumbleTypes;
    s32 n = t[1];
    if (BinaryRange(t + 2, n, id, &lo, &hi) != 0) {
        i = lo;
        if (hi >= i) {
            do {
                s32 *tbl = lv->m_pRumbles;
                Rumble_Start(a1, (u8 *)tbl + (i * tbl[0] + 8), v, f6, f7, 0);
                i++;
            } while (hi >= i);
        }
    }
}
