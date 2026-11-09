/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80444CF8_de_3.c, func_804446D8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/game.h"
#include "protos/menu.h"

s32 Menu_StepDebugMode(void *first, void *second) {
    u8 *option = &gEngineApp_m_Settings.unk18;
    *option = Menu_StepValue(second, *option, 1, 0, 2, 1);
    return 0;
}
