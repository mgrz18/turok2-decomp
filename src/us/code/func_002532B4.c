#include "common.h"
#include "structs/player.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ CPlayer *head;
} PlayerList;

/* The first player whose tag mode (options 0x3E) is 1. */
CPlayer *func_002532B4(PlayerList *list) {
    CPlayer *p;

    for (p = list->head; p != NULL; p = p->m_pNext) {
        if (p->m_pOptions->m_TagMode == 1) {
            return p;
        }
    }
    return NULL;
}
