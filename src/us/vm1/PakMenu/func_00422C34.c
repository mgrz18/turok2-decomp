/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80409A5C_de_31.c, func_8040ACA0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/menu_text.h"
#include "globals/pak.h"

s32 PakMessage_UpdateLine4(struct FieldRecAt14 *field) {
    u32 state = gPakMessageKind;
    if (state >= 16) {
        goto state_other;
    }
    switch (state) {
        case 0: goto state_other;
        case 1: goto case_1;
        case 2: goto case_2;
        case 3: goto case_3;
        case 4: goto case_4;
        case 5: goto case_5;
        case 6: goto case_6;
        case 7: goto case_7;
        case 8: goto case_8;
        case 9: goto case_9;
        case 10: goto case_10;
        case 11: goto case_11;
        case 12: goto case_12;
        case 13: goto case_13;
        case 14: goto case_14;
        case 15: goto case_15;
        }
state_other:
    field->text = sPakMessageText_Empty_B4BF4;
    goto done;
case_1:
    field->text = sPakMessageText_ControllerPakAnd;
    goto done;
case_2:
    field->text = sPakMessageText_PakNowIfYouWishTo_B4C90;
    goto done;
case_3:
    field->text = sPakMessageText_PakNowIfYouWishTo_B4D64;
    goto done;
case_4:
    field->text = sPakMessageText_PakNowIfYouWishTo_B4CFC;
    goto done;
case_5:
    field->text = sPakMessageText_NowIfYouWishToPlay_B4DC8;
    goto done;
case_6:
    field->text = sPakMessageText_Game;
    goto done;
case_7:
    field->text = sPakMessageText_Again;
    goto done;
case_8:
    field->text = sPakMessageText_Empty_B4E88;
    goto done;
case_9:
    field->text = sPakMessageText_NowIfYouWishToPlay_B4ED4;
    goto done;
case_10:
    field->text = sPakMessageText_Empty_B4F14;
    goto done;
case_11:
    field->text = sPakMessageText_Note;
    goto done;
case_12:
    field->text = sPakMessageText_Empty_B4F90;
    goto done;
case_13:
    field->text = sPakMessageText_Empty_B4FCC;
    goto done;
case_14:
    field->text = sPakMessageText_Empty_B5008;
    goto done;
case_15:
    field->text = sPakMessageText_SavedNotesMayHave;
done:
    return 0;
}
