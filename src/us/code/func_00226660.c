/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0028D7A0.c, func_0028D7A0.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/world.h"

s32 BinaryRange(s32 a, s32 b, s32 c, s32 *lo, s32 *hi);
void CScene__GetEventRecords(CWorld *lv, s32 which, s32 id, CWorldRecord **outRecs, s32 *outCount) {
    s32 lo;
    s32 hi;
    s32 a;
    s32 b;
    s32 off;
    if (which != 0) {
        a = (s32) lv->m_pAltRecordIndex;
        b = lv->m_nAltRecords;
    } else {
        a = (s32) lv->m_pRecordIndex;
        b = lv->m_nRecords;
    }
    if (BinaryRange(a, b, id, &lo, &hi) != 0) {
        off = lo * 0x14;
        if (which != 0) {
            *outRecs = (CWorldRecord *)((u8 *)lv->m_pAltRecords + off);
        } else {
            *outRecs = (CWorldRecord *)((u8 *)lv->m_pRecords + off);
        }
        *outCount = hi - lo + 1;
    } else {
        *outRecs = 0;
        *outCount = 0;
    }
}
