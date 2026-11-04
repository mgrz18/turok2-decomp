#include "common.h"
#include "structs/game_object.h"
#include "structs/collision.h"

s32 func_00243414(s32, s32, s32);

void func_00262BE0(s32 arg0, CAI *arg1, DamageEvent *arg2) {
    s32 var_v1;

    var_v1 = arg1->m_Health - arg2->damage;
    if (var_v1 < 0) {
        var_v1 = 0;
    }
    arg1->m_Health = var_v1;
    if (var_v1 == 0) {
        func_00243414(arg0, (s32) arg1, 0x40);
    }
}
