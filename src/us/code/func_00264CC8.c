#include "common.h"
#include "structs/game_object.h"
#include "protos/ai.h"


void Throwable_Advance(sptr arg0, CAI *arg1) {
    arg1->m_VarBuffer[1] = 0;
    arg1->m_VarBuffer[2] = 0;
    CAI__SetMode(arg0, arg1, 0);
}
