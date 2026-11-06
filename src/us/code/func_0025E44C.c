#include "common.h"
#include "structs/game_object.h"

/* How this actor uses CAI's m_VarBuffer (as in EnemyAI_IdleSoundCallback.c). */
typedef struct {
    /* 0x00 */ u8 pad0[0x24];
    /* 0x24 */ s8 mode[3];
    /* 0x27 */ s8 modeIndex;
} AIModeVars;

/* The current mode entry. */
s8 EnemyAI_GetCurrentBehavior(s32 arg0, CAI *arg1) {
    return ((AIModeVars *) arg1->m_VarBuffer)->mode[((AIModeVars *) arg1->m_VarBuffer)->modeIndex];
}
