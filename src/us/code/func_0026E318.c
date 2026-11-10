#include "common.h"
#include "structs/game_object.h"

void CGameObjectInstance__AdvanceAnimation();                               /* extern */

#ifdef TARGET_PORT
/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS CGameObjectInstance__AdvanceAnimation receives Event_Show's own argument
 * registers (pass-through); the host passes them. */
void Event_Show(CGameObjectInstance *arg0) {
    CGameObjectInstance__AdvanceAnimation(arg0);
    arg0->m_dwFlags = (s32) (arg0->m_dwFlags | 0x2100);
}
#else
void Event_Show(CGameObjectInstance *arg0) {
    CGameObjectInstance__AdvanceAnimation();
    arg0->m_dwFlags = (s32) (arg0->m_dwFlags | 0x2100);
}
#endif
