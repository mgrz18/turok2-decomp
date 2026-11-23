#include "common.h"
#include "globals/menu_text.h"
#include "structs/menu.h"
#include "globals/game.h"


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
