#ifndef STRUCTS_BOSS_H
#define STRUCTS_BOSS_H

/* The boss and level-script state of the VM modules (func_00403xxx-
 * func_0040Fxxx, and their second copies at func_0043Cxxx-func_00448xxx,
 * which use the same globals). Three blocks follow each other in BSS:
 *
 *   gLevelScript  LevelScriptState  the running script/boss mode (0x568)
 *   gMotherBoss  MotherBossState   the Mother boss fight (0x690)
 *   gBlindOne  BlindOneState     the Blind One boss fight (0x560), with
 *               its script VM (BlindOneStep, BlindOnePhase, BlindOneMover)
 *
 * Several fields also have symbols of their own, which some files still
 * use, and some files reach a block through a neighbouring symbol at a
 * negative offset (gLevelScript_bossMode - 0x3D4, D_8012F604, ...): those keep their
 * symbol, as the ROM addresses them that way. The extern declarations here
 * are the block-level symbols only.
 *
 * Nothing here includes structs/game_object.h, since many module files
 * still define a local CAI: actors are `struct CGameObjectInstance *`. */

#include "structs/vector.h"

/* The 0x64-byte value/colour animator ScriptAnimator_Start sets up,
 * ScriptAnimator_SetScript starts on a script (fires) and Animator_Update steps;
 * ScriptAnimator_Apply applies it when the owner is drawn (rgb and alpha to the
 * draw colour gBlendState, alpha and value to gBlendFade) and func_00236390
 * adds it to the fade. The script is a list of floats: an op (the mode),
 * then its operands; mode 0 ends it (and restarts it from loop when that
 * is set), 1 waits, 4 fades value, 6 fades rgb and alpha. The level
 * script keeps eight (lights), each Mother boss weak spot one, and the
 * Blind One a hit flash for itself and each of its actors. Rage Wars'
 * Animator (rw/abu_types.h, names from there) is the same object without
 * the last word. */
typedef struct ScriptAnimator {
    /* 0x00 */ void *owner;                /* ScriptAnimator_Start's second argument */
    /* 0x04 */ s32 mode;
    /* 0x08 */ f32 time;
    /* 0x0C */ f32 *loop;
    /* 0x10 */ f32 *script;
    /* 0x14 */ f32 value;                  /* 255 at the start */
    /* 0x18 */ f32 valueFrom;
    /* 0x1C */ f32 valueTo;
    /* 0x20 */ f32 valueDuration;
    /* 0x24 */ f32 alpha;
    /* 0x28 */ f32 alphaFrom;
    /* 0x2C */ f32 alphaTo;
    /* 0x30 */ f32 colourDuration;
    /* 0x34 */ f32 rgb[3];
    /* 0x40 */ u8 pad40[4];
    /* 0x44 */ f32 rgbFrom[3];
    /* 0x50 */ u8 pad50[4];
    /* 0x54 */ f32 rgbTo[3];
    /* 0x60 */ u8 pad60[4];
} ScriptAnimator; /* size 0x64 */

/* One weak spot of the boss: its health and its hit-flash animator. */
typedef struct BossWeakSpot {
    /* 0x00 */ s32 health;
    /* 0x04 */ ScriptAnimator anim;
} BossWeakSpot; /* size 0x68 */

/* gMotherBoss: the Mother boss fight, MotherBoss_Hurt-MotherBoss_OnDefeatCinemaDone (second
 * copy MotherBoss_Hurt_2-MotherBoss_OnDefeatCinemaDone_2; MotherBoss_OnDefeatCinemaDone sets
 * m_bMotherBossComplete). Fifteen weak spots in three phases. Fields
 * with symbols of their own: gMotherBoss_phase (phase), gMotherBoss_activeMask (activeMask),
 * gMotherBoss_liveMask (liveMask), gMotherBoss_counter (counter), gMotherBoss_minRadius / gMotherBoss_maxRadius
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
    /* 0x658 */ void *intel;                         /* CScene__FindIntelligence(world, 1, 0x1DB1) */
    /* 0x65C */ s32 phase;
    /* 0x660 */ s32 activeMask;                      /* spots that can be hit */
    /* 0x664 */ s32 liveMask;                        /* spots of the phase still alive */
    /* 0x668 */ s32 unk668;
    /* 0x66C */ s32 lastAnim;
    /* 0x670 */ s32 partsLeft;
    /* 0x674 */ s32 totalHealth;
    /* 0x678 */ s32 health;
    /* 0x67C */ s32 counter;
    /* 0x680 */ s32 pickupTimer;                     /* frames since the last random pickup (MotherBoss_UpdateSpotsAndPickups) */
    /* 0x684 */ s32 dropTimer;                       /* frames since the last drop at the spawn points */
    /* 0x688 */ s32 minRadius;
    /* 0x68C */ s32 maxRadius;
} MotherBossState; /* size 0x690 */

