#include "common.h"
#include "structs/game_object.h"

s32 CGameObjectInstance__AdvanceAnimation(void *);                      /* extern */
s32 CScene__SetInstanceSavedBit(s32 *, void *, s32);  /* extern */
s32 Cinema_PathPlaying();                                /* extern */
extern s32 gScene;

void InteractiveAnim_EventShow(CGameObjectInstance *arg0, s32 *arg1) {
    arg0->m_dwFlags = (s32) (arg0->m_dwFlags | 0x2100);
    if (Cinema_PathPlaying() != 0) {
        *arg1 |= 0x200;
    }
    CGameObjectInstance__AdvanceAnimation(arg0);
    CScene__SetInstanceSavedBit(&gScene, arg0, 0);
}
