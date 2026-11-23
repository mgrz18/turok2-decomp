#include "common.h"
#include "structs/game.h"
#include "structs/menu.h"
#include "globals/game.h"

s32 MenuItem_TextLength_2(MenuItem *item); /* length of the item's text line */
s32 sprintf(char *buf, const char *fmt, ...); /* sprintf */

extern char sDebugMenuText_DebugOff[];
extern char sDebugMenuText_DebugOn[];
extern char sDebugMenuText_DebugVis[];
extern char sDebugMenuText_DebugAis[];
extern char sDebugMenuText_DebugSnd[];
extern char sDebugMenuText_DebugLinks[];

/* Names setting 0x18. */
s32 Menu_DebugModeLabel_2(MenuItem *item) {
    switch (gEngineApp_m_Settings.unk18) {
    case 0:
        item->text = sDebugMenuText_DebugOff;
        break;
    case 1:
        item->text = sDebugMenuText_DebugOn;
        break;
    case 2:
        item->text = sDebugMenuText_DebugVis;
        break;
    case 3:
        item->text = sDebugMenuText_DebugAis;
        break;
    case 4:
        item->text = sDebugMenuText_DebugSnd;
        break;
    case 5:
        item->text = sDebugMenuText_DebugLinks;
        break;
    }
    return 0;
}
