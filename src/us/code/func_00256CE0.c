#include "common.h"
#include "structs/player.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ CPlayer *head;
} PlayerList;

f32 func_0020F6BC(s32, void *);                 /* extern */

f32 func_00256CE0(PlayerList *arg0, s32 arg1) {
    f32 var_f20;
    CPlayer *var_s0;

    var_s0 = arg0->head;
    var_f20 = 0.0f;
    if (var_s0 != NULL) {
        do {
            if (var_s0->m_pCamera != 0) {
                var_f20 += func_0020F6BC(arg1, &var_s0->m_CharacterActor.header.m_vPos);
            }
            var_s0 = var_s0->m_pNext;
        } while (var_s0 != NULL);
    }
    return var_f20;
}
