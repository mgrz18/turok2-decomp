#include "common.h"
#include "structs/game_object.h"
#include "protos/ai.h"

/* the boss by member (round 9 x2): gMantidQueen.boss is a pointer, the ROM's s32 symbol cut it to its
 * low half (--high-memory) and its N64 AI offset (+ 0x140) is not the host CAI's */
#include "structs/boss.h"
#include "globals/boss.h"
#define T2_QUEEN_PLAY(anim) AI_PlayAnim(gMantidQueen.boss, &gMantidQueen.boss->m_AI, (anim))

void MantidQueen_EnterMode4(CGameObjectInstance *arg0) {
    T2_QUEEN_PLAY(0x5AB4);
    arg0->m_dwFlags = (s32) (arg0->m_dwFlags & 0xFFFEFFFF);
}
