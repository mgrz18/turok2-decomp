#include "common.h"
#include "structs/game_object.h"

s32 CAI__SetMode(s32, s32, s32);


void WeaponMode_AE934_Enter(s32 arg0, CAI *ai) {
    ai->m_Weapon.m_RefireDelay = 9.0e1f;
    CAI__SetMode(arg0, (s32) ai, 4);
}
