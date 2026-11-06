#include "common.h"
#include "structs/game.h"
#include "structs/menu.h"
#include "globals/game.h"

extern char D_800B3FD8[];
extern char D_800B3FF0[];

s32 OptionsMenu_TwoPlayerLayoutText(MenuItem *a) {
    if (gEngineApp_m_Settings.mode == 13 && gEngineApp_m_Settings.multiplayer == 0) {
        a->flags &= ~0x1000000;
    } else {
        a->flags |= 0x1000000;
    }
    switch (gEngineApp_m_Settings.unk1B) {
    case 0:
        a->text = D_800B3FD8;
        break;
    case 1:
        a->text = D_800B3FF0;
        break;
    }
    return 0;
}
