#include "common.h"
#include "structs/game_object.h"

void Throwable_SetModeAnim(sptr arg0, CAI *arg1, sptr arg2) {
    arg1->m_VarBuffer[0] = arg2;
}
