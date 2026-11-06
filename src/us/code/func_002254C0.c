#include "structs/vector.h"
#include "globals/scene.h"

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0028C6B0.c, func_0028C6B0.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/world.h"
#include "structs/light.h"
void CScene__FindLightsAt(CScene *lv, CVector3 *a, u8 *buf, s32 *out);
void CScene__ComputeLights(u8 *buf, s32 n, CVector3 *a, CLights1 *out);
s32 *CISet__GetBlock(s32 *tbl, s32 idx);

/* A separate function in Rage Wars; inlined into this one in Turok 2. */
static inline s32 hasTbl(CScene *lv) {
    s32 i;
    for (i = 0; i < lv->m_nSectors; i++) {
        if (CISet__GetBlock(*lv->m_Sectors[i].m_hData, 2)[1] != 0) {
            return 1;
        }
    }
    return 0;
}
void CScene__GetLightsAt(CScene *lv, CVector3 *a1, CLights1 *out) {
    u8 buf[0x28];
    s32 n;
    if (hasTbl(lv) == 0 || gLevelLightsEnabled == 0) {
        *out = gSceneLights;
        return;
    }
    CScene__FindLightsAt(lv, a1, buf, &n);
    CScene__ComputeLights(buf, n, a1, out);
}
