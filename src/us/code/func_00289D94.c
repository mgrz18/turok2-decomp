#include "common.h"
#include "structs/game.h"
#include "globals/libultra.h"

/* The game state (CEngineApp in CEngineApp__Construct). */

void osViSetMode(s32);
s32 osViSetSpecialFeatures(s32);

s32 osViSetYScale(f32);                         /* extern */
extern f32 D_800A9E24;
extern s32 D_800B7E60;
extern s32 gResolution;

void EngineMode17_Switch_Init(CEngineApp *arg0) {
    s32 *var_a0;

    if (arg0->m_Settings.unk1C8 != 0) {
        var_a0 = &D_800B7E60;
        if (gResolution == 0) {
            var_a0 = &osViModeTable;
        }
        osViSetMode((s32) var_a0);
        osViSetYScale(D_800A9E24);
        osViSetSpecialFeatures(2);
    }
}
