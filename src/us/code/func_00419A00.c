#include "common.h"
#include "structs/menu.h"
#include "globals/game.h"

extern char D_800C1EE0[];
extern char D_800C1EF8[];
extern char D_800C1F10[];
extern char D_800C1F28[];
extern char D_800C1F40[];
extern char D_800C1F58[];

s32 Menu_DebugModeLabel(MenuItem *a) {
    switch (gEngineApp_m_Settings.unk18) {
    case 0:
        a->text = D_800C1EE0;
        break;
    case 1:
        a->text = D_800C1EF8;
        break;
    case 2:
        a->text = D_800C1F10;
        break;
    case 3:
        a->text = D_800C1F28;
        break;
    case 4:
        a->text = D_800C1F40;
        break;
    case 5:
        a->text = D_800C1F58;
        break;
    }
    return 0;
}
