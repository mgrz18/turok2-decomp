#include "common.h"
#include "structs/game_object.h"
#include "globals/scene.h"
#include "protos/scene.h"
#include "protos/ai.h"

s32 Turret_EventShow(sptr, void *, s32);        /* extern */

void Turret_EventActivate(sptr arg0, CAI *ai, sptr arg2) {
    if ((CScene__GetInstanceSavedBit(&gScene, arg0) == 0) && (ai->m_Mode == 0)) {
        Turret_EventShow(arg0, ai, arg2);
        CAI__SetMode(arg0, ai, 1);
    }
}
