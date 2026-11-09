#include "common.h"
#include "structs/player.h"
#include "structs/game_object.h"
#include "protos/player.h"

f32 PlayerDodgeFall_Update(sptr, sptr);

void PlayerDodge_Update(CPlayer *arg0, CGameObjectInstance *arg1) {
    f32 temp_f2;

    temp_f2 = arg0->m_SideSpeed;
    if (((temp_f2 < 0.0f) && (arg0->m_ContState.m_FinalMoveX >= 0.0f)) || ((temp_f2 > 0.0f) && (arg0->m_ContState.m_FinalMoveX <= 0.0f)) || (arg0->m_ModeTime >= 1.5f)) {
        CPlayer__SetState(arg0, arg1, 7);
    } else {
        arg1->m_vVelocity.y = 307.19998f;
    }
    PlayerDodgeFall_Update((sptr) arg0, (sptr) arg1);
}
