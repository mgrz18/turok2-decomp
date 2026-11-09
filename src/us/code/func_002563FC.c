#include "common.h"
#include "structs/player.h"
#include "protos/player.h"

s32 CPlayer__UpdateRiding();                            /* extern */

void PlayerRideIdle_Update(CPlayer *arg0, s32 arg1) {
    CPlayer__UpdateRiding();
    if (arg0->m_Speed != 0.0f) {
        CPlayer__SetState(arg0, arg1, 0x22);
    }
}
