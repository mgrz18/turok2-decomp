#include "common.h"
#include "structs/game_object.h"
#include "protos/ai.h"



void WeaponMode_AE934_Enter(sptr arg0, CAI *ai) {
    ai->m_Weapon.m_RefireDelay = 9.0e1f;
    CAI__SetMode(arg0, ai, 4);
}
