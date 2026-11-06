#include "structs/vector.h"
#include "structs/game.h"

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MULTI/8043E9A8_6.CPP, func_8043EDDC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "ultra_types.h"
#include "gfx.h"
#include "structs/list.h"
extern s32 GameOverMenu_Poll_2(void);
extern f32 gEngineApp_m_Settings_match_countdown; /* local type: the field's real type (or the member path) changes the ROM's code here */
extern void func_00469250(void *);
s32 GameOverMenu_Poll_2(void) {
    if (gEngineApp_m_Settings_match_countdown <= 0.0f) {
        func_00469250(&gEngineApp);
        return 1;
    }
    return 0;
}
