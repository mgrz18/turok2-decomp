#include "common.h"
#include "structs/game_object.h"

s32 CPlayer__StopLoopingSounds(sptr);                         /* extern */

void WeaponMode_Idle_Enter(CGameObjectInstance *arg0) {
    CPlayer__StopLoopingSounds((sptr) arg0->m_AI.m_pParent);
}
