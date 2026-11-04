#include "common.h"
#include "structs/game_object.h"

/* How this actor uses CAI's m_VarBuffer (as in func_0025C1B0.c). */
typedef struct {
    /* 0x00 */ u8 pad0[0x24];
    /* 0x24 */ s8 mode[3];
    /* 0x27 */ s8 modeIndex;
} AIModeVars;

s8 func_0025E43C(s32 arg0, CAI *arg1, s32 arg2) {
    return ((AIModeVars *) arg1->m_VarBuffer)->mode[arg2];
}
