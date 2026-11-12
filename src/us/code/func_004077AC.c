#include "structs/game_object.h"
#include "globals/boss.h"


/* Fields addressed relative to gLevelScript_timer; the mode word sits 0xC before it. */
typedef struct {
    f32 unk_0;                  /* 0x00 */
    char pad4[0x50 - 0x4];
    f32 unk_50;                 /* 0x50 */
    char pad54[0x5C - 0x54];
    f32 unk_5C;                 /* 0x5C */
} State;

#ifdef TARGET_PORT
/* The port (round 12 a2): State views LevelScriptState from its timer with N64 deltas (0x50 dest.y,
 * 0x5C home.y, -0xC bossState); the host's script blocks hold pointers, so dest and home sit later
 * and the write missed dest.y: mode 24 then waited for good for the Primagen to reach a dest.y it
 * never set (stage 2's bossState 6 path, he lands on the floor at y 0). Members instead. */
void Primagen_EnterMode24(CGameObjectInstance *actor) {
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
void Primagen_EnterMode24(CGameObjectInstance *actor) {
    f32 one = 1.0f;
    State *state;

    actor->m_AI.m_AnimSpeed = one;
    actor->m_dwFlags |= 0x2000;
    state = &gLevelScript.timer;
    state->unk_0 = one;
    if (((s32 *)state)[-3] == 3) {
        state->unk_50 = state->unk_5C;
    } else {
        state->unk_50 = 0;
    }
}
#endif
