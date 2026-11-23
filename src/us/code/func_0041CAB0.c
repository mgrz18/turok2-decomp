#include "common.h"
#include "globals/game.h"

/* Only the field written here. */
typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ u8 *data;
} MenuData;

extern u8 sStatsMenuText_YouFragged[];
extern u8 sStatsMenuText_YouTagged[];

s32 StatsMenu_HeaderText(MenuData *arg0) {
    if (gEngineApp_m_Settings.match.tag != 0) {
        arg0->data = sStatsMenuText_YouTagged;
    } else {
        arg0->data = sStatsMenuText_YouFragged;
    }
    return 0;
}