extern MotherBossState gMotherBoss;

/* The Blind One (the second boss, "Blind" and "EyeBall" on its health
 * bars) and the small script VM that runs it. Its module is linked twice,
 * at BlindOne_UpdateMover-BlindOne_NextPhase and again at BlindOne_Setup_2-BlindOne_NextPhase_2
 * with its data 0xE0 lower; both copies use the same state, gBlindOne, and
 * each has its own phase table (sBlindOnePhases and sBlindOnePhases_2). The script is a
 * list of BlindOneStep: stepping it (the nextPhase inlines and
 * BlindOne_PhaseWaitCinemaUpdate...) stops the three movers, clears every actor's
 * m_AI.m_BlindOne.m_nPhaseHits, stores the step's id in `phase` and calls that
 * phase's init; the phase's update then runs every frame. The boss's three
 * groups of actors (types 0x454, 0x455 and 0x456) keep their group index in
 * m_AI.m_BlindOne.m_nGroupIndex and their fader in m_AI.m_BlindOne.m_nFader
 * (BlindOne_Setup). BlindOne_Draw reaches the faders through their own
 * symbol, gBlindOne_faders (= gBlindOne.faders), and BlindOne_UpdateFight the update
 * entries of the phase table through sBlindOnePhases_update (= &sBlindOnePhases[0].update);
 * BlindOne_UpdateFight and BlindOne_Group3Draw reach the faders from the block base by
 * raw offsets, as the ROM does. */

struct BlindOneState;

/* One step of the script (sBlindOneScript...). */
typedef struct BlindOneStep {
    /* 0x00 */ s32 id;                 /* the phase: an index into the phase table */
    /* 0x04 */ f32 seconds;            /* how long the phase's movers run (BlindOne_PhaseGroup1Init) */
    /* 0x08 */ s32 unk8;
} BlindOneStep; /* size 0xC */

/* An entry of the phase table (sBlindOnePhases, sBlindOnePhases_2). */
typedef struct BlindOnePhase {
    /* 0x00 */ void (*init)(struct BlindOneState *state);
    /* 0x04 */ void (*update)(struct BlindOneState *state);
} BlindOnePhase; /* size 0x8 */

/* Plays a sequence of bit masks: every interval, each set bit sends the
 * matching actor (bits 0-3: group 1, 4-7: group 2, 8-11: group 3, rotated
 * by offset) to an attack state (BlindOne_UpdateMover). A NULL seq stops it. */
typedef struct BlindOneMover {
    /* 0x00 */ s32 *seq;
    /* 0x04 */ s32 pos;
    /* 0x08 */ f32 interval;           /* in frames: seconds * 15 */
    /* 0x0C */ f32 timer;
    /* 0x10 */ s32 step;
    /* 0x14 */ s32 offset;
} BlindOneMover; /* size 0x18 */

/* gBlindOne: the Blind One boss fight, BlindOne_UpdateMover (setup) to
 * BlindOne_Group3Draw (second copy BlindOne_Setup_2-BlindOne_ActorAppearEnter_2). Three groups of
 * level actors, each with the count of its live ones (BlindOne_ActorAppearEnter counts
 * them up, BlindOne_ActorDeathEnter down), twelve hit-flash animators, the phase
 * script and the minions the phases spawn (BlindOne_PhaseMinionsUpdate). */
