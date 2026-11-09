#include "common.h"
#include "structs/game_object.h"
#include "globals/scene.h"
#include "protos/scene.h"
#include "protos/ai.h"

s32 Turret_EventShow(s32, void *, s32);        /* extern */

void Turret_EventActivate(s32 arg0, CAI *ai, s32 arg2) {
    if ((CScene__GetInstanceSavedBit(&gScene, arg0) == 0) && (ai->m_Mode == 0)) {
        Turret_EventShow(arg0, ai, arg2);
        CAI__SetMode(arg0, ai, 1);
    }
}
