/* Second-module copy of MissionMenu_UpdateObjective1Count (first module; this one at link address 0x00424AB0), calling this module's own functions at their link addresses. */
#include "common.h"
#include "globals/menu_text.h"
#include "structs/menu.h"
#include "globals/menu.h"
#include "globals/scene.h"
#include "protos/scene.h"

s32 MenuItem_TextLength_2(void *item);


s32 MissionMenu_UpdateObjective1Count_2(MenuItem *item) {
    s32 rank = -1;

    switch (sMissionHub) {
        case -1:
        case 0:
        default:
            item->text = sMissionMenuText_Unavailable;
            break;
        case 1:
            item->text = sMissionMenuText_ActivatedXOf3;
            rank = CScene__GetLinkGroupCount(&gScene, 2);
            if (rank != -1) {
                rank = 3 - rank;
            }
            break;
        case 2:
            item->text = sMissionMenuText_DestroyedXOf2;
            rank = CScene__GetLinkGroupCount(&gScene, 4);
            if (rank != -1) {
                rank = 2 - rank;
            }
            break;
        case 3:
            item->text = sMissionMenuText_RescuedXOf5;
            rank = CScene__GetLinkGroupCount(&gScene, 6);
            if (rank != -1) {
                rank = 5 - rank;
            }
            break;
        case 4:
            item->text = sMissionMenuText_SealedXOf3;
            rank = CScene__GetLinkGroupCount(&gScene, 8);
            if (rank != -1) {
                rank = 3 - rank;
            }
            break;
        case 5:
            item->text = sMissionMenuText_DestroyedXOf3_B44E8;
            rank = CScene__GetLinkGroupCount(&gScene, 9);
            if (rank != -1) {
                rank = 3 - rank;
            }
            break;
        case 6:
            item->text = sMissionMenuText_RecalibratedXOf4;
            rank = CScene__GetLinkGroupCount(&gScene, 11);
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