typedef struct BlindOneState {
    /* 0x000 */ struct CGameObjectInstance *boss;
    /* 0x004 */ s32 count1;            /* the actors of type 0x454 */
    /* 0x008 */ struct CGameObjectInstance *actors1[4];
    /* 0x018 */ s32 count2;            /* type 0x455 */
    /* 0x01C */ struct CGameObjectInstance *actors2[4];
    /* 0x02C */ s32 count3;            /* type 0x456 */
    /* 0x030 */ struct CGameObjectInstance *actors3[3];
    /* 0x03C */ ScriptAnimator faders[12]; /* the boss's and every actor's (m_AI.m_BlindOne.m_nFader) */
    /* 0x4EC */ s32 faderCount;
    /* 0x4F0 */ void *minionDefA;      /* the definitions of its two minion kinds (CScene__FindIntelligence 0x1CE9, 0x1CEA) */
    /* 0x4F4 */ void *minionDefB;
    /* 0x4F8 */ BlindOneStep *script;  /* the current step */
    /* 0x4FC */ s32 phase;
    /* 0x500 */ s32 toSpawn;           /* minions the phase still spawns */
    /* 0x504 */ s32 minionsAlive;
    /* 0x508 */ s32 maxMinions;
    /* 0x50C */ s32 alive1;            /* live actors of each group (BlindOne_ActorAppearEnter, BlindOne_ActorDeathEnter) */
    /* 0x510 */ s32 alive2;
    /* 0x514 */ s32 alive3;
    /* 0x518 */ BlindOneMover movers[3];
} BlindOneState; /* size 0x560 */

extern BlindOneState gBlindOne;

/* A step of a boss script: what to do (type), its argument, and the odds
 * (chance, out of 100) that the runner takes it; a type marks the end
 * (4: stop, 5: wrap around, Primagen_NextAttackStep).
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

/* An attack the boss can choose (sPrimagenAttacks, indexed by a script A
 * step's arg): its AI mode and the distance range it is taken in, scaled
 * (Primagen_RunAttackStep). */
typedef struct BossAttack {
    /* 0x0 */ s32 mode;
    /* 0x4 */ f32 minDist;
    /* 0x8 */ f32 maxDist;
    /* 0xC */ s32 padC;
} BossAttack; /* size 0x10 */

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

/* gLevelScript: the state of the running level script, shared by the mode
 * handlers of the first VM module (Primagen_Hurt-Primagen_UpdateMode10, second copy
 * func_0043C7B0-Primagen_UpdateMode10_2): the stage the script is in, the lights and
 * animators, the wave spawner, the boss parts and the boss AI (its state,
 * mode and three step scripts). Each mode uses the fields it needs and
 * some reuse a word for something else; the names follow the boss, which
 * uses most of them.
 *
 * Fields with symbols of their own (and the files' views anchored there):
 * gLevelScript_swayPhase (swayPhase/swayAmount: BossWait), gLevelScript_spawnsLeft (spawnsLeft...),
 * gLevelScript_values / D_8012F9A8 (values[0] / values[1]), gLevelScript_scriptCOn
 * (scriptCOn...), gLevelScript_unk3CC (unk3CC...), gLevelScript_bossState (bossState...:
 * BossState), gLevelScript_bossMode (bossMode...: BossMode), gLevelScript_armed (armed,
 * scriptOn), gLevelScript_script (script), gLevelScript_script_steps (script.steps), gLevelScript_script_cursor
 * (script.cursor), gLevelScript_script2_cursor (script2.cursor...), gLevelScript_script3 (script3),
 * gLevelScript_path (path). sPrimagenPathPoint and D_8012F604 come just before it. */
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
    /* 0x344 */ void *intel;                         /* CScene__FindIntelligence(world, 1, ...) */
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
 * keeps &gLevelScript in a register). They define BOSS_STAGE_AS_WORD and
 * declare it as an s32 themselves (Primagen_RunAttackStep, Primagen_RunAttackStep_2,
 * Primagen_UpdateMode19, Primagen_UpdateMode19_2). */
#ifndef BOSS_STAGE_AS_WORD
extern LevelScriptState gLevelScript;
#endif

/* LevelScriptState from bossState on (gLevelScript_bossState, +0x3D0). */
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

/* LevelScriptState from bossMode on (gLevelScript_bossMode, +0x3D4). */
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

