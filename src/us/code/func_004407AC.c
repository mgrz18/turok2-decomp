/* The port (round 12 C27): the module-2 twin of Primagen_EnterMode24 (Primagen_EnterMode24, round 12 a2):
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
