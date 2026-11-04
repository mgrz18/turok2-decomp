#include "common.h"
#include "structs/player.h"

s32 func_0024C648(void *, s32);                 /* extern */
s32 func_002812CC(s32);                         /* extern */

void func_0025322C(CPlayerList *arg0, s32 arg1) {
    CPlayer *var_s0;

    func_002812CC(arg1);
    var_s0 = arg0->m_ActiveList.m_pHead;
    if (var_s0 != NULL) {
        do {
            if ((s32) var_s0->m_pCamera == arg1) {
                func_0024C648(var_s0, arg1);
            }
            var_s0 = var_s0->m_pNext;
        } while (var_s0 != NULL);
    }
}
