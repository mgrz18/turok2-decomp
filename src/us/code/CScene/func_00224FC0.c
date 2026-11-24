#include "structs/vector.h"

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0028C1B0.c, func_0028C1B0.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/intelligence.h"
#include "structs/world.h"
#include "structs/vector.h"
/* protos/scene.h's prototypes of what this file calls (the module defines them all; the
 * merged file declares them from the definitions). */
struct CCamera;
struct CGameObjectInstance;
struct CScene;
struct CSceneRecord;
struct LevelEvent;
void CScene__GetRegionEventRecords(struct CScene *, u16 *, struct CSceneRecord **, s32 *);
s32 BinaryRange(void *array, s32 count, u32 key, s32 *first, s32 *last);
void CScene__GetRegionEventRecords(CScene *lv, u16 *idp, CSceneRecord **outRecs, s32 *outCount) {
    s32 hi;
    s32 lo;
    s32 r; sptr a; s32 b; s32 id;
    id = *idp; a = (sptr) lv->m_pAltRecordIndex; b = lv->m_nAltRecords;
    r = BinaryRange(a, b, id, &lo, &hi);
    if (r != 0) {
        *outRecs = lv->m_pAltRecords + lo;
        *outCount = hi - lo + 1;
    } else {
        *outRecs = 0;
        *outCount = 0;
    }
}
