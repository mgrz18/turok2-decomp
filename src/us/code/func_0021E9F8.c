#include "common.h"
#include "structs/game_object.h"

s32 CGameObjectInstance__RequestAnim();                            /* extern */

#ifdef TARGET_PORT
/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS CGameObjectInstance__RequestAnim receives CGameObjectInstance__SetAnim's own argument
 * registers (pass-through); the host passes them. */
s32 CGameObjectInstance__SetAnim(CGameObjectInstance *arg0, sptr arg1, s32 arg2) {
    if ((arg1 < 0) || ((arg2 == 0) && (arg0->m_dwFlags & 0x400))) {
        return 0;
    }
    CGameObjectInstance__RequestAnim(arg0, arg1);
    return 1;
}
#else
s32 CGameObjectInstance__SetAnim(CGameObjectInstance *arg0, sptr arg1, s32 arg2) {
    if ((arg1 < 0) || ((arg2 == 0) && (arg0->m_dwFlags & 0x400))) {
        return 0;
    }
    CGameObjectInstance__RequestAnim();
    return 1;
}
#endif
