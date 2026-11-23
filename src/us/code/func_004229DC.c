/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80409A5C_de_31.c, func_8040ACA0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/pak.h"

extern char sPakMessageText_SuccessfullyRepaired[];
extern char sPakMessageText_WhileTryingTo_B4FE8[];
extern char sPakMessageText_Successfully[];
extern char sPakMessageText_Found[];
extern char sPakMessageText_WhileTryingTo_B4FAC[];
extern char sPakMessageText_CompatiblePlease_B4D9C[];
extern char sPakMessageText_PakCompatiblePlease_B4CCC[];
extern char sPakMessageText_WasChanged[];
extern char sPakMessageText_CompatiblePlease_B4EA8[];
extern char sPakMessageText_Successful[];
extern char sPakMessageText_AndCannotBeUsed[];
extern char sPakMessageText_PakCompatiblePlease_B4D34[];
extern char sPakMessageText_FoundYouWillNot[];
extern char sPakMessageText_DeleteANoteToFree[];
extern char sPakMessageText_Empty_B4E74[];
extern char sPakMessageText_PakCompatiblePlease_B4C60[];
s32 PakMessage_UpdateLine2(struct FieldRecAt14 *field) {
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
    field->text = sPakMessageText_DeleteANoteToFree;
    goto done;
case_1:
    field->text = sPakMessageText_Found;
    goto done;
case_2:
    field->text = sPakMessageText_PakCompatiblePlease_B4C60;
    goto done;
case_3:
    field->text = sPakMessageText_PakCompatiblePlease_B4D34;
    goto done;
case_4:
    field->text = sPakMessageText_PakCompatiblePlease_B4CCC;
    goto done;
case_5:
    field->text = sPakMessageText_CompatiblePlease_B4D9C;
    goto done;
case_6:
    field->text = sPakMessageText_FoundYouWillNot;
    goto done;
case_7:
    field->text = sPakMessageText_WasChanged;
    goto done;
case_8:
    field->text = sPakMessageText_Empty_B4E74;
    goto done;
case_9:
    field->text = sPakMessageText_CompatiblePlease_B4EA8;
    goto done;
case_10:
    field->text = sPakMessageText_Successful;
    goto done;
case_11:
    field->text = sPakMessageText_AndCannotBeUsed;
    goto done;
case_12:
    field->text = sPakMessageText_Successfully;
    goto done;
case_13:
    field->text = sPakMessageText_WhileTryingTo_B4FAC;
    goto done;
case_14:
    field->text = sPakMessageText_WhileTryingTo_B4FE8;
    goto done;
case_15:
    field->text = sPakMessageText_SuccessfullyRepaired;
done:
    return 0;
}
