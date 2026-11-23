/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/ITEMS/80444030_6.CPP, func_8044472C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "globals/menu_text.h"
#include "globals/game.h"
struct FieldRecAt14;
struct FieldRecAt14 {
    char pad[0x14];
    char *text;
};
s32 OptionsMenu_BloodColorText_2(struct FieldRecAt14 *field) {
    switch (gEngineApp_m_Settings.unk17) {
    case 0:
        field->text = sOptionsMenuText_BloodColorOff;
        break;
    case 1:
        field->text = sOptionsMenuText_BloodColorRed;
        break;
    case 2:
        field->text = sOptionsMenuText_BloodColorGreen;
        break;
    default:
        return 0;
    }
    return 0;
}
