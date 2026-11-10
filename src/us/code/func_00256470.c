#include "common.h"
#include "structs/player.h"
#include "protos/player.h"

s32 CPlayer__UpdateRiding();                            /* extern */

void PlayerRideMove_Update(void *arg0, sptr arg1) {
    CPlayer__UpdateRiding();
    if (((CPlayer *)arg0)->m_Speed == 0.0f) {
        CPlayer__SetState(arg0, arg1, 0x21);
    }
}
