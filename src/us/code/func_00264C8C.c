#include "common.h"
#include "structs/game_object.h"

void Throwable_SetModeAnim(sptr arg0, CAI *arg1, sptr arg2) {
#ifdef TARGET_PORT
    arg1->m_VarPtr[0] = (void *) arg2; /* the owner, pointer-wide (func_002649A0.c) */
#else
    arg1->m_VarBuffer[0] = arg2;
#endif
}
