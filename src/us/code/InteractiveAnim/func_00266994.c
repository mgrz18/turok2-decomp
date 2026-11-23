#include "common.h"
#include "structs/game_object.h"
#include "globals/scene.h"
#include "protos/scene.h"

s32 CGameObjectInstance__AdvanceAnimation(void *);                      /* extern */
s32 Cinema_PathPlaying();                                /* extern */

void InteractiveAnim_EventShow(CGameObjectInstance *arg0, s32 *arg1) {
    arg0->m_dwFlags = (s32) (arg0->m_dwFlags | 0x2100);
    if (Cinema_PathPlaying() != 0) {
        *arg1 |= 0x200;
    }
    CGameObjectInstance__AdvanceAnimation(arg0);
    CScene__SetInstanceSavedBit(&gScene, arg0, 0);
}
