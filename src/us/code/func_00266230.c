#include "common.h"
#include "structs/game_object.h"
#include "globals/scene.h"

void CScene__SetInstanceSavedBit(void *, s32, s32);               /* extern */
void GameObject_FireTriggers(s32, s32, s32);                  /* extern */

void Action_IdleEnter(s32 arg0, CAI *arg1) {
    CScene__SetInstanceSavedBit(&gScene, arg0, 0);
    GameObject_FireTriggers(arg0, 0x200000, arg0);
    arg1->m_VarBuffer[2] = 0;
}
