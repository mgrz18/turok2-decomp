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

#include "structs/vector.h"

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

/* A step of a boss script: what to do (type), its argument, and the odds
 * (chance, out of 100) that the runner takes it; type 4 ends the script.
 * Script A (LevelScriptState.script) runs these. */
typedef struct BossStep {
    /* 0x00 */ u8 type;
    /* 0x04 */ s32 arg;
    /* 0x08 */ f32 animSpeed;
    /* 0x0C */ s32 chance;
    /* 0x10 */ f32 delay;
} BossStep; /* size 0x14 */

/* A step of script B (LevelScriptState.script2). */
typedef struct BossStep2 {
    /* 0x00 */ u8 type;
    /* 0x04 */ s32 arg;
    /* 0x08 */ u8 pad08[0x10 - 0x8];
    /* 0x10 */ s32 chance;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ f32 delay;
} BossStep2; /* size 0x1C */

/* A step of script C (LevelScriptState.script3): waits delay plus up to
 * delayRange (x15) before the next one. */
typedef struct BossStep3 {
    /* 0x00 */ u8 type;
    /* 0x04 */ s32 arg;
    /* 0x08 */ s32 chance;
    /* 0x0C */ f32 delay;
    /* 0x10 */ f32 delayRange;
} BossStep3; /* size 0x14 */

/* A script runner: the time left before the next step, the steps, the
 * cursor the runner advances and the step it took (-1 when stopped). */
typedef struct BossScript {
    /* 0x0 */ f32 timer;
    /* 0x4 */ BossStep *steps;
    /* 0x8 */ s32 cursor;
    /* 0xC */ s32 current;
} BossScript; /* size 0x10 */

typedef struct BossScript2 {
    /* 0x0 */ f32 timer;
    /* 0x4 */ BossStep2 *steps;
    /* 0x8 */ s32 cursor;
    /* 0xC */ s32 current;
} BossScript2; /* size 0x10 */

typedef struct BossScript3 {
    /* 0x0 */ f32 timer;
    /* 0x4 */ BossStep3 *steps;
    /* 0x8 */ s32 cursor;
    /* 0xC */ s32 current;
} BossScript3; /* size 0x10 */

/* D_8012F608: the state of the running level script, shared by the mode
 * handlers of the first VM module (func_004037B0-func_004079C4, second copy
 * func_0043C7B0-func_004409C4): the stage the script is in, the lights and
 * animators, the wave spawner, the boss parts and the boss AI (its state,
 * mode and three step scripts). Each mode uses the fields it needs and
 * some reuse a word for something else; the names follow the boss, which
 * uses most of them.
 *
 * Fields with symbols of their own (and the files' views anchored there):
 * D_8012F618 (swayPhase/swayAmount: BossWait), D_8012F950 (spawnsLeft...),
 * D_8012F988 / D_8012F9A8 (values[0] / values[1]), D_8012F9D0
 * (scriptCOn...), D_8012F9D4 (unk3CC...), D_8012F9D8 (bossState...:
 * BossState), D_8012F9DC (bossMode...: BossMode), D_8012F9E0 (armed,
 * scriptOn), D_8012F9F8 (script), D_8012F9FC (script.steps), D_8012FA00
 * (script.cursor), D_8012FA10 (script2.cursor...), D_8012FA1C (script3),
 * D_8012FB48 (path). D_8012F600 and D_8012F604 come just before it. */
