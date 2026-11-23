#include "common.h"
#include "structs/game_object.h"
#include "protos/ai.h"


void Throwable_Advance(sptr arg0, CAI *arg1) {
#ifdef TARGET_PORT
    /* the tilt (N64 [1], [2]); [1] is the high half of the host owner pointer (func_002649A0.c) */
    arg1->m_VarF32[2] = 0.0f;
    arg1->m_VarF32[3] = 0.0f;
#else
    arg1->m_VarBuffer[1] = 0;
    arg1->m_VarBuffer[2] = 0;
#endif
    CAI__SetMode(arg0, arg1, 0);
}
