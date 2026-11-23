/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_0040AD20.c, func_0040AD20.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "common.h"
#include "globals/menu_text.h"
#include "structs/menu.h"
#include "globals/pak.h"

s32 PakMessage_UpdateLine4_2(MenuItem *item) {
    switch (gPakMessageKind) {
    default:
    case 0:
        item->text = sPakMessageText_Empty_B4BF4;
        break;
    case 1:
        item->text = sPakMessageText_ControllerPakAnd;
        break;
    case 2:
        item->text = sPakMessageText_PakNowIfYouWishTo_B4C90;
        break;
    case 3:
        item->text = sPakMessageText_PakNowIfYouWishTo_B4D64;
        break;
    case 4:
        item->text = sPakMessageText_PakNowIfYouWishTo_B4CFC;
        break;
    case 5:
        item->text = sPakMessageText_NowIfYouWishToPlay_B4DC8;
        break;
    case 6:
        item->text = sPakMessageText_Game;
        break;
    case 7:
        item->text = sPakMessageText_Again;
        break;
    case 8:
        item->text = sPakMessageText_Empty_B4E88;
        break;
    case 9:
        item->text = sPakMessageText_NowIfYouWishToPlay_B4ED4;
        break;
    case 10:
        item->text = sPakMessageText_Empty_B4F14;
        break;
    case 11:
        item->text = sPakMessageText_Note;
        break;
    case 12:
        item->text = sPakMessageText_Empty_B4F90;
        break;
    case 13:
        item->text = sPakMessageText_Empty_B4FCC;
        break;
    case 14:
        item->text = sPakMessageText_Empty_B5008;
        break;
    case 15:
        item->text = sPakMessageText_SavedNotesMayHave;
        break;
    }
    return 0;
}
