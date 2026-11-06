#include "common.h"
#include "structs/menu.h"
#include "structs/save.h"

s32 Level_GetHub(s32 id);

extern char D_800B4A40[];
extern char D_800B4A50[];
extern char D_800B4A60[];
extern char D_800B4A70[];
extern char D_800B4A88[];
extern char D_800B4A9C[];
extern char D_800B4AB4[];
extern char D_800B4ABC[];

s32 GameNoteMenu_UpdateLevel(MenuItem *item) {
    switch (Level_GetHub(gLoadBuffer->player.m_CheckPointWarpID)) {
        case -1:
        default:
            item->text = D_800B4ABC;
            break;
        case 0:
            item->text = D_800B4AB4;
            break;
        case 1:
            item->text = D_800B4A40;
            break;
        case 2:
            item->text = D_800B4A50;
            break;
        case 3:
            item->text = D_800B4A60;
            break;
        case 4:
            item->text = D_800B4A70;
            break;
        case 5:
            item->text = D_800B4A88;
            break;
        case 6:
            item->text = D_800B4A9C;
            break;
    }
    return 0;
}
