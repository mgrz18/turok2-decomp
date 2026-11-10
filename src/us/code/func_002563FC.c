#include "common.h"
#include "structs/player.h"
#include "protos/player.h"

s32 CPlayer__UpdateRiding();                            /* extern */

#ifdef TARGET_PORT
/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS CPlayer__UpdateRiding receives PlayerRideIdle_Update's own argument
 * registers (pass-through); the host passes them. */
void PlayerRideIdle_Update(CPlayer *arg0, sptr arg1) {
    CPlayer__UpdateRiding(arg0, arg1);
    if (arg0->m_Speed != 0.0f) {
        CPlayer__SetState(arg0, arg1, 0x22);
    }
}
#else
void PlayerRideIdle_Update(CPlayer *arg0, sptr arg1) {
    CPlayer__UpdateRiding();
    if (arg0->m_Speed != 0.0f) {
        CPlayer__SetState(arg0, arg1, 0x22);
    }
}
#endif
