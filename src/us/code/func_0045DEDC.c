/* Second-module copy of MissionMenu_UpdateLevelName (first module; this one at link address 0x00424EDC), calling this module's own functions at their link addresses. */
#include "common.h"
#include "structs/menu.h"

extern char D_800C1768[];
extern char D_800B4398[];
extern char D_800B43EC[];
extern char D_800B4444[];
extern char D_800B449C[];
extern char D_800B44D4[];
extern char D_800B4534[];
extern s32 D_80130A30;

s32 MissionMenu_UpdateLevelName_2(MenuItem *item) {
    switch (D_80130A30) {
        case -1:
        case 0:
        default:
            item->text = D_800C1768;
            break;
        case 1:
            item->text = D_800B4398;
            break;
        case 2:
            item->text = D_800B43EC;
            break;
        case 3:
            item->text = D_800B4444;
            break;
        case 4:
            item->text = D_800B449C;
            break;
        case 5:
            item->text = D_800B44D4;
            break;
        case 6:
            item->text = D_800B4534;
            break;
    }
    return 0;
}
