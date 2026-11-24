/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0028D7A0.c, func_0028D7A0.
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
void CScene__GetEventRecords(struct CScene *, s32, s32, struct CSceneRecord **, s32 *);

s32 BinaryRange(void *array, s32 count, u32 key, s32 *first, s32 *last);
void CScene__GetEventRecords(CScene *lv, s32 which, s32 id, CSceneRecord **outRecs, s32 *outCount) {
    s32 lo;
    s32 hi;
    sptr a;
    s32 b;
    s32 off;
    if (which != 0) {
        a = (sptr) lv->m_pAltRecordIndex;
        b = lv->m_nAltRecords;
    } else {
        a = (sptr) lv->m_pRecordIndex;
        b = lv->m_nRecords;
    }
    if (BinaryRange(a, b, id, &lo, &hi) != 0) {
        off = lo * 0x14;
        if (which != 0) {
            *outRecs = (CSceneRecord *)((u8 *)lv->m_pAltRecords + off);
        } else {
            *outRecs = (CSceneRecord *)((u8 *)lv->m_pRecords + off);
        }
        *outCount = hi - lo + 1;
    } else {
        *outRecs = 0;
        *outCount = 0;
    }
}
