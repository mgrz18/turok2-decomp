#include "common.h"
#include "globals/menu_text.h"
#include "structs/menu.h"
#include "globals/game.h"



s32 StatsMenu_HeaderText(MenuItem *arg0) {
    if (gEngineApp_m_Settings.match.tag != 0) {
        arg0->text = (char *)sStatsMenuText_YouTagged;
    } else {
        arg0->text = (char *)sStatsMenuText_YouFragged;
    }
    return 0;
}
