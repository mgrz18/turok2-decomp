#include "common.h"
#include "structs/game_object.h"
#include "globals/scene.h"
#include "protos/scene.h"
#include "protos/enemy_ai.h"

s32 Cinema_PathPlaying();                                /* extern */

/* The third argument is not read (EnemyAI_SleepIfOffscreen passes its own on). */
void EnemyAI_CheckOffscreen(CGameObjectInstance *arg0, CAI *arg1, s32 unused) {
    if (CScene__GetInstanceSavedBit(&gScene, arg0) == 0) {
        arg0->m_dwFlags = (s32) (arg0->m_dwFlags | 0x2100);
    }
    if (Cinema_PathPlaying() != 0) {
        arg1->m_dwFlags |= 0x200;
    }
}
