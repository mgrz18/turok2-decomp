#include "common.h"
#include "structs/game_object.h"

/* How this actor uses CAI's m_VarBuffer (as in func_0025C1B0.c). */
typedef struct {
    /* 0x00 */ u8 pad0[0x24];
    /* 0x24 */ s8 mode[3];
    /* 0x27 */ s8 modeIndex;
} AIModeVars;

s32 func_00243414(CGameObjectInstance *obj, CAI *ai, s32 mode);

void func_0025F784(CGameObjectInstance *obj, CAI *ai) {
    ((AIModeVars *) ai->m_VarBuffer)->modeIndex = 1;
    if (ai->m_AnimRepeatedCount != 0) {
        func_00243414(obj, ai, 4);
    }
}
