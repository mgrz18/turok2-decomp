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

s32 func_0020B624(void);
s32 func_0024972C(s32, s32);
s32 func_002842F8(void);

void func_0020B5D4();                               /* extern */

void func_00253148(CPlayerList *arg0, CCamera *arg1) {
    CPlayer *var_s0;

    func_0020B5D4();
    var_s0 = arg0->m_ActiveList.m_pHead;
    if (var_s0 != NULL) {
        do {
            if ((var_s0->m_pCamera != arg1) || (arg1->m_ViewParams.m_nType == 1) || (func_002842F8() != 0)) {
                func_0024972C((s32) var_s0, (s32) arg1);
            }
            var_s0 = var_s0->m_pNext;
        } while (var_s0 != NULL);
    }
    func_0020B624();
}
