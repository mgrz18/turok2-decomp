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
