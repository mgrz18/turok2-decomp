#include "common.h"
#include "structs/player.h"
#include "protos/player.h"

s32 CPlayer__UpdateRiding();                            /* extern */

#ifdef TARGET_PORT
/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS CPlayer__UpdateRiding receives PlayerRideMove_Update's own argument
 * registers (pass-through); the host passes them. */
void PlayerRideMove_Update(void *arg0, s32 arg1) {
    CPlayer__UpdateRiding(arg0, arg1);
    if (((CPlayer *)arg0)->m_Speed == 0.0f) {
        CPlayer__SetState(arg0, arg1, 0x21);
    }
}
#else
void PlayerRideMove_Update(void *arg0, s32 arg1) {
    CPlayer__UpdateRiding();
    if (((CPlayer *)arg0)->m_Speed == 0.0f) {
        CPlayer__SetState(arg0, arg1, 0x21);
    }
}
#endif
