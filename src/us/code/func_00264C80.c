#include "common.h"
#include "structs/game_object.h"

s32 Throwable_GetModeAnim(sptr arg0, CAI *arg1) {
#ifdef TARGET_PORT
    return (sptr) arg1->m_VarPtr[0]; /* the owner, pointer-wide (func_002649A0.c) */
#else
    return arg1->m_VarBuffer[0];
#endif
}
