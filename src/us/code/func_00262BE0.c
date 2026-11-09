#include "common.h"
#include "structs/game_object.h"
#include "structs/collision.h"
#include "protos/ai.h"


void Turret_Hurt(s32 arg0, CAI *arg1, DamageEvent *arg2) {
    s32 var_v1;

    var_v1 = arg1->m_Health - arg2->damage;
    if (var_v1 < 0) {
        var_v1 = 0;
    }
    arg1->m_Health = var_v1;
    if (var_v1 == 0) {
        CAI__SetMode(arg0, (s32) arg1, 0x40);
    }
}
