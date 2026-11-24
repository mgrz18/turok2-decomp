/* Second-module copy of MissionMenu_UpdateObjective2Count (first module; this one at link address 0x00424C74), calling this module's own functions at their link addresses. */
#include "common.h"
#include "globals/menu_text.h"
#include "structs/menu.h"
#include "globals/menu.h"
#include "globals/scene.h"
#include "protos/scene.h"

s32 MenuItem_TextLength_2(void *item);


s32 MissionMenu_UpdateObjective2Count_2(MenuItem *item) {
    s32 rank = -1;

    switch (sMissionHub) {
        case -1:
        case 0:
        default:
            item->text = sMissionMenuText_Space;
            break;
        case 1:
            item->text = sMissionMenuText_RescuedXOf4;
            rank = CScene__GetLinkGroupCount(&gScene, 3);
            if (rank != -1) {
                rank = 4 - rank;
            }
            break;
        case 2:
            item->text = sMissionMenuText_DestroyedXOf3_B441C;
            rank = CScene__GetLinkGroupCount(&gScene, 5);
            if (rank != -1) {
                rank = 3 - rank;
            }
            break;
        case 3:
            item->text = sMissionMenuText_DestroyedXOf3_B4470;
            rank = CScene__GetLinkGroupCount(&gScene, 7);
            if (rank != -1) {
                rank = 3 - rank;
            }
            break;
        case 4:
            item->text = sMissionMenuText_Space;
            break;
        case 5:
            item->text = sMissionMenuText_DestroyedXOf1;
            rank = CScene__GetLinkGroupCount(&gScene, 10);
            if (rank != -1) {
                rank = 1 - rank;
            }
            break;
        case 6:
            item->text = sMissionMenuText_DestroyedXOf;
            rank = CScene__GetLinkGroupCount(&gScene, 12);
            if (rank != -1) {
                rank = 3 - rank;
                if (rank == 3) {
                    item->text = sMissionMenuText_DestroyedXOf3_B4574;
                }
            }
            break;
    }
    if (rank != -1) {
        item->text[MenuItem_TextLength_2(item) - 6] = rank + '0';
    }
    return 0;
}
