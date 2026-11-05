#include "structs/vector.h"

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0028E768.c, func_0028E768.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/intelligence.h"
#include "structs/game_object.h"

void func_0020EF2C(CVector3 *out, CVector3 *a, CVector3 *b);
s32 func_00226DE8(CGameObjectInstance *e, CVector3 *target) {
    CVector3 d;
    f32 dist;
    u32 t;
    func_0020EF2C(&d, &e->header.m_vPos, target);
    dist = d.x * d.x + d.y * d.y + d.z * d.z;
    if (e->header.m_pIntelligence->m_Type == 1) {
        t = e->m_nTypeFlag;
        if (t == 0x44E || t == 0x453 || t == 0x451 || t == 0x450 || t == 0x454 || t == 0x455 || t == 0x456) {
            return 0;
        }
        if (e->m_AI.m_Health != 0) {
            if (e->m_AI.m_Mode != 0x3D) {
                if (dist <= 6553600.0f) {
                    return 1;
                }
            }
        }
    }
    return 0;
}
