/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/WEAPONS/80204E78_7.CPP, func_80206080_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/game_object.h"
#include "structs/intelligence.h"
#include "structs/game.h"


typedef struct {
    /* 0x00 */ u8 pad0[0x64];
    /* 0x64 */ f32 delay;
} AI;




extern f32 func_002119FC(f32 lo, f32 hi); /* random in range */

/* AI init: a random delay from the definition's range, shortened by 25% per
 * player beyond two in multiplayer. */
void func_00263594(CGameObjectInstance *actor, AI *ai) {
    CGeneratorIntelligence *trigger = &actor->header.m_pIntelligence->m_Generator;
    f32 delay;
    GameSettings *opts;

    delay = func_002119FC(trigger->m_IntervalTime1, trigger->m_IntervalTime2);
    ai->delay = delay;
    opts = &D_80119870.m_Settings;
    if (opts->multiplayer && (u32) D_80119870.m_PlayerList.m_ActiveList.m_nCount >= 3) {
        ai->delay = delay * 0.01f * (f32) (u32) (100 - ((u32) D_80119870.m_PlayerList.m_ActiveList.m_nCount - 2) * 25);
    }
}