typedef struct LevelScriptState {
    /* 0x000 */ s32 stage;
    /* 0x004 */ s32 total;
    /* 0x008 */ s32 bonus;
    /* 0x00C */ f32 speed;
    /* 0x010 */ f32 swayPhase;
    /* 0x014 */ f32 swayAmount;
    /* 0x018 */ s32 spawnStep;
    /* 0x01C */ s32 subStage;
    /* 0x020 */ u8 pad020[0x24 - 0x20];
    /* 0x024 */ ScriptAnimator anims[8];
    /* 0x344 */ void *intel;                         /* func_00225EBC(world, 1, ...) */
    /* 0x348 */ s32 spawnsLeft;
    /* 0x34C */ s32 wavesLeft;
    /* 0x350 */ s32 enemiesAlive;
    /* 0x354 */ f32 spawnTimer;
    /* 0x358 */ s32 region;
    /* 0x35C */ u8 doorsOpen;
    /* 0x35D */ u8 pad35D[0x360 - 0x35D];
    /* 0x360 */ f32 waveTimer;
    /* 0x364 */ f32 nextTimer;
    /* 0x368 */ struct CGameObjectInstance *actor;
    /* 0x36C */ struct CGameObjectInstance *found[5]; /* the boss parts: found[0], found[1] */
    /* 0x380 */ s32 values[2][8];
    /* 0x3C0 */ s32 part;
    /* 0x3C4 */ s32 partsDone;
    /* 0x3C8 */ u8 scriptCOn;                        /* script3 runs */
    /* 0x3C9 */ u8 partActive;
    /* 0x3CA */ u8 unk3CA;
    /* 0x3CB */ u8 pad3CB[0x3CC - 0x3CB];
    /* 0x3CC */ s32 unk3CC;
    /* 0x3D0 */ s32 bossState;
    /* 0x3D4 */ s32 bossMode;
    /* 0x3D8 */ u8 armed;
    /* 0x3D9 */ u8 scriptOn;                         /* script runs */
    /* 0x3DA */ u8 pad3DA[0x3DC - 0x3DA];
    /* 0x3DC */ f32 timer;
    /* 0x3E0 */ f32 swayTimer;
    /* 0x3E4 */ s32 damage;
    /* 0x3E8 */ f32 partTimer;
    /* 0x3EC */ f32 timer3EC;
    /* 0x3F0 */ BossScript script;
    /* 0x400 */ BossScript2 script2;
    /* 0x410 */ u8 script2On;
    /* 0x411 */ u8 pad411[0x414 - 0x411];
    /* 0x414 */ BossScript3 script3;
    /* 0x424 */ s32 resetRegion;
    /* 0x428 */ CVector3 dest;
    /* 0x434 */ CVector3 home;
    /* 0x440 */ CVector3 point;
    /* 0x44C */ u8 pad44C[0x540 - 0x44C];
    /* 0x540 */ s32 path;
    /* 0x544 */ u8 pad544[0x568 - 0x544];
} LevelScriptState; /* size 0x568 */

/* A few files read only the stage, as a plain word: the ROM reloads its
 * address before every read there, which the struct does not give (gcc
 * keeps &D_8012F608 in a register). They define BOSS_STAGE_AS_WORD and
 * declare it as an s32 themselves (func_00404C44, func_0043DC44). */
#ifndef BOSS_STAGE_AS_WORD
extern LevelScriptState D_8012F608;
#endif

/* LevelScriptState from bossState on (D_8012F9D8, +0x3D0). */
typedef struct BossState {
    /* 0x00 */ s32 state;
    /* 0x04 */ s32 mode;
    /* 0x08 */ u8 armed;
    /* 0x09 */ u8 scriptOn;
    /* 0x0A */ u8 pad0A[0xC - 0xA];
    /* 0x0C */ f32 timer;
    /* 0x10 */ f32 swayTimer;
    /* 0x14 */ s32 damage;
    /* 0x18 */ f32 partTimer;
    /* 0x1C */ f32 timer3EC;
    /* 0x20 */ BossScript script;
} BossState; /* size 0x30 */

/* LevelScriptState from bossMode on (D_8012F9DC, +0x3D4). */
typedef struct BossMode {
    /* 0x00 */ s32 mode;
    /* 0x04 */ u8 armed;
    /* 0x05 */ u8 scriptOn;
    /* 0x06 */ u8 pad06[0x8 - 0x6];
    /* 0x08 */ f32 timer;
    /* 0x0C */ f32 swayTimer;
    /* 0x10 */ s32 damage;
    /* 0x14 */ f32 partTimer;
    /* 0x18 */ f32 timer3EC;
    /* 0x1C */ BossScript script;
} BossMode; /* size 0x2C */

extern BossState D_8012F9D8;
extern BossMode D_8012F9DC;

#endif
