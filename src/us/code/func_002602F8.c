#include "common.h"
#include "structs/game_object.h"

/* The AI mode scratch (CAI::m_VarBuffer at 0x10C): three slot flags. */
typedef struct {
    /* 0x000 */ u8 pad0[0x130];
    /* 0x130 */ s8 slot[3];
} AI;

s32 func_002602F8(CGameObjectInstance *arg0, AI *arg1) {
    CEnemyIntelligence *temp_a0;

    temp_a0 = &(arg0->header.m_pIntelligence)->m_Enemy;
    if (((arg1->slot[0] != 0) && (temp_a0->m_wComboEndAnimFlags & 0x1F)) || ((arg1->slot[1] != 0) && (temp_a0->m_wComboEndAnimFlags & 0x3E0)) || ((arg1->slot[2] != 0) && (temp_a0->m_wComboEndAnimFlags & 0x7C00))) {
        return 1;
    }
    return 0;
}
