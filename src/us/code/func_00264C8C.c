#include "common.h"
#include "structs/game_object.h"

void Throwable_SetModeAnim(sptr arg0, CAI *arg1, sptr arg2) {
    arg1->m_VarPtr[0] = (void *) arg2; /* the owner, pointer-wide (func_002649A0.c) */
}
