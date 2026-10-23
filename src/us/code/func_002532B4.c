#include "common.h"
#include "structs/player.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x3E];
    /* 0x3E */ u8 field3E;
} PlayerOptions;

typedef struct {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ CPlayer *head;
} PlayerList;

/* The first player whose options byte 0x3E is 1. */
CPlayer *func_002532B4(PlayerList *list) {
    CPlayer *p;

    for (p = list->head; p != NULL; p = p->m_pNext) {
        if (((PlayerOptions *)p->m_pOptions)->field3E == 1) {
            return p;
        }
    }
    return NULL;
}
