#include "common.h"
#include "structs/game_object.h"

/* A table of up to 0x80 actors at 0xBF8 with its count at 0xDF8. */
typedef struct {
    /* 0x000 */ u8 pad0[0xBF8];
    /* 0xBF8 */ CGameObjectInstance *actors[0x80];
    /* 0xDF8 */ s32 count;
} ActorTable;

/* Clears m_AI.m_pParent on every actor whose parent is arg1. */
void func_00225B44(ActorTable *arg0, s32 arg1) {
    s32 i;
    s32 n;
    CGameObjectInstance **p;

    i = 0;
    n = arg0->count;
    p = arg0->actors;
    if (n > 0) {
        do {
            if ((s32) (*p)->m_AI.m_pParent == arg1) {
                (*p)->m_AI.m_pParent = NULL;
            }
            i++;
            p++;
        } while (i < n);
    }
}
