/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80409A5C_de_31.c, func_8040ACA0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/pak.h"

extern char sPakMessageText_ControllerPakWas[];
extern char sPakMessageText_ProblemOccurred_B4FD4[];
extern char sPakMessageText_NoteWas[];
extern char sPakMessageText_ControllerPakNot_B4BFC[];
extern char sPakMessageText_ProblemOccurred_B4F98[];
extern char sPakMessageText_ThisGameIsRumblePak_B4D84[];
extern char sPakMessageText_ThisGameIsController_B4CB4[];
extern char sPakMessageText_ControllerPak[];
extern char sPakMessageText_ThisGameIsRumblePak_B4E90[];
extern char sPakMessageText_SaveWas[];
extern char sPakMessageText_ThisNoteIsCorrupt[];
extern char sPakMessageText_ThisGameIsController_B4D1C[];
extern char sPakMessageText_ControllerPakNot_B4DEC[];
extern char sPakMessageText_ControllerPakIsFull[];
extern char sPakMessageText_Empty_B4E70[];
extern char sPakMessageText_ThisGameIsController_B4C48[];
s32 PakMessage_UpdateLine1(struct FieldRecAt14 *field) {
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
    field->text = sPakMessageText_ControllerPakIsFull;
    goto done;
case_1:
    field->text = sPakMessageText_ControllerPakNot_B4BFC;
    goto done;
case_2:
    field->text = sPakMessageText_ThisGameIsController_B4C48;
    goto done;
case_3:
    field->text = sPakMessageText_ThisGameIsController_B4D1C;
    goto done;
case_4:
    field->text = sPakMessageText_ThisGameIsController_B4CB4;
    goto done;
case_5:
    field->text = sPakMessageText_ThisGameIsRumblePak_B4D84;
    goto done;
case_6:
    field->text = sPakMessageText_ControllerPakNot_B4DEC;
    goto done;
case_7:
    field->text = sPakMessageText_ControllerPak;
    goto done;
case_8:
    field->text = sPakMessageText_Empty_B4E70;
    goto done;
case_9:
    field->text = sPakMessageText_ThisGameIsRumblePak_B4E90;
    goto done;
case_10:
    field->text = sPakMessageText_SaveWas;
    goto done;
case_11:
    field->text = sPakMessageText_ThisNoteIsCorrupt;
    goto done;
case_12:
    field->text = sPakMessageText_NoteWas;
    goto done;
case_13:
    field->text = sPakMessageText_ProblemOccurred_B4F98;
    goto done;
case_14:
    field->text = sPakMessageText_ProblemOccurred_B4FD4;
    goto done;
case_15:
    field->text = sPakMessageText_ControllerPakWas;
done:
    return 0;
}
