#include "common.h"
#include "structs/game_object.h"
#include "globals/scene.h"
#include "protos/scene.h"
#include "protos/game.h"


void Action_IdleEnter(sptr arg0, CAI *arg1) {
    CScene__SetInstanceSavedBit(&gScene, arg0, 0);
    GameObject_FireTriggers(arg0, 0x200000, arg0);
    arg1->m_VarBuffer[2] = 0;
}
