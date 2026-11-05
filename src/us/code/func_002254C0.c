#include "structs/vector.h"

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0028C6B0.c, func_0028C6B0.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/world.h"
#include "structs/light.h"
void func_002228E4(CWorld *lv, CVector3 *a, u8 *buf, s32 *out);
void func_00222A9C(u8 *buf, s32 n, CVector3 *a, CLights1 *out);
extern s32 D_800ACAE0;
s32 *func_002017D4(s32 *tbl, s32 idx);

/* A separate function in Rage Wars; inlined into this one in Turok 2. */
static inline s32 hasTbl(CWorld *lv) {
    s32 i;
    for (i = 0; i < lv->m_nSectors; i++) {
        if (func_002017D4(*lv->m_Sectors[i].m_hData, 2)[1] != 0) {
            return 1;
        }
    }
    return 0;
}
void func_002254C0(CWorld *lv, CVector3 *a1, CLights1 *out) {
    u8 buf[0x28];
    s32 n;
    if (hasTbl(lv) == 0 || D_800ACAE0 == 0) {
        *out = D_800AB0D0;
        return;
    }
    func_002228E4(lv, a1, buf, &n);
    func_00222A9C(buf, n, a1, out);
}
