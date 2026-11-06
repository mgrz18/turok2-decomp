/* Second-module copy of MissionMenu_UpdateObjective2Count (first module; this one at link address 0x00424C74), calling this module's own functions at their link addresses. */
#include "common.h"
#include "structs/menu.h"
#include "globals/menu.h"
#include "globals/scene.h"

s32 CScene__GetLinkGroupCount(void *arg0, s32 arg1);
s32 MenuItem_TextLength_2(void *item);

extern char D_800C1768[];
extern char D_800B43D0[];
extern char D_800B441C[];
extern char D_800B4470[];
extern char D_800B450C[];
extern char D_800B4574[];
extern char D_800B4588[];

s32 MissionMenu_UpdateObjective2Count_2(MenuItem *item) {
    s32 rank = -1;

    switch (sMissionHub) {
        case -1:
        case 0:
        default:
            item->text = D_800C1768;
            break;
        case 1:
            item->text = D_800B43D0;
            rank = CScene__GetLinkGroupCount(&gScene, 3);
            if (rank != -1) {
                rank = 4 - rank;
            }
            break;
        case 2:
            item->text = D_800B441C;
            rank = CScene__GetLinkGroupCount(&gScene, 5);
            if (rank != -1) {
                rank = 3 - rank;
            }
            break;
        case 3:
            item->text = D_800B4470;
            rank = CScene__GetLinkGroupCount(&gScene, 7);
            if (rank != -1) {
                rank = 3 - rank;
            }
            break;
        case 4:
            item->text = D_800C1768;
            break;
        case 5:
            item->text = D_800B450C;
            rank = CScene__GetLinkGroupCount(&gScene, 10);
            if (rank != -1) {
                rank = 1 - rank;
            }
            break;
        case 6:
            item->text = D_800B4588;
            rank = CScene__GetLinkGroupCount(&gScene, 12);
            if (rank != -1) {
                rank = 3 - rank;
                if (rank == 3) {
                    item->text = D_800B4574;
                }
            }
            break;
    }
    if (rank != -1) {
        item->text[MenuItem_TextLength_2(item) - 6] = rank + '0';
    }
    return 0;
}
