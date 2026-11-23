/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_0040AD20.c, func_0040AD20.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "common.h"
#include "structs/menu.h"
#include "globals/pak.h"

extern char sPakMessageText_ControllerPakIsFull[];
extern char sPakMessageText_ControllerPakNot_B4BFC[];
extern char sPakMessageText_ThisGameIsController_B4C48[];
extern char sPakMessageText_ThisGameIsController_B4D1C[];
extern char sPakMessageText_ThisGameIsController_B4CB4[];
extern char sPakMessageText_ThisGameIsRumblePak_B4D84[];
extern char sPakMessageText_ControllerPakNot_B4DEC[];
extern char sPakMessageText_ControllerPak[];
extern char sPakMessageText_Empty_B4E70[];
extern char sPakMessageText_ThisGameIsRumblePak_B4E90[];
extern char sPakMessageText_SaveWas[];
extern char sPakMessageText_ThisNoteIsCorrupt[];
extern char sPakMessageText_NoteWas[];
extern char sPakMessageText_ProblemOccurred_B4F98[];
extern char sPakMessageText_ProblemOccurred_B4FD4[];
extern char sPakMessageText_ControllerPakWas[];
s32 PakMessage_UpdateLine1_2(MenuItem *item) {
    switch (gPakMessageKind) {
    default:
    case 0:
        item->text = sPakMessageText_ControllerPakIsFull;
        break;
    case 1:
        item->text = sPakMessageText_ControllerPakNot_B4BFC;
        break;
    case 2:
        item->text = sPakMessageText_ThisGameIsController_B4C48;
        break;
    case 3:
        item->text = sPakMessageText_ThisGameIsController_B4D1C;
        break;
    case 4:
        item->text = sPakMessageText_ThisGameIsController_B4CB4;
        break;
    case 5:
        item->text = sPakMessageText_ThisGameIsRumblePak_B4D84;
        break;
    case 6:
        item->text = sPakMessageText_ControllerPakNot_B4DEC;
        break;
    case 7:
        item->text = sPakMessageText_ControllerPak;
        break;
    case 8:
        item->text = sPakMessageText_Empty_B4E70;
        break;
    case 9:
        item->text = sPakMessageText_ThisGameIsRumblePak_B4E90;
        break;
    case 10:
        item->text = sPakMessageText_SaveWas;
        break;
    case 11:
        item->text = sPakMessageText_ThisNoteIsCorrupt;
        break;
    case 12:
        item->text = sPakMessageText_NoteWas;
        break;
    case 13:
        item->text = sPakMessageText_ProblemOccurred_B4F98;
        break;
    case 14:
        item->text = sPakMessageText_ProblemOccurred_B4FD4;
        break;
    case 15:
        item->text = sPakMessageText_ControllerPakWas;
        break;
    }
    return 0;
}
