/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/WEAPONS/80204E78_7.CPP, func_80206080_de, with Turok 2's layouts. */
#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ f32 delayMin;
    /* 0x10 */ f32 delayMax;
} TriggerDef;

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ TriggerDef trigger;
} ActorDef;

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ ActorDef *def;
} Actor;

typedef struct {
    /* 0x00 */ u8 pad0[0x64];
    /* 0x64 */ f32 delay;
} AI;

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
void func_00263594(Actor *actor, AI *ai) {
    TriggerDef *trigger = &actor->def->trigger;
    f32 delay;
    GameOptions *opts;

    delay = func_002119FC(trigger->delayMin, trigger->delayMax);
    ai->delay = delay;
    opts = &D_80119870.options;
    if (opts->multiplayer && D_80119870.playerCount >= 3) {
        ai->delay = delay * 0.01f * (f32) (u32) (100 - (D_80119870.playerCount - 2) * 25);
    }
}