extern BossState gLevelScript_bossState;
extern BossMode gLevelScript_bossMode;
extern BossScript3 gLevelScript_script3;  /* LevelScriptState.script3 */

/* A spawn point of the Mother fight (gMotherSpawnPoints, and gMotherSpawnPoints_2 in the
 * second copy of the module): where its pickups drop (MotherBoss_DropPickupsAtSpawnPoints) and
 * around which it throws its target, inside the angle range
 * (MotherBoss_TeleportTarget). */
typedef struct SpawnPoint {
    /* 0x00 */ CVector3 pos;
    /* 0x0C */ f32 minAngle;
    /* 0x10 */ f32 maxAngle;
} SpawnPoint; /* size 0x14 */

/* A minion kind of the wave boss (sMantidQueenMinions): the intelligence
 * id MantidQueen_Init finds, and the model and spawn offset MantidQueen_SpawnMinion
 * uses. */
typedef struct WaveBossMinion {
    /* 0x00 */ s32 id;
    /* 0x04 */ s32 model;
    /* 0x08 */ CVector3 offset;
} WaveBossMinion; /* size 0x14 */

/* One step of the wave boss's script (sMantidQueenScript...): the minions of the
 * wave, how many may fight at once, and the weak spot it opens. Stepping it
 * (MantidQueen_NextStep) adds spawnCount to the minions still to come; the wave
 * runs (waveActive) when both partPicks and spawnCount are set. */
typedef struct WaveBossStep {
    /* 0x00 */ struct Pick *partPicks;   /* weighted pick of the part a hit spawns (MantidQueen_SpawnMinion) */
    /* 0x04 */ s32 spawnCount;
    /* 0x08 */ s32 maxActive;            /* minions sent in at once (MantidQueen_SendMinions) */
    /* 0x0C */ s32 unkC;                 /* allows the long-range attack (MantidQueen_ChooseAttack) */
    /* 0x10 */ struct Pick *picks10;     /* MantidQueen_PickFromStep */
    /* 0x14 */ s32 weakSpot;             /* the spot open to hits, -1: none */
    /* 0x18 */ f32 onTime;               /* the blink of the open spot */
    /* 0x1C */ f32 offTime;
} WaveBossStep; /* size 0x20 */

/* gMantidQueen: the boss of the first VM module (MantidQueen_Init sets it up,
 * MantidQueen_Update runs it; boss id 0x1D4D in gScene_m_LevelID, between the Blind
 * One's and the Mother's): the Mantid Queen, whose intro starts the "Queen"
 * health bar (MantidQueen_UpdateIntro). Five weak spots,
 * opened one at a time by the script, and waves of minions (type 0x651) in
 * between: while a wave runs the spots cannot be hit. Fields with symbols
 * of their own: gMantidQueen_pushed (pushed), gMantidQueen_step (step), gMantidQueen_boss (boss),
 * gMantidQueen_waveActive (waveActive), gMantidQueen_blink (blink). */
typedef struct WaveBossState {
    /* 0x000 */ struct CGameObjectInstance *actor;   /* the script owner (MantidQueen_EnterStart) */
    /* 0x004 */ s32 pushed;                          /* the push attack hit (MantidQueen_UpdatePush) */
    /* 0x008 */ BossWeakSpot spots[5];
    /* 0x210 */ s32 stepIndex;
    /* 0x214 */ WaveBossStep *step;
    /* 0x218 */ s32 toSpawn;                         /* minions of the wave still to come */
    /* 0x21C */ s32 timer;
    /* 0x220 */ struct CGameObjectInstance *boss;    /* the 0x457 actor */
    /* 0x224 */ struct CGameObjectInstance *members[8]; /* the 0x651 minions */
    /* 0x244 */ s32 memberCount;
    /* 0x248 */ void *intel[2];                      /* CScene__FindIntelligence(world, 1, sMantidQueenMinions[i].id) */
    /* 0x250 */ s32 waveActive;
    /* 0x254 */ s32 blink;
    /* 0x258 */ f32 blinkTimer;
} WaveBossState; /* size 0x25C */

extern WaveBossState gMantidQueen;
extern WaveBossStep *gMantidQueen_step;  /* WaveBossState.step */

#endif
