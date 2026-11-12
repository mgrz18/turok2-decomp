#ifdef TARGET_PORT
/* The port (round 12 C27): the module-2 twin of Primagen_EnterMode24 (func_004077AC, round 12 a2):
 * the raw actor offsets 0x1FC (m_AI.m_AnimSpeed) and 0xD4 (m_dwFlags) and the N64 deltas of
 * LevelScriptState from bossState (timer +0xC, dest.y +0x5C, home.y +0x68) do not hold on the host
 * (pointer-wide members). Members instead. Only in the compile-only module-2 library. */
#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"
#include "globals/boss.h"

void Primagen_EnterMode24_2(CGameObjectInstance *actor) {
    actor->m_AI.m_AnimSpeed = 1.0f;
    actor->m_dwFlags |= 0x2000;
    gLevelScript.timer = 1.0f;
    if (gLevelScript.bossState == 3) {
        gLevelScript.dest.y = gLevelScript.home.y;
    } else {
        gLevelScript.dest.y = 0.0f;
    }
}
#else
typedef signed int s32;
typedef float f32;

typedef struct {
    /* 0x00 */ f32 value;
    /* 0x04 */ char pad4[0x4C];
    /* 0x50 */ f32 out;
    /* 0x54 */ char pad54[0x8];
    /* 0x5C */ f32 src;
} SubState;

typedef struct {
    /* 0x00 */ s32 mode;
    /* 0x04 */ char pad4[0x8];
    /* 0x0C */ SubState sub;
} State;

extern State gLevelScript_bossState;

void Primagen_EnterMode24_2(char *actor) {
    f32 v = 1.0f;
    SubState *sub = &gLevelScript_bossState.sub;

    *(f32 *) (actor + 0x1FC) = v;
    *(s32 *) (actor + 0xD4) |= 0x2000;
    sub->value = v;
    if (gLevelScript_bossState.mode == 3) {
        v = sub->src;
        sub->out = v;
    } else {
        sub->out = 0.0f;
    }
}
#endif
