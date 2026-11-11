#include "common.h"
#include "structs/game_object.h"
#include "protos/ai.h"

#ifdef TARGET_PORT
/* the boss by member (round 9 x2): gMantidQueen.boss is a pointer, the ROM's s32 symbol cut it to its
 * low half (--high-memory) and its N64 AI offset (+ 0x140) is not the host CAI's */
#include "structs/boss.h"
#include "globals/boss.h"
#define T2_QUEEN_PLAY(anim) AI_PlayAnim(gMantidQueen.boss, &gMantidQueen.boss->m_AI, (anim))
#else
extern s32 gMantidQueen_boss; /* local type: the field's real type (or the member path) changes the ROM's code here */
#define T2_QUEEN_PLAY(anim) AI_PlayAnim(gMantidQueen_boss, gMantidQueen_boss + 0x140, (anim))
#endif

void MantidQueen_EnterMode3(CGameObjectInstance *arg0) {
    T2_QUEEN_PLAY(0x5AAA);
    arg0->m_dwFlags = (s32) (arg0->m_dwFlags | 0x2000);
}
