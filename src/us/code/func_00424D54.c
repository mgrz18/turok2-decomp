#include "common.h"
#include "structs/menu.h"

s32 CScene__GetLinkGroupCount(void *arg0, s32 arg1);
s32 MenuItem_TextLength(void *item);

extern u8 D_800F7078[];
extern char D_800C1768[];
extern char D_800B43D0[];
extern char D_800B441C[];
extern char D_800B4470[];
extern char D_800B450C[];
extern char D_800B4574[];
extern char D_800B4588[];
extern s32 D_80130A30;

s32 MissionMenu_UpdateObjective2Count(MenuItem *item) {
    s32 rank = -1;

    switch (D_80130A30) {
        case -1:
        case 0:
        default:
            item->text = D_800C1768;
            break;
        case 1:
            item->text = D_800B43D0;
            rank = CScene__GetLinkGroupCount(D_800F7078, 3);
            if (rank != -1) {
                rank = 4 - rank;
            }
            break;
        case 2:
            item->text = D_800B441C;
            rank = CScene__GetLinkGroupCount(D_800F7078, 5);
            if (rank != -1) {
                rank = 3 - rank;
            }
            break;
        case 3:
            item->text = D_800B4470;
            rank = CScene__GetLinkGroupCount(D_800F7078, 7);
            if (rank != -1) {
                rank = 3 - rank;
            }
            break;
        case 4:
            item->text = D_800C1768;
            break;
        case 5:
            item->text = D_800B450C;
            rank = CScene__GetLinkGroupCount(D_800F7078, 10);
            if (rank != -1) {
                rank = 1 - rank;
            }
            break;
        case 6:
            item->text = D_800B4588;
            rank = CScene__GetLinkGroupCount(D_800F7078, 12);
            if (rank != -1) {
                rank = 3 - rank;
                if (rank == 3) {
                    item->text = D_800B4574;
                }
            }
            break;
    }
    if (rank != -1) {
        item->text[MenuItem_TextLength(item) - 6] = rank + '0';
    }
    return 0;
}
