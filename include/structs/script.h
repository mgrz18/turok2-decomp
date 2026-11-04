#ifndef STRUCTS_SCRIPT_H
#define STRUCTS_SCRIPT_H

/* The second boss (the Blind One, "Blind" on its health bar) and the small
 * script VM that runs it: BlindOneState, D_80130200.
 *
 * The boss module is linked twice, at 0x0040C850-0x0040EF64 and again at
 * 0x00445CC0-0x00447F64 with its data 0xE0 lower; both copies use the same
 * state, D_80130200, and each has its own phase table (D_0043240C and
 * D_0043232C). The script is a list of BlindOneStep: stepping it (the
 * nextPhase inlines and func_0040E46C...) stops the three movers, clears
 * every actor's m_AI.m_VarBuffer[0], stores the step's id in `phase` and
 * calls that phase's init; the phase's update then runs every frame.
 *
 * The boss's three groups of actors (types 0x454, 0x455 and 0x456) keep
 * their group index in m_AI.m_VarBuffer[1] and their fader in
 * m_AI.m_VarBuffer[2] (func_0040CCC0). func_0040EDB0 reaches the faders
 * through their own symbol, D_8013023C (= D_80130200.faders), and
 * func_0040ECDC the update entries of the phase table through D_00432410
 * (= &D_0043240C[0].update). */

struct CGameObjectInstance;
struct BlindOneState;

/* One step of the script. */
typedef struct BlindOneStep {
    /* 0x00 */ s32 id;                 /* the phase: an index into the phase table */
    /* 0x04 */ f32 seconds;            /* how long the phase's movers run (func_0040E59C) */
    /* 0x08 */ s32 unk8;
} BlindOneStep; /* size 0xC */

/* An entry of the phase table (D_0043240C, D_0043232C). */
typedef struct BlindOnePhase {
    /* 0x00 */ void (*init)(struct BlindOneState *state);
    /* 0x04 */ void (*update)(struct BlindOneState *state);
} BlindOnePhase; /* size 0x8 */

/* Plays a sequence of bit masks: every interval, each set bit sends the
 * matching actor (bits 0-3: group 1, 4-7: group 2, 8-11: group 3, rotated
 * by offset) to an attack state (func_0040C850). A NULL seq stops it. */
typedef struct BlindOneMover {
    /* 0x00 */ s32 *seq;
    /* 0x04 */ s32 pos;
    /* 0x08 */ f32 interval;           /* in frames: seconds * 15 */
    /* 0x0C */ f32 timer;
    /* 0x10 */ s32 step;
    /* 0x14 */ s32 offset;
} BlindOneMover; /* size 0x18 */

/* The hit flash of the boss or one of its actors (func_002362E4 starts it,
 * func_00236314 fires it when hit, func_00236324 applies it when drawn). */
typedef struct BlindOneFader {
    /* 0x00 */ u8 data[0x64];
} BlindOneFader; /* size 0x64 */

/* The boss state, D_80130200. */
typedef struct BlindOneState {
    /* 0x000 */ struct CGameObjectInstance *boss;
    /* 0x004 */ s32 count1;            /* the actors of type 0x454 */
    /* 0x008 */ struct CGameObjectInstance *actors1[4];
    /* 0x018 */ s32 count2;            /* type 0x455 */
    /* 0x01C */ struct CGameObjectInstance *actors2[4];
    /* 0x02C */ s32 count3;            /* type 0x456 */
    /* 0x030 */ struct CGameObjectInstance *actors3[3];
    /* 0x03C */ BlindOneFader faders[12]; /* the boss's and every actor's (m_AI.m_VarBuffer[2]) */
    /* 0x4EC */ s32 faderCount;
    /* 0x4F0 */ void *minionDefA;      /* the intelligences of its two minion kinds (func_00225EBC 0x1CE9, 0x1CEA) */
    /* 0x4F4 */ void *minionDefB;
    /* 0x4F8 */ BlindOneStep *script;  /* the current step */
    /* 0x4FC */ s32 phase;
    /* 0x500 */ s32 toSpawn;           /* minions left to spawn (func_0040D0F0) */
    /* 0x504 */ s32 alive;             /* minions alive */
    /* 0x508 */ s32 maxAlive;
    /* 0x50C */ s32 alive1;            /* actors of each group alive (func_0040E00C, func_0040E30C) */
    /* 0x510 */ s32 alive2;
    /* 0x514 */ s32 alive3;
    /* 0x518 */ BlindOneMover movers[3];
} BlindOneState; /* size 0x560 */

#endif
