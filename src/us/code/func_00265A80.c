#include "common.h"
#include "structs/game_object.h"
#include "globals/scene.h"

s32 CScene__GetInstanceSavedBit(void *, void *);                  /* extern */
void CAI__SetMode(void *, CAI *, s32);             /* extern */

void Destructible_Advance(CGameObjectInstance *arg0, CAI *arg1) {
    if (CScene__GetInstanceSavedBit(&gScene, arg0) == 1) {
        CAI__SetMode(arg0, arg1, 1);
        return;
    }
    CAI__SetMode(arg0, arg1, 0);
    arg0->m_dwFlags = (s32) (arg0->m_dwFlags | 0x10000);
}
