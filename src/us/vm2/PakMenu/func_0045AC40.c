/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/PAK/80409A88_7.CPP, func_8040A37C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "globals/menu_text.h"
#include "globals/pak.h"
struct Field;
struct Field {
    char pad[0x14];
    char **text;
    char pad18[0x4];
    signed int value;
};
extern char *D_800C140C[];
s32 PakMenu_UpdateSubtitle_2(struct Field *field, s16 *arg1) {
    s32 option;
    u32 rel;
    option = *arg1;
    rel = option - 3;
    if (gPakMenuIsLoad != 0) {
        if (gPakMenuIsProfile != 0) {
            field->text = sPakMenuText_LoadNote_B4870;
        } else {
            field->text = sPakMenuText_LoadNote_B4888;
        }
        goto done;
    }
    if (gPakMenuIsSave != 0) {
        if (rel < 16) {
            field->text = sPakMenuText_DeleteNote;
            return 0;
        }
        if (option == 0x15) {
            field->text = D_800C140C;
        } else if (gPakMenuIsProfile != 0) {
            field->text = sPakMenuText_SaveNote_B487C;
        } else {
            field->text = sPakMenuText_SaveNote_B4894;
        }
        goto done;
    }
    if (gPakMenuAnyPak != 0) {
        field->text = sPakMenuText_ControllerPakMenu;
    }
done:
    return 0;
}
