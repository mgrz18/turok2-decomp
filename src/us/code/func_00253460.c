#include "common.h"
#include "structs/player.h"
#include "structs/game.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x3D];
    /* 0x3D */ u8 field3D;
} PlayerOptions;

typedef struct {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ CPlayer *head;
    /* 0x24 */ u8 pad24[0xC];
    /* 0x30 */ s32 count;
} PlayerList;


/* Once every player has options byte 0x3D set to 1, turns the pending flag
 * into the done flag and returns 1. */
s32 func_00253460(PlayerList *list) {
    GameMatch *state = &D_8011ACB0;
    CPlayer *p;
    s32 n;

    if (state->done) {
        return 0;
    }
    if (state->over) {
        n = 0;
        for (p = list->head; p != NULL; p = p->m_pNext) {
            if (((PlayerOptions *)p->m_pOptions)->field3D == 1) {
                n++;
            }
        }
        if (n == list->count) {
            GameMatch *s = &D_8011ACB0;

            s->done = 1;
            s->over = 0;
            return 1;
        }
    }
    return 0;
}
