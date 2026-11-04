#include "common.h"
#include "structs/game_object.h"
#include "structs/game.h"

/* AI state: calls func_002535E4 on the players with 900, then goes to anim state 0x1F once the anim is done. */

extern void func_002535E4(void *players, f32 value);
extern void func_00243414(void *obj, CAI *ai, s32 state);

void func_0040BE50(void *obj, CAI *ai) {
    func_002535E4(&D_80119870.m_PlayerList, 900.0f);
    if (ai->m_AnimRepeatedCount > 0) {
        func_00243414(obj, ai, 0x1F);
    }
}
