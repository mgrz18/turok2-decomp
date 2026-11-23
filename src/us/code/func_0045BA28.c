/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_0040AD20.c, func_0040AD20.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "common.h"
#include "structs/menu.h"
#include "globals/pak.h"

extern char sPakMessageText_UpSpace[];
extern char sPakMessageText_PleaseInsertA[];
extern char sPakMessageText_InsertYourController_B4C78[];
extern char sPakMessageText_InsertYourController_B4D4C[];
extern char sPakMessageText_InsertYourController_B4CE4[];
extern char sPakMessageText_InsertYourRumblePak_B4DB0[];
extern char sPakMessageText_BeAbleToSaveYour[];
extern char sPakMessageText_PleaseTry[];
extern char sPakMessageText_NoController[];
extern char sPakMessageText_InsertYourRumblePak_B4EBC[];
extern char sPakMessageText_Empty_B4F10[];
extern char sPakMessageText_YouMayDeleteThis[];
extern char sPakMessageText_Deleted[];
extern char sPakMessageText_SaveANote[];
extern char sPakMessageText_DeleteANote[];
extern char sPakMessageText_PleaseRememberThat[];
s32 PakMessage_UpdateLine3_2(MenuItem *item) {
    switch (gPakMessageKind) {
    default:
    case 0:
        item->text = sPakMessageText_UpSpace;
        break;
    case 1:
        item->text = sPakMessageText_PleaseInsertA;
        break;
    case 2:
        item->text = sPakMessageText_InsertYourController_B4C78;
        break;
    case 3:
        item->text = sPakMessageText_InsertYourController_B4D4C;
        break;
    case 4:
        item->text = sPakMessageText_InsertYourController_B4CE4;
        break;
    case 5:
        item->text = sPakMessageText_InsertYourRumblePak_B4DB0;
        break;
    case 6:
        item->text = sPakMessageText_BeAbleToSaveYour;
        break;
    case 7:
        item->text = sPakMessageText_PleaseTry;
        break;
    case 8:
        item->text = sPakMessageText_NoController;
        break;
    case 9:
        item->text = sPakMessageText_InsertYourRumblePak_B4EBC;
        break;
    case 10:
        item->text = sPakMessageText_Empty_B4F10;
        break;
    case 11:
        item->text = sPakMessageText_YouMayDeleteThis;
        break;
    case 12:
        item->text = sPakMessageText_Deleted;
        break;
    case 13:
        item->text = sPakMessageText_SaveANote;
        break;
    case 14:
        item->text = sPakMessageText_DeleteANote;
        break;
    case 15:
        item->text = sPakMessageText_PleaseRememberThat;
        break;
    }
    return 0;
}
