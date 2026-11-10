#include "common.h"
#include "structs/game_object.h"
#include "protos/ai.h"


void Turret_ActivateUpdate(sptr arg0, CAI *arg1) {
    if (arg1->m_AnimRepeatedCount != 0) {
        CAI__SetMode(arg0, arg1, 0x3E);
    }
}
