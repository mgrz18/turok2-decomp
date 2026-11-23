/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_0040AD20.c, func_0040AD20.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "common.h"
#include "structs/menu.h"
#include "globals/pak.h"

extern char sPakMessageText_Empty_B4BF8[];
extern char sPakMessageText_TryAgain[];
extern char sPakMessageText_ManageIt[];
extern char sPakMessageText_Save[];
extern char sPakMessageText_Load[];
extern char sPakMessageText_WithIt_B4DE0[];
extern char sPakMessageText_Empty_B4E34[];
extern char sPakMessageText_Empty_B4E6C[];
extern char sPakMessageText_Empty_B4E8C[];
extern char sPakMessageText_WithIt_B4EEC[];
extern char sPakMessageText_Empty_B4F18[];
extern char sPakMessageText_Empty_B4F64[];
extern char sPakMessageText_Empty_B4F94[];
extern char sPakMessageText_Empty_B4FD0[];
extern char sPakMessageText_Empty_B500C[];
extern char sPakMessageText_BeenDeleted[];
s32 PakMessage_UpdateLine5_2(MenuItem *item) {
    switch (gPakMessageKind) {
    default:
    case 0:
        item->text = sPakMessageText_Empty_B4BF8;
        break;
    case 1:
        item->text = sPakMessageText_TryAgain;
        break;
    case 2:
        item->text = sPakMessageText_ManageIt;
        break;
    case 3:
        item->text = sPakMessageText_Save;
        break;
    case 4:
        item->text = sPakMessageText_Load;
        break;
    case 5:
        item->text = sPakMessageText_WithIt_B4DE0;
        break;
    case 6:
        item->text = sPakMessageText_Empty_B4E34;
        break;
    case 7:
        item->text = sPakMessageText_Empty_B4E6C;
        break;
    case 8:
        item->text = sPakMessageText_Empty_B4E8C;
        break;
    case 9:
        item->text = sPakMessageText_WithIt_B4EEC;
        break;
    case 10:
        item->text = sPakMessageText_Empty_B4F18;
        break;
    case 11:
        item->text = sPakMessageText_Empty_B4F64;
        break;
    case 12:
        item->text = sPakMessageText_Empty_B4F94;
        break;
    case 13:
        item->text = sPakMessageText_Empty_B4FD0;
        break;
    case 14:
        item->text = sPakMessageText_Empty_B500C;
        break;
    case 15:
        item->text = sPakMessageText_BeenDeleted;
        break;
    }
    return 0;
}
