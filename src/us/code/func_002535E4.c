#include "common.h"
#include "structs/player.h"

/* The player list (D_80119870): the first player at 0x20. */
typedef struct {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ CPlayer *first;
} PlayerList;

void func_002535E4(PlayerList *arg0, f32 arg1) {
    CPlayer *var_v0;

    var_v0 = arg0->first;
    if (var_v0 != NULL) {
        do {
            var_v0->m_InvincibilityTime = arg1;
            var_v0 = var_v0->m_pNext;
        } while (var_v0 != NULL);
    }
}
