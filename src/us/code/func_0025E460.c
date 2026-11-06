#include "common.h"
#include "structs/game_object.h"

/* How this actor uses CAI's m_VarBuffer (as in EnemyAI_IdleSoundCallback.c). */
typedef struct {
    /* 0x00 */ u8 pad0[0x24];
    /* 0x24 */ s8 mode[3];
    /* 0x27 */ s8 modeIndex;
} AIModeVars;

void EnemyAI_SetBehavior(s32 arg0, CAI *arg1, s32 arg2, s8 arg3) {
    ((AIModeVars *) arg1->m_VarBuffer)->mode[arg2] = arg3;
}
