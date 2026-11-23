#include "common.h"
#include "structs/game.h"
#include "structs/menu.h"
#include "globals/game.h"

extern char sLimitLabelFmt_02d[]; /* "  %02d" */
extern char sLimitLabelText_None[]; /* "none" */
extern s32 MenuItem_TextLength(MenuItem *a);
extern void sprintf(char *dst, char *fmt, ...);

/* Prints the setting at gEngineApp_m_Settings+0x2A into the label, or "none". */
s32 TeamBloodMenu_TimeLimitLabel(MenuItem *a) {
    GameSettings *setup = &gEngineApp_m_Settings;

    if (setup->teamTimeLimit != 0) {
        sprintf(a->text + (MenuItem_TextLength(a) - 4), sLimitLabelFmt_02d, setup->teamTimeLimit);
    } else {
        sprintf(a->text + (MenuItem_TextLength(a) - 4), sLimitLabelText_None);
    }
    return 0;
}
