#include "common.h"
#include "structs/game_object.h"

/* How this actor uses CAI's m_VarBuffer (as in EnemyAI_IdleSoundCallback.c). */
typedef struct {
    /* 0x00 */ u8 pad0[0x24];
    /* 0x24 */ s8 mode[3];
    /* 0x27 */ s8 modeIndex;
} AIModeVars;

s32 CAI__SetMode(CGameObjectInstance *obj, CAI *ai, s32 mode);

void EnemyMode_AirToGround_Update(CGameObjectInstance *obj, CAI *ai) {
    ((AIModeVars *) ai->m_VarBuffer)->modeIndex = 0;
    if (ai->m_AnimRepeatedCount != 0) {
        CAI__SetMode(obj, ai, 4);
    }
}
