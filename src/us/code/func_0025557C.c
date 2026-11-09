#include "common.h"
#include "structs/player.h"
#include "protos/player.h"


void PlayerStand_Enter(CPlayer *arg0, s32 arg1) {
    if ((arg0->m_Speed != 0.0f) || (arg0->m_SideSpeed != 0.0f)) {
        CPlayer__SetState((s32) arg0, arg1, 3);
    }
}
