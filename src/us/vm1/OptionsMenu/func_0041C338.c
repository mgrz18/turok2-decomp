#include "common.h"
#include "globals/menu_text.h"
#include "structs/game.h"
#include "structs/menu.h"
#include "globals/game.h"


s32 OptionsMenu_TwoPlayerLayoutText(MenuItem *a) {
    if (gEngineApp_m_Settings.mode == 13 && gEngineApp_m_Settings.multiplayer == 0) {
        a->flags &= ~0x1000000;
    } else {
        a->flags |= 0x1000000;
    }
    switch (gEngineApp_m_Settings.unk1B) {
    case 0:
        a->text = sOptionsMenuText_2PlyrLayoutHorz;
        break;
    case 1:
        a->text = sOptionsMenuText_2PlyrLayoutVert;
        break;
    }
    return 0;
}
