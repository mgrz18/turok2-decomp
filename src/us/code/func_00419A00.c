#include "common.h"
#include "structs/menu.h"
#include "globals/game.h"

extern char sDebugMenuText_DebugOff[];
extern char sDebugMenuText_DebugOn[];
extern char sDebugMenuText_DebugVis[];
extern char sDebugMenuText_DebugAis[];
extern char sDebugMenuText_DebugSnd[];
extern char sDebugMenuText_DebugLinks[];

s32 Menu_DebugModeLabel(MenuItem *a) {
    switch (gEngineApp_m_Settings.unk18) {
    case 0:
        a->text = sDebugMenuText_DebugOff;
        break;
    case 1:
        a->text = sDebugMenuText_DebugOn;
        break;
    case 2:
        a->text = sDebugMenuText_DebugVis;
        break;
    case 3:
        a->text = sDebugMenuText_DebugAis;
        break;
    case 4:
        a->text = sDebugMenuText_DebugSnd;
        break;
    case 5:
        a->text = sDebugMenuText_DebugLinks;
        break;
    }
    return 0;
}
