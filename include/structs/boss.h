#ifndef STRUCTS_BOSS_H
#define STRUCTS_BOSS_H

/* The boss and level-script state of the VM modules (func_00403xxx-
 * func_0040Fxxx, and their second copies at func_0043Cxxx-func_00448xxx,
 * which use the same globals). Three blocks follow each other in BSS:
 *
 *   D_8012F608  LevelScriptState  the running script/boss mode (0x568)
 *   D_8012FB70  MotherBossState   the Mother boss fight (0x690)
 *   D_80130200  BlindOneState     the Blind One boss fight (0x560)
 *
 * Several fields also have symbols of their own, which some files still
 * use, and some files reach a block through a neighbouring symbol at a
 * negative offset (D_8012F9DC - 0x3D4, D_8012F604, ...): those keep their
 * symbol, as the ROM addresses them that way. The extern declarations here
 * are the block-level symbols only.
 *
 * Nothing here includes structs/game_object.h, since many module files
 * still define a local CAI: actors are `struct CGameObjectInstance *`. */

/* The 0x64-byte value/colour animator func_002362E4 sets up,
 * func_00236314 restarts and func_00235510 steps (Rage Wars' Animator,
 * rw/abu_types.h, is the same object without the last word). */
typedef struct ScriptAnimator {
    /* 0x00 */ u8 data[0x64];
} ScriptAnimator; /* size 0x64 */

/* One weak spot of the boss: its health and its hit-flash animator. */
typedef struct BossWeakSpot {
    /* 0x00 */ s32 health;
    /* 0x04 */ u8 anim[0x68 - 0x4];   /* a ScriptAnimator */
} BossWeakSpot; /* size 0x68 */

/* D_8012FB70: the Mother boss fight, func_00409000-func_0040C7DC (second
 * copy func_00442000-func_004457DC; func_0040C7DC sets
 * m_bMotherBossComplete). Fifteen weak spots in three phases. Fields
 * with symbols of their own: D_801301CC (phase), D_801301D0 (activeMask),
 * D_801301D4 (liveMask), D_801301EC (counter), D_801301F8 / D_801301FC
 * (minRadius / maxRadius). */
typedef struct MotherBossState {
    /* 0x000 */ struct CGameObjectInstance *obj;     /* the boss */
    /* 0x004 */ s32 count;                           /* entries in actors */
    /* 0x008 */ struct CGameObjectInstance *actors[4];
    /* 0x018 */ BossWeakSpot slots[15];
    /* 0x630 */ struct CGameObjectInstance *parts[7]; /* the 0x1DB1 actors */
    /* 0x64C */ s32 partCount;
    /* 0x650 */ struct CGameObjectInstance *controller; /* the 0x1DB2 actor */
    /* 0x654 */ u8 pad654[0x658 - 0x654];
    /* 0x658 */ void *intel;                         /* func_00225EBC(world, 1, 0x1DB1) */
    /* 0x65C */ s32 phase;
    /* 0x660 */ s32 activeMask;                      /* spots that can be hit */
    /* 0x664 */ s32 liveMask;                        /* spots of the phase still alive */
    /* 0x668 */ s32 unk668;
    /* 0x66C */ s32 lastAnim;
    /* 0x670 */ s32 partsLeft;
    /* 0x674 */ s32 totalHealth;
    /* 0x678 */ s32 health;
    /* 0x67C */ s32 counter;
    /* 0x680 */ s32 unk680;
    /* 0x684 */ s32 unk684;
    /* 0x688 */ s32 minRadius;
    /* 0x68C */ s32 maxRadius;
} MotherBossState; /* size 0x690 */

extern MotherBossState D_8012FB70;

/* A step of the Blind One's phase script (D_00432340): the phase to enter. */
typedef struct BlindOneStep {
    /* 0x0 */ s32 id;
    /* 0x4 */ s32 unk4;
    /* 0x8 */ s32 unk8;
} BlindOneStep; /* size 0xC */

/* Plays a sequence of bit masks: every interval seconds, each set bit sends
 * the matching actor (bits 0-3: group 1, 4-7: group 2, 8-11: group 3,
 * rotated by offset) to an attack state (func_0040C850). */
typedef struct BlindOneMover {
    /* 0x00 */ s32 *seq;
    /* 0x04 */ s32 pos;
    /* 0x08 */ f32 interval;
    /* 0x0C */ f32 timer;
    /* 0x10 */ s32 step;
    /* 0x14 */ s32 offset;
} BlindOneMover; /* size 0x18 */

/* D_80130200: the Blind One boss fight, func_0040C850-func_0040EEA0 (second
 * copy func_00445CC0-func_0044700C); its health bars read "Blind" and
 * "EyeBall". Three groups of level actors (types 0x454, 0x455, 0x456),
 * each with the count of its live ones (func_0040E00C counts them up,
 * func_0040E30C down), twelve animators, the phase script and the minions
 * the phases spawn (func_0040D0F0). func_0040ECDC and func_0040EEA0 reach
 * the animators from the block base by raw offsets, as the ROM does. */
typedef struct BlindOneState {
    /* 0x000 */ struct CGameObjectInstance *obj;     /* the boss */
    /* 0x004 */ s32 count1;
    /* 0x008 */ struct CGameObjectInstance *actors1[4];
    /* 0x018 */ s32 count2;
    /* 0x01C */ struct CGameObjectInstance *actors2[4];
    /* 0x02C */ s32 count3;
    /* 0x030 */ struct CGameObjectInstance *actors3[3];
    /* 0x03C */ ScriptAnimator slots[12];
    /* 0x4EC */ s32 slotCount;
    /* 0x4F0 */ void *minionIntel;                   /* func_00225EBC(world, 1, 0x1CE9) */
    /* 0x4F4 */ void *minionIntel2;                  /* func_00225EBC(world, 1, 0x1CEA) */
    /* 0x4F8 */ BlindOneStep *script;
    /* 0x4FC */ s32 phase;
    /* 0x500 */ s32 toSpawn;                         /* minions the phase still spawns */
    /* 0x504 */ s32 minionsAlive;
    /* 0x508 */ s32 maxMinions;
    /* 0x50C */ s32 alive1;                          /* live actors of type 0x454 */
    /* 0x510 */ s32 alive2;                          /* 0x455 */
    /* 0x514 */ s32 alive3;                          /* 0x456 */
    /* 0x518 */ BlindOneMover movers[3];
} BlindOneState; /* size 0x560 */

extern BlindOneState D_80130200;

#endif
