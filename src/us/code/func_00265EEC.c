#include "common.h"
#include "structs/game_object.h"
#include "structs/collision.h"
#include "protos/ai.h"


void Destructible_Hurt(CGameObjectInstance *arg0, CAI *arg1, DamageEvent *arg2) {
    s32 temp_v1;
    s32 var_a2;
    s32 var_a3;
    s32 var_v1;

    temp_v1 = arg0->header.m_pIntelligence->m_dwTypeFlags;
    var_a3 = 0;
    if (temp_v1 & 1) {
        var_a3 = (((u32) arg2->flags >> 0x16) ^ 1) & 1;
    }
    if ((temp_v1 & 2) && (arg2->flags & 0x400000)) {
        var_a3 = 1;
    }
    if (var_a3 != 0) {
        var_v1 = arg1->m_Health - arg2->damage;
        if (var_v1 < 0) {
            var_v1 = 0;
        }
        arg1->m_Health = var_v1;
        if (var_v1 == 0) {
            var_a2 = 1;
            if (arg0->header.m_pIntelligence->m_dwTypeFlags & 0x40) {
                var_a2 = 2;
            }
            CAI__SetMode(arg0, arg1, var_a2);
        }
    }
}
