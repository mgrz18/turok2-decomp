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

void func_00249898(void *, void *);                 /* extern */

void func_002531D0(CPlayerList *arg0, CCamera *arg1) {
    CPlayer *var_s0;

    var_s0 = arg0->m_ActiveList.m_pHead;
    if (var_s0 != NULL) {
        do {
            if ((var_s0->m_pCamera == arg1) && (arg1->m_ViewParams.m_nType == 0)) {
                func_00249898(var_s0, arg1);
            }
            var_s0 = var_s0->m_pNext;
        } while (var_s0 != NULL);
    }
}
