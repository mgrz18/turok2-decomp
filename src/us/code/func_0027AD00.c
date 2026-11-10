#include "common.h"

/* What this sets on its first argument: a value and a mode. */
typedef struct {
    /* 0x0 */ s32 unk0;
    /* 0x4 */ s32 value;
    /* 0x8 */ s32 mode;
} ModeState;

void COverlay__Update();                               /* extern */

#ifdef TARGET_PORT
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
#else
void COverlay__StartScript(ModeState *arg0, sptr arg1) {
    arg0->value = arg1;
    arg0->mode = 2;
    COverlay__Update();
}
#endif
