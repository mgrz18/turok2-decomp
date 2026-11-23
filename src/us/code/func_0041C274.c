/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80444CF8_de_3.c, func_8044472C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/game.h"

extern char sOptionsMenuText_BloodColorGreen[];
extern char sOptionsMenuText_BloodColorRed[];
extern char sOptionsMenuText_BloodColorOff[];
s32 OptionsMenu_BloodColorText(struct FieldRecAt14 *field) {
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
