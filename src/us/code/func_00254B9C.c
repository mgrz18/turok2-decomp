#include "common.h"
#include "structs/player.h"

typedef struct {
    /* 0x000 */ u8 pad0[0x54C];
    /* 0x54C */ s32 unk54C;
} Owner;

/* Counts the players whose owner has unk54C clear and settings unk3F clear. */
s32 func_00254B9C(CPlayerList *arg0) {
    s32 var_a0;
    Owner *temp_v0;
    CPlayer *var_v1;

    var_v1 = arg0->m_ActiveList.m_pHead;
    var_a0 = 0;
    if (var_v1 != NULL) {
        do {
            temp_v0 = var_v1->m_pCamera;
            if ((temp_v0 != NULL) && (temp_v0->unk54C == 0) && (var_v1->m_pOptions->m_LastTagMode == 0)) {
                var_a0 += 1;
            }
            var_v1 = var_v1->m_pNext;
        } while (var_v1 != NULL);
    }
    return var_a0;
}
