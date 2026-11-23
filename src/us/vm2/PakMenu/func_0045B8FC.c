/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_0040AD20.c, func_0040AD20.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "common.h"
#include "globals/menu_text.h"
#include "structs/menu.h"
#include "globals/pak.h"

s32 PakMessage_UpdateLine2_2(MenuItem *item) {
    switch (gPakMessageKind) {
    default:
    case 0:
        item->text = sPakMessageText_DeleteANoteToFree;
        break;
    case 1:
        item->text = sPakMessageText_Found;
        break;
    case 2:
        item->text = sPakMessageText_PakCompatiblePlease_B4C60;
        break;
    case 3:
        item->text = sPakMessageText_PakCompatiblePlease_B4D34;
        break;
    case 4:
        item->text = sPakMessageText_PakCompatiblePlease_B4CCC;
        break;
    case 5:
        item->text = sPakMessageText_CompatiblePlease_B4D9C;
        break;
    case 6:
        item->text = sPakMessageText_FoundYouWillNot;
        break;
    case 7:
        item->text = sPakMessageText_WasChanged;
        break;
    case 8:
        item->text = sPakMessageText_Empty_B4E74;
        break;
    case 9:
        item->text = sPakMessageText_CompatiblePlease_B4EA8;
        break;
    case 10:
        item->text = sPakMessageText_Successful;
        break;
    case 11:
        item->text = sPakMessageText_AndCannotBeUsed;
        break;
    case 12:
        item->text = sPakMessageText_Successfully;
        break;
    case 13:
        item->text = sPakMessageText_WhileTryingTo_B4FAC;
        break;
    case 14:
        item->text = sPakMessageText_WhileTryingTo_B4FE8;
        break;
    case 15:
        item->text = sPakMessageText_SuccessfullyRepaired;
        break;
    }
    return 0;
}
