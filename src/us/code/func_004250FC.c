#include "common.h"
#include "structs/menu.h"
#include "globals/menu.h"

extern char D_800C1768[];
extern char D_800B43E0[];
extern char D_800B4430[];
extern char D_800B4484[];
extern char D_800B4520[];
extern char D_800B459C[];

s32 MissionMenu_UpdateObjective2(MenuItem *item) {
    switch (sMissionHub) {
        case -1:
        case 0:
        default:
            item->text = D_800C1768;
            break;
        case 1:
            item->text = D_800B43E0;
            break;
        case 2:
            item->text = D_800B4430;
            break;
        case 3:
            item->text = D_800B4484;
            break;
        case 4:
            item->text = D_800C1768;
            break;
        case 5:
            item->text = D_800B4520;
            break;
        case 6:
            item->text = D_800B459C;
            break;
    }
    return 0;
}
