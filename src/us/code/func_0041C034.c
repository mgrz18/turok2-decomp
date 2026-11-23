#include "common.h"
#include "globals/menu_text.h"
#include "structs/menu.h"
#include "structs/player.h"
#include "globals/game.h"

#ifdef TARGET_PORT
#define T2_TEXT(s) ((void *) (s))
#else
#define T2_TEXT(s) (&s)
#endif

#ifdef TARGET_PORT
#else
#endif
#ifdef TARGET_PORT
#else
#endif


s32 OptionsMenu_ControlSideText(MenuItem *arg0, Menu *arg1) {
    CPlayerOptions *temp_v0_2;
    CPlayerOptions *var_v1;
    u8 temp_v1;
    CPlayer *temp_v0;

    temp_v0 = arg1->player;
    var_v1 = &gEngineApp_m_Settings.defaultSlot;
    if (temp_v0 != NULL) {
        temp_v0_2 = temp_v0->m_pOptions;
        if (temp_v0_2 != NULL) {
            var_v1 = temp_v0_2;
        }
    }
    temp_v1 = var_v1->m_ControlSide;
    if (temp_v1 != 0) {
        if (temp_v1 == 1) {
            arg0->table = T2_TEXT(sOptionsMenuText_ControlSideLeft);
        }
    } else {
        arg0->table = T2_TEXT(sOptionsMenuText_ControlSideRight);
    }
    return 0;
}
