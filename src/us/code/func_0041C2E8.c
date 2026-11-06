/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80444CF8_de_3.c, func_804447A0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/game.h"

extern s32 Menu_StepValue(void *, s32, s32, s32, s32, s32);
s32 OptionsMenu_StepTwoPlayerLayout(void *first, void *second) {
    u8 *option = &gEngineApp_m_Settings.unk1B;
    *option = Menu_StepValue(second, *option, 1, 0, 1, 1);
    return 0;
}
