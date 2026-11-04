/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/WEAPONS/80204E78_7.CPP, func_80206080_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/game_object.h"
#include "structs/intelligence.h"

typedef struct {
    /* 0x00 */ s32 flags; /* D_8011AAD8 */
    /* 0x04 */ u8 pad4[0x19 - 0x4];
    /* 0x19 */ u8 multiplayer; /* D_8011AAF1 */
} GameOptions;

/* The player lists; the player count is at +0x30, the game options at +0x1268. */
typedef struct {
    /* 0x0000 */ u8 pad0[0x30];
    /* 0x0030 */ u32 playerCount;
    /* 0x0034 */ u8 pad34[0x1268 - 0x34];
    /* 0x1268 */ GameOptions options;
} Players;

extern Players D_80119870;

extern f32 func_002119FC(f32 lo, f32 hi); /* random in range */

/* AI init: a random delay from the definition's range, shortened by 25% per
 * player beyond two in multiplayer. */
void func_00263594(CGameObjectInstance *actor, CAI *ai) {
    CGeneratorIntelligence *trigger = &actor->header.m_pIntelligence->m_Generator;
    f32 delay;
    GameOptions *opts;

    delay = func_002119FC(trigger->m_IntervalTime1, trigger->m_IntervalTime2);
    ai->m_ConditionTimer = delay;
    opts = &D_80119870.options;
    if (opts->multiplayer && D_80119870.playerCount >= 3) {
        ai->m_ConditionTimer = delay * 0.01f * (f32) (u32) (100 - (D_80119870.playerCount - 2) * 25);
    }
}
