#include "common.h"
#include "globals/menu_text.h"
#include "globals/game.h"

/* Only the field written here. */
typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ u8 *data;
} MenuData;


s32 StatsMenu_HeaderText(MenuData *arg0) {
    if (gEngineApp_m_Settings.match.tag != 0) {
        arg0->data = sStatsMenuText_YouTagged;
    } else {
        arg0->data = sStatsMenuText_YouFragged;
    }
    return 0;
}
