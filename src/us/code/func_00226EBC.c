#include "structs/vector.h"

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0028E83C.c, func_0028E83C.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/intelligence.h"
#include "structs/game_object.h"

void func_0020EF2C(CVector3 *out, CVector3 *a, CVector3 *b);
s32 func_00226EBC(CGameObjectInstance *e, CVector3 *target) {
    CVector3 d;
    f32 dist;
    u32 t;
    if (*(u8 *)e == 1 && e->header.m_pIntelligence->m_Type == 7) {
        func_0020EF2C(&d, &e->header.m_vPos, target);
        dist = d.x * d.x + d.z * d.z;
        d.y = 0;
        if (262144.0f < dist) {
            return 0;
        }
        return 0xFF;
    }
    if (e->header.m_pIntelligence->m_Type == 1) {
        t = e->m_nTypeFlag;
        if (t == 0x44E || t == 0x453 || t == 0x451 || t == 0x450 || t == 0x454 || t == 0x455 || t == 0x456) {
            return 0xFF;
        }
    }
    return 0;
}
