#include "common.h"
#include "globals/game.h"

extern f32 D_800C1064;
/* Plain pointer arithmetic, not struct fields: GCC marks struct accesses
 * MEM_IN_STRUCT and would then hoist them above the global store. */
#define OBJ(arg)        (*(u8 **) ((u8 *) (arg) + 0xC))
#define OBJ_FLAGS(obj)  (*(s32 *) ((u8 *) (obj) + 0x30))
#include "structs/menu.h"

/* The port: by member. The N64 reads the window's items at 0xC and the word at items + 0x30: the
 * second item's flags (MenuItem is 0x28 bytes on the N64, larger on the host). The game over /
 * continue window crashed here when the last life was lost. */
void GameOverMenu_Init(void *arg0) {
    gEngineApp_m_Settings.match.countdown = D_800C1064;
    ((Menu *) arg0)->items[1].flags &= ~0x04000000;
}

#include "structs/vector.h"
#include "structs/game.h"
#include "ultra_types.h"
#include "gfx.h"
#include "structs/list.h"

s32 GameOverMenu_Poll(void);
extern void CEngineApp__QuitToLegal(void *);

s32 GameOverMenu_Poll(void) {
    if (gEngineApp_m_Settings_match_countdown <= 0.0f) {
        CEngineApp__QuitToLegal(&gEngineApp);
        return 1;
    }
    return 0;
}

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8043E830_de_13.c, func_8043EE24_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/engine.h"

s32 GameOverMenu_TickCountdown(void) {
    GameMatch *state = &gEngineApp_m_Settings.match;
    f32 timer = state->countdown - gFrameIncrement;
    state->countdown = timer;
    if (timer < 0.0f) {
        state->countdown = 0.0f;
    }
    return 0;
}
