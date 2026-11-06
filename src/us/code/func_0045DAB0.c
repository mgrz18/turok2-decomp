/* Second-module copy of MissionMenu_UpdateObjective1Count (first module; this one at link address 0x00424AB0), calling this module's own functions at their link addresses. */
#include "common.h"
#include "structs/menu.h"
#include "globals/menu.h"

s32 CScene__GetLinkGroupCount(void *arg0, s32 arg1);
s32 MenuItem_TextLength_2(void *item);

extern u8 gScene[];
extern char D_800B438C[];
extern char D_800B43A8[];
extern char D_800B43FC[];
extern char D_800B4454[];
extern char D_800B44B4[];
extern char D_800B44E8[];
extern char D_800B454C[];

s32 MissionMenu_UpdateObjective1Count_2(MenuItem *item) {
    s32 rank = -1;

    switch (sMissionHub) {
        case -1:
        case 0:
        default:
            item->text = D_800B438C;
            break;
        case 1:
            item->text = D_800B43A8;
            rank = CScene__GetLinkGroupCount(gScene, 2);
            if (rank != -1) {
                rank = 3 - rank;
            }
            break;
        case 2:
            item->text = D_800B43FC;
            rank = CScene__GetLinkGroupCount(gScene, 4);
            if (rank != -1) {
                rank = 2 - rank;
            }
            break;
        case 3:
            item->text = D_800B4454;
            rank = CScene__GetLinkGroupCount(gScene, 6);
            if (rank != -1) {
                rank = 5 - rank;
            }
            break;
        case 4:
            item->text = D_800B44B4;
            rank = CScene__GetLinkGroupCount(gScene, 8);
            if (rank != -1) {
                rank = 3 - rank;
            }
            break;
        case 5:
            item->text = D_800B44E8;
            rank = CScene__GetLinkGroupCount(gScene, 9);
            if (rank != -1) {
                rank = 3 - rank;
            }
            break;
        case 6:
            item->text = D_800B454C;
            rank = CScene__GetLinkGroupCount(gScene, 11);
            if (rank != -1) {
                rank = 4 - rank;
            }
            break;
    }
    if (rank != -1) {
        item->text[MenuItem_TextLength_2(item) - 6] = rank + '0';
    }
    return 0;
}
