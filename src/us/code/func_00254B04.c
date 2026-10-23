#include "common.h"
#include "structs/player.h"
#include "structs/camera.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x3F];
    /* 0x3F */ u8 field3F;
} PlayerOptions;

typedef struct {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ CPlayer *head;
} PlayerList;

/* The index-th (0-3) player with a camera whose camera word 0x54C and
 * options byte 0x3F are both clear. */
CPlayer *func_00254B04(PlayerList *list, u32 index) {
    CPlayer *p = NULL;
    s32 n = 0;

    if (index < 4) {
        for (p = list->head; p != NULL; p = p->m_pNext) {
            if (p->m_pCamera != NULL && *(s32 *)((u8 *)p->m_pCamera + 0x54C) == 0
                && ((PlayerOptions *)p->m_pOptions)->field3F == 0) {
                if (n == index) {
                    return p;
                }
                n++;
            }
        }
    }
    return p;
}
