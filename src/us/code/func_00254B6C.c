#include "common.h"
#include "structs/player.h"
#include "structs/camera.h"

/* The players: the ones not in the game and the ones in it (as in
 * func_00251FC4.c). */
typedef struct {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ CList m_IdleList;
    /* 0x20 */ CList m_ActiveList;
} CPlayerList;

void func_00254B6C(CPlayerList *arg0) {
    struct CPlayerOptions *temp_a0;
    CPlayer *var_v1;

    var_v1 = arg0->m_ActiveList.m_pHead;
    if (var_v1 != NULL) {
        do {
            temp_a0 = var_v1->m_pOptions;
            if (temp_a0->m_LastTagMode == 1) {
                temp_a0->m_LastTagMode = 0U;
            }
            var_v1 = var_v1->m_pNext;
        } while (var_v1 != NULL);
    }
}
