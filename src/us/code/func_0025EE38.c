#include "common.h"
#include "structs/game_object.h"
#include "globals/scene.h"

s32 CScene__GetInstanceSavedBit(void *, void *);               /* extern */
s32 Cinema_PathPlaying();                                /* extern */

void EnemyAI_CheckOffscreen(CGameObjectInstance *arg0, s32 *arg1) {
    if (CScene__GetInstanceSavedBit(&gScene, arg0) == 0) {
        arg0->m_dwFlags = (s32) (arg0->m_dwFlags | 0x2100);
    }
    if (Cinema_PathPlaying() != 0) {
        *arg1 |= 0x200;
    }
}
