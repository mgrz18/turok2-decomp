#include "common.h"
#include "structs/player.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ s32 pending;
    /* 0x20 */ s32 done;
} GameState;

extern GameState D_8011ACB0;

/* Once every player has options byte 0x3D set to 1, turns the pending flag
 * into the done flag and returns 1. */
s32 func_00253460(CPlayerList *list) {
    GameState *state = &D_8011ACB0;
    CPlayer *p;
    s32 n;

    if (state->done) {
        return 0;
    }
    if (state->pending) {
        n = 0;
        for (p = list->m_ActiveList.m_pHead; p != NULL; p = p->m_pNext) {
            if (((CPlayerOptions *)p->m_pOptions)->unk_0x3D == 1) {
                n++;
            }
        }
        if (n == list->m_ActiveList.m_nCount) {
            GameState *s = &D_8011ACB0;

            s->done = 1;
            s->pending = 0;
            return 1;
        }
    }
    return 0;
}
