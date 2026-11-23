#include "common.h"
#include "globals/menu_text.h"
#include "structs/game.h"
#include "structs/menu.h"
#include "globals/game.h"

extern s32 MenuItem_TextLength(MenuItem *a);
extern void sprintf(char *dst, char *fmt, ...);

/* Prints the second limit (gEngineApp_m_Settings+0x26) into the label, or "none". */
s32 FragTagMenu_TagLimitLabel(MenuItem *a) {
    GameSettings *setup = &gEngineApp_m_Settings;

    if (setup->tagScoreLimit != 0) {
        sprintf(a->text + (MenuItem_TextLength(a) - 4), sLimitLabelFmt_02d, setup->tagScoreLimit);
    } else {
        sprintf(a->text + (MenuItem_TextLength(a) - 4), sLimitLabelText_None);
    }
    return 0;
}
