#include "common.h"

#include "structs/hud.h"

void COverlay__Update();                               /* extern */

/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS COverlay__Update receives COverlay__StartScript's own argument
 * registers (pass-through); the host passes them. */
/* and COverlay's members: the script pointer is pointer-wide on the host (the view stored it in
 * 4 bytes at 0x4 and the mode over the script's high half) */
#include "structs/player.h"
void COverlay__StartScript(COverlay *arg0, s32 *arg1) {
    arg0->m_pScript = arg1;
    arg0->m_Mode = 2;
    COverlay__Update(arg0);
}
