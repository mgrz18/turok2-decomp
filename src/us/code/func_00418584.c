#include "common.h"
#include "structs/game.h"
#include "structs/menu.h"
#include "globals/game.h"

extern char D_800C0E84[]; /* "  %02d" */
extern char D_800C0E8C[]; /* "none" */
extern s32 MenuItem_TextLength(MenuItem *a);
extern void sprintf(char *dst, char *fmt, ...);

/* Prints the second limit (gEngineApp_m_Settings+0x26) into the label, or "none". */
s32 FragTagMenu_TagLimitLabel(MenuItem *a) {
    GameSettings *setup = &gEngineApp_m_Settings;

    if (setup->tagScoreLimit != 0) {
        sprintf(a->text + (MenuItem_TextLength(a) - 4), D_800C0E84, setup->tagScoreLimit);
    } else {
        sprintf(a->text + (MenuItem_TextLength(a) - 4), D_800C0E8C);
    }
    return 0;
}
