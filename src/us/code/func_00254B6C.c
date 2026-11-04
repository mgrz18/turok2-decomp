#include "common.h"
#include "structs/player.h"
#include "structs/camera.h"

/* Multiplayer options kept per player (CPlayer.m_pOptions; see
 * func_00247C10.c): only the byte at 0x3F here. */
struct CPlayerOptions {
    /* 0x00 */ u8 pad0[0x3F];
    /* 0x3F */ u8 unk3F;
};

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
            if (temp_a0->unk3F == 1) {
                temp_a0->unk3F = 0U;
            }
            var_v1 = var_v1->m_pNext;
        } while (var_v1 != NULL);
    }
}
