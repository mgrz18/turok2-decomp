#include "common.h"
#include "structs/menu.h"

extern char D_800C1768[];
extern char D_800B43BC[];
extern char D_800B4410[];
extern char D_800B4464[];
extern char D_800B44C4[];
extern char D_800B44FC[];
extern char D_800B4560[];
extern s32 sMissionHub;

s32 MissionMenu_UpdateObjective1(MenuItem *item) {
    switch (sMissionHub) {
        case -1:
        case 0:
        default:
            item->text = D_800C1768;
            break;
        case 1:
            item->text = D_800B43BC;
            break;
        case 2:
            item->text = D_800B4410;
            break;
        case 3:
            item->text = D_800B4464;
            break;
        case 4:
            item->text = D_800B44C4;
            break;
        case 5:
            item->text = D_800B44FC;
            break;
        case 6:
            item->text = D_800B4560;
            break;
    }
    return 0;
}
